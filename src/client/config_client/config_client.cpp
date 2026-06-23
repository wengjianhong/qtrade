/// @file      config_client.cpp
/// @brief     配置管理客户端实现（gRPC 出站）
/// @author    wengjianhong
/// @date      2026-05-19
/// @copyright CC BY-NC-SA 4.0
#include <qtrade/client/config_client.hpp>

#include <qtrade/config/v1/config.grpc.pb.h>

#include <grpcpp/grpcpp.h>
#include <spdlog/spdlog.h>

#include <atomic>
#include <chrono>
#include <thread>
#include <utility>

namespace qtrade::client {

struct ConfigClient::Impl {
  ConfigClientOptions options;                                    ///< 连接参数
  UpdateHandler on_update;                                        ///< 配置 upsert 回调
  DeleteHandler on_delete;                                        ///< 配置 delete 回调
  std::shared_ptr<grpc::Channel> channel;                         ///< gRPC 通道
  std::unique_ptr<qtrade::config::v1::ConfigService::Stub> stub;  ///< gRPC 存根
  std::thread watch_thread;                                       ///< WatchConfig 控制线程
  std::atomic<bool> watch_running{false};                         ///< Watch 线程运行标志
  std::atomic<std::uint64_t> version{0};                          ///< 已应用配置版本
  bool initialized = false;                                       ///< 是否已完成 Init
};

ConfigClient::ConfigClient() : impl_(std::make_unique<Impl>()) {}

ConfigClient::~ConfigClient() { Shutdown(); }

ErrorCode ConfigClient::Init(const ConfigClientOptions& options) {
  if (impl_->initialized) {
    return ErrorCode::kSystemError;
  }
  if (options.server_address.empty()) {
    return ErrorCode::kInternal;
  }

  impl_->options = options;
  impl_->channel = grpc::CreateChannel(options.server_address, grpc::InsecureChannelCredentials());
  impl_->stub = qtrade::config::v1::ConfigService::NewStub(impl_->channel);
  impl_->initialized = true;
  return ErrorCode::kSuccess;
}

ErrorCode ConfigClient::FetchSnapshot() {
  if (!impl_->initialized || !impl_->stub) {
    return ErrorCode::kNotInitialized;
  }

  qtrade::config::v1::GetConfigRequest request;
  request.set_tenant_id(impl_->options.tenant_id);
  request.set_engine_id(impl_->options.engine_id);

  qtrade::config::v1::ConfigSnapshot response;
  grpc::ClientContext context;
  context.set_deadline(std::chrono::system_clock::now() + std::chrono::seconds(5));

  const grpc::Status status = impl_->stub->GetConfig(&context, request, &response);
  if (!status.ok()) {
    spdlog::warn("[ConfigClient] GetConfig failed: {}", status.error_message());
    return ErrorCode::kTimeout;
  }

  impl_->version.store(response.version(), std::memory_order_release);
  if (impl_->on_update) {
    for (const auto& entry : response.entries()) {
      impl_->on_update(entry.key(), entry.value());
    }
  }

  spdlog::info("[ConfigClient] snapshot loaded, version={}, entries={}", response.version(), response.entries_size());
  return ErrorCode::kSuccess;
}

ErrorCode ConfigClient::StartWatch() {
  if (!impl_->initialized) {
    return ErrorCode::kNotInitialized;
  }
  if (impl_->watch_running.load(std::memory_order_acquire)) {
    return ErrorCode::kSystemError;
  }

  impl_->watch_running.store(true, std::memory_order_release);
  impl_->watch_thread = std::thread([this] {
    int backoff_ms = 500;
    constexpr int kMaxBackoffMs = 30'000;

    while (impl_->watch_running.load(std::memory_order_acquire)) {
      if (!impl_->stub) {
        std::this_thread::sleep_for(std::chrono::milliseconds(backoff_ms));
        continue;
      }

      qtrade::config::v1::WatchConfigRequest request;
      request.set_tenant_id(impl_->options.tenant_id);
      request.set_engine_id(impl_->options.engine_id);
      request.set_since_version(impl_->version.load(std::memory_order_acquire));

      grpc::ClientContext context;
      qtrade::config::v1::ConfigDelta delta;
      std::unique_ptr<grpc::ClientReader<qtrade::config::v1::ConfigDelta>> reader(
        impl_->stub->WatchConfig(&context, request));

      while (impl_->watch_running.load(std::memory_order_acquire) && reader->Read(&delta)) {
        backoff_ms = 500;
        impl_->version.store(delta.version(), std::memory_order_release);

        if (impl_->on_update) {
          for (const auto& entry : delta.upserts()) {
            impl_->on_update(entry.key(), entry.value());
          }
        }
        if (impl_->on_delete) {
          for (const auto& key : delta.deletes()) {
            impl_->on_delete(key);
          }
        }

        spdlog::debug("[ConfigClient] applied delta version={}", delta.version());
      }

      const grpc::Status status = reader->Finish();
      if (!impl_->watch_running.load(std::memory_order_acquire)) {
        break;
      }

      if (!status.ok() && status.error_code() != grpc::StatusCode::CANCELLED) {
        spdlog::warn("[ConfigClient] WatchConfig disconnected: {}", status.error_message());
      }

      if (!status.ok() && impl_->watch_running.load(std::memory_order_acquire)) {
        spdlog::info("[ConfigClient] reconnecting in {} ms...", backoff_ms);
        std::this_thread::sleep_for(std::chrono::milliseconds(backoff_ms));
        backoff_ms = std::min(backoff_ms * 2, kMaxBackoffMs);
      }
    }
  });

  return ErrorCode::kSuccess;
}

void ConfigClient::Shutdown() {
  impl_->watch_running.store(false, std::memory_order_release);
  if (impl_->watch_thread.joinable()) {
    impl_->watch_thread.join();
  }
  impl_->stub.reset();
  impl_->channel.reset();
  impl_->initialized = false;
}

void ConfigClient::SetOnUpdate(UpdateHandler handler) { impl_->on_update = std::move(handler); }

void ConfigClient::SetOnDelete(DeleteHandler handler) { impl_->on_delete = std::move(handler); }

std::uint64_t ConfigClient::Version() const { return impl_->version.load(std::memory_order_acquire); }

bool ConfigClient::IsInitialized() const { return impl_->initialized; }

}  // namespace qtrade::client
