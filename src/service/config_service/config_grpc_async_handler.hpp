/// @file      config_grpc_async_handler.hpp
/// @brief     ConfigService Async + CQ RPC 处理器
/// @author    wengjianhong
/// @date      2026-06-28
/// @copyright CC BY-NC-SA 4.0
#ifndef QTRADE_SERVICE_CONFIG_GRPC_ASYNC_HANDLER_HPP_
#define QTRADE_SERVICE_CONFIG_GRPC_ASYNC_HANDLER_HPP_

#include "service/config_service/config_store.hpp"

#include <qtrade/config/v1/config.grpc.pb.h>

#include <memory>

namespace grpc {
class ServerCompletionQueue;
}

namespace qtrade::service {

namespace detail {
class WatchSessionRegistry;
}

/// @brief 管理 GetConfig / WatchConfig 的 Async CallData 生命周期
class ConfigGrpcAsyncHandler {
 public:
  ConfigGrpcAsyncHandler();
  ~ConfigGrpcAsyncHandler();

  ConfigGrpcAsyncHandler(const ConfigGrpcAsyncHandler&) = delete;
  ConfigGrpcAsyncHandler& operator=(const ConfigGrpcAsyncHandler&) = delete;

  /// @brief 绑定 AsyncService、CQ 与配置存储
  void Init(qtrade::config::v1::ConfigService::AsyncService* async_service,
            grpc::ServerCompletionQueue* cq, std::shared_ptr<ConfigStore> store);

  /// @brief 注册版本监听并预投递 RPC 接收
  void Start();

  /// @brief 停止新推送（Shutdown 前调用）
  void Shutdown();

  void SpawnGetConfig();
  void SpawnWatchConfig();

  void OnStoreVersion(std::uint64_t version);

  [[nodiscard]] std::shared_ptr<ConfigStore> Store() const { return store_; }
  [[nodiscard]] grpc::ServerCompletionQueue* CompletionQueue() const { return cq_; }
  [[nodiscard]] qtrade::config::v1::ConfigService::AsyncService* AsyncService() const {
    return async_service_;
  }

 private:
  qtrade::config::v1::ConfigService::AsyncService* async_service_ = nullptr;
  grpc::ServerCompletionQueue* cq_ = nullptr;
  std::shared_ptr<ConfigStore> store_;
  std::unique_ptr<detail::WatchSessionRegistry> watch_registry_;
  bool started_ = false;
};

}  // namespace qtrade::service

#endif  // QTRADE_SERVICE_CONFIG_GRPC_ASYNC_HANDLER_HPP_
