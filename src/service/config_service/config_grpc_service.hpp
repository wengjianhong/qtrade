/// @file      config_grpc_service.hpp
/// @brief     config-service gRPC 服务实现
/// @author    wengjianhong
/// @date      2026-06-22
/// @copyright CC BY-NC-SA 4.0
#ifndef QTRADE_SERVICE_CONFIG_GRPC_SERVICE_HPP_
#define QTRADE_SERVICE_CONFIG_GRPC_SERVICE_HPP_

#include "service/config_service/config_store.hpp"

#include <qtrade/config/v1/config.grpc.pb.h>

#include <memory>

namespace qtrade::service {

/// @brief ConfigService gRPC 服务端（引擎仅作 Client 出站订阅）
class ConfigGrpcService final : public qtrade::config::v1::ConfigService::Service {
 public:
  /// @brief 构造 gRPC 服务实现
  /// @param store 共享配置存储
  explicit ConfigGrpcService(std::shared_ptr<ConfigStore> store);

  /// @brief 冷启动：返回全量配置快照
  /// @param context gRPC 服务端上下文
  /// @param request GetConfig 请求（tenant_id / engine_id）
  /// @param response 输出 ConfigSnapshot
  /// @return gRPC 状态
  grpc::Status GetConfig(grpc::ServerContext* context,
                         const qtrade::config::v1::GetConfigRequest* request,
                         qtrade::config::v1::ConfigSnapshot* response) override;

  /// @brief 运行时：Server Streaming 推送配置增量
  /// @param context gRPC 服务端上下文
  /// @param request WatchConfig 请求（since_version 等）
  /// @param writer 增量流写入器
  /// @return gRPC 状态
  grpc::Status WatchConfig(grpc::ServerContext* context,
                           const qtrade::config::v1::WatchConfigRequest* request,
                           grpc::ServerWriter<qtrade::config::v1::ConfigDelta>* writer) override;

 private:
  std::shared_ptr<ConfigStore> store_;  ///< 共享配置存储
};

}  // namespace qtrade::service

#endif  // QTRADE_SERVICE_CONFIG_GRPC_SERVICE_HPP_
