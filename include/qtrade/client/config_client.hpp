/// @file    config_client.hpp
/// @brief   配置管理客户端
/// @details 提供配置订阅功能，支持配置变更的实时推送
/// @author  wengjianhong
/// @date    2026-05-19
/// @copyright CC BY-NC-SA 4.0
#ifndef QTRADE_TRADING_CLIENT_CONFIG_CLIENT_HPP_
#define QTRADE_TRADING_CLIENT_CONFIG_CLIENT_HPP_

#include <functional>
#include <string>
#include <string_view>

#include <qtrade/error_code/code_define.hpp>

namespace qtrade::client {

class ConfigClient {
 public:
  using UpdateHandler = std::function<void(std::string_view key, std::string_view value)>;

  ErrorCode Init(std::string_view bootstrap_address);
  void Shutdown();

  void SetOnUpdate(UpdateHandler handler);

 private:
  UpdateHandler on_update_;
  bool initialized_{false};
};

}  // namespace qtrade::client

#endif  // QTRADE_TRADING_CLIENT_CONFIG_CLIENT_HPP_
