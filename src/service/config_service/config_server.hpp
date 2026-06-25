/// @file      config_server.hpp
/// @brief     config-service gRPC 进程封装
/// @author    wengjianhong
/// @date      2026-06-22
/// @copyright CC BY-NC-SA 4.0
#ifndef QTRADE_SERVICE_CONFIG_SERVER_HPP_
#define QTRADE_SERVICE_CONFIG_SERVER_HPP_

#include "service/config_service/config_store.hpp"

#include <qtrade/error_code/error_codes.hpp>

#include <memory>
#include <string>

namespace grpc {
class Server;
}

namespace qtrade::service {

class ConfigGrpcService;

/// @brief 启动/停止 config-service gRPC 监听
class ConfigServer {
 public:
  /// @brief 构造 ConfigServer（未启动）
  ConfigServer();

  /// @brief 析构并 Shutdown
  ~ConfigServer();

  ConfigServer(const ConfigServer&) = delete;
  ConfigServer& operator=(const ConfigServer&) = delete;

  /// @brief 启动 gRPC 监听
  /// @param listen_address 监听地址，如 0.0.0.0:50051
  /// @param store 共享配置存储
  /// @return ErrorCode::kSuccess 表示成功
  ErrorCode Start(const std::string& listen_address, std::shared_ptr<ConfigStore> store);

  /// @brief 优雅停止 gRPC 服务
  void Shutdown();

  /// @brief 阻塞直至服务停止
  void Wait();

  /// @brief 服务是否正在运行
  [[nodiscard]] bool IsRunning() const { return running_; }

 private:
  std::unique_ptr<grpc::Server> server_;        ///< gRPC 服务器实例
  std::shared_ptr<ConfigStore> store_;          ///< 配置存储
  std::unique_ptr<ConfigGrpcService> service_;  ///< gRPC 服务实现
  bool running_ = false;                        ///< 是否已 Start
};

/// @brief 从 JSON 配置文件加载 entries 到 ConfigStore
/// @param json_path 配置文件路径（含 grpc.listen 与 config.entries）
/// @param store 输出：加载后的 ConfigStore
/// @return ErrorCode::kSuccess 表示成功
ErrorCode LoadConfigStoreFromJson(const std::string& json_path, ConfigStore& store);

}  // namespace qtrade::service

#endif  // QTRADE_SERVICE_CONFIG_SERVER_HPP_
