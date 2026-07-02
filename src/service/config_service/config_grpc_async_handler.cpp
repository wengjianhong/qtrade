/// @file      config_grpc_async_handler.cpp
/// @brief     ConfigService Async + CQ RPC 实现
/// @author    wengjianhong
/// @date      2026-06-28
/// @copyright CC BY-NC-SA 4.0
#include "service/config_service/config_grpc_async_handler.hpp"

#include "common/grpc/call_data_base.hpp"

#include <grpcpp/alarm.h>
#include <grpcpp/grpcpp.h>
#include <spdlog/spdlog.h>

#include <mutex>
#include <unordered_set>
#include <utility>

namespace qtrade::service {

namespace detail {

class WatchConfigCallData;

/// @brief WatchConfig 会话注册表（版本 bump 时 SchedulePush）
class WatchSessionRegistry {
 public:
  void Add(WatchConfigCallData* call);
  void Remove(WatchConfigCallData* call);
  void NotifyAll();

 private:
  std::mutex mutex_;
  std::unordered_set<WatchConfigCallData*> sessions_;
};

class GetConfigCallData final : public qtrade::common::grpc_async::CallDataBase {
 public:
  GetConfigCallData(ConfigGrpcAsyncHandler* handler,
                    qtrade::config::v1::ConfigService::AsyncService* service,
                    grpc::ServerCompletionQueue* cq,
                    std::shared_ptr<ConfigStore> store)
    : handler_(handler), service_(service), cq_(cq), store_(std::move(store)), responder_(&ctx_) {
    Proceed(true);
  }

  void Proceed(bool ok) override {
    if (!ok) {
      delete this;
      return;
    }

    if (status_ == CallStatus::kCreate) {
      status_ = CallStatus::kProcess;
      service_->RequestGetConfig(&ctx_, &request_, &responder_, cq_, cq_, this);
      return;
    }

    if (status_ == CallStatus::kProcess) {
      status_ = CallStatus::kFinish;
      response_ = store_->GetSnapshot();
      responder_.Finish(response_, grpc::Status::OK, this);
      return;
    }

    handler_->SpawnGetConfig();
    delete this;
  }

 private:
  enum class CallStatus { kCreate, kProcess, kFinish };

  ConfigGrpcAsyncHandler* handler_;
  qtrade::config::v1::ConfigService::AsyncService* service_;
  grpc::ServerCompletionQueue* cq_;
  std::shared_ptr<ConfigStore> store_;
  grpc::ServerContext ctx_;
  qtrade::config::v1::GetConfigRequest request_;
  qtrade::config::v1::ConfigSnapshot response_;
  grpc::ServerAsyncResponseWriter<qtrade::config::v1::ConfigSnapshot> responder_;
  CallStatus status_ = CallStatus::kCreate;
};

class WatchConfigCallData final : public qtrade::common::grpc_async::CallDataBase {
 public:
  WatchConfigCallData(ConfigGrpcAsyncHandler* handler,
                      qtrade::config::v1::ConfigService::AsyncService* service,
                      grpc::ServerCompletionQueue* cq,
                      std::shared_ptr<ConfigStore> store,
                      WatchSessionRegistry* registry)
    : handler_(handler), service_(service), cq_(cq), store_(std::move(store)), registry_(registry), writer_(&ctx_) {
    Proceed(true);
  }

  void SchedulePush() {
    if (finished_) {
      return;
    }
    alarm_.Set(cq_, gpr_now(GPR_CLOCK_MONOTONIC), this);
  }

  void Proceed(bool ok) override {
    if (!ok && status_ != CallStatus::kCreate) {
      Finish(grpc::Status(grpc::StatusCode::CANCELLED, "cancelled"));
      return;
    }

    if (status_ == CallStatus::kCreate) {
      status_ = CallStatus::kAccept;
      service_->RequestWatchConfig(&ctx_, &request_, &writer_, cq_, cq_, this);
      return;
    }

    if (status_ == CallStatus::kAccept) {
      if (!accepted_) {
        accepted_ = true;
        since_version_ = request_.since_version();
        registry_->Add(this);
        handler_->SpawnWatchConfig();
      }
      TryWrite();
      return;
    }

    if (status_ == CallStatus::kWrite) {
      write_in_flight_ = false;
      if (!ok) {
        Finish(grpc::Status(grpc::StatusCode::CANCELLED, "write failed"));
        return;
      }
      since_version_ = outgoing_.version();
      status_ = CallStatus::kAccept;
      TryWrite();
      return;
    }

    if (status_ == CallStatus::kFinish) {
      delete this;
    }
  }

 private:
  friend class WatchSessionRegistry;

  enum class CallStatus { kCreate, kAccept, kWrite, kFinish };

  void TryWrite() {
    if (finished_ || write_in_flight_) {
      return;
    }
    const auto snapshot = store_->GetSnapshot();
    if (snapshot.version() <= since_version_) {
      return;
    }
    outgoing_ = snapshot;
    write_in_flight_ = true;
    status_ = CallStatus::kWrite;
    writer_.Write(outgoing_, this);
  }

  void Finish(const grpc::Status& status) {
    if (finished_) {
      return;
    }
    finished_ = true;
    alarm_.Cancel();
    registry_->Remove(this);
    status_ = CallStatus::kFinish;
    writer_.Finish(status, this);
  }

  ConfigGrpcAsyncHandler* handler_;
  qtrade::config::v1::ConfigService::AsyncService* service_;
  grpc::ServerCompletionQueue* cq_;
  std::shared_ptr<ConfigStore> store_;
  WatchSessionRegistry* registry_;
  grpc::ServerContext ctx_;
  qtrade::config::v1::WatchConfigRequest request_;
  qtrade::config::v1::ConfigSnapshot outgoing_;
  grpc::ServerAsyncWriter<qtrade::config::v1::ConfigSnapshot> writer_;
  grpc::Alarm alarm_;
  CallStatus status_ = CallStatus::kCreate;
  std::uint64_t since_version_ = 0;
  bool accepted_ = false;
  bool write_in_flight_ = false;
  bool finished_ = false;
};

void WatchSessionRegistry::Add(WatchConfigCallData* call) {
  std::lock_guard lock(mutex_);
  sessions_.insert(call);
}

void WatchSessionRegistry::Remove(WatchConfigCallData* call) {
  std::lock_guard lock(mutex_);
  sessions_.erase(call);
}

void WatchSessionRegistry::NotifyAll() {
  std::unordered_set<WatchConfigCallData*> copy;
  {
    std::lock_guard lock(mutex_);
    copy = sessions_;
  }
  for (auto* call : copy) {
    call->SchedulePush();
  }
}

}  // namespace detail

ConfigGrpcAsyncHandler::ConfigGrpcAsyncHandler() = default;

ConfigGrpcAsyncHandler::~ConfigGrpcAsyncHandler() { Shutdown(); }

void ConfigGrpcAsyncHandler::Init(qtrade::config::v1::ConfigService::AsyncService* async_service,
                                  grpc::ServerCompletionQueue* cq,
                                  std::shared_ptr<ConfigStore> store) {
  async_service_ = async_service;
  cq_ = cq;
  store_ = std::move(store);
  watch_registry_ = std::make_unique<detail::WatchSessionRegistry>();
}

void ConfigGrpcAsyncHandler::Start() {
  if (started_ || async_service_ == nullptr || cq_ == nullptr || !store_) {
    return;
  }

  store_->SetVersionListener([this](std::uint64_t version) { OnStoreVersion(version); });

  SpawnGetConfig();
  SpawnWatchConfig();

  started_ = true;
  spdlog::info("[ConfigGrpcAsyncHandler] async RPC handlers started");
}

void ConfigGrpcAsyncHandler::Shutdown() {
  if (!started_) {
    return;
  }
  if (store_) {
    store_->SetVersionListener(nullptr);
  }
  started_ = false;
}

void ConfigGrpcAsyncHandler::SpawnGetConfig() {
  if (async_service_ == nullptr || cq_ == nullptr || !store_) {
    return;
  }
  new detail::GetConfigCallData(this, async_service_, cq_, store_);
}

void ConfigGrpcAsyncHandler::SpawnWatchConfig() {
  if (async_service_ == nullptr || cq_ == nullptr || !store_ || !watch_registry_) {
    return;
  }
  new detail::WatchConfigCallData(this, async_service_, cq_, store_, watch_registry_.get());
}

void ConfigGrpcAsyncHandler::OnStoreVersion(std::uint64_t version) {
  (void)version;
  if (watch_registry_) {
    watch_registry_->NotifyAll();
  }
}

}  // namespace qtrade::service
