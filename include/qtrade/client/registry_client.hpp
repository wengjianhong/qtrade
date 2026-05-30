/// @file      registry_client.hpp
/// @brief     服务注册客户端
/// @details   提供服务注册与发现功能，对接 etcd 服务注册中心
/// @author    wengjianhong
/// @date      2026-05-19
/// @copyright CC BY-NC-SA 4.0
#ifndef QTRADE_TRADING_CLIENT_REGISTRY_CLIENT_HPP_
#define QTRADE_TRADING_CLIENT_REGISTRY_CLIENT_HPP_

#include <string>
#include <string_view>

#include <qtrade/error_code/code_define.hpp>

namespace qtrade::client {

class RegistryClient {
 public:
  ErrorCode Init(std::string_view etcd_endpoints);
  void Shutdown();

  ErrorCode Register(std::string_view service_name, std::string_view instance_id, std::string_view address);

 private:
  bool initialized_{false};
};

}  // namespace qtrade::client

#endif  // QTRADE_TRADING_CLIENT_REGISTRY_CLIENT_HPP_
