/// @file    log_client.hpp
/// @brief   日志客户端
/// @details 提供日志发送功能，用于向日志服务发送结构化日志
/// @author  wengjianhong
/// @date    2026-05-19
/// @copyright CC BY-NC-SA 4.0
#ifndef QTRADE_TRADING_CLIENT_LOG_CLIENT_HPP_
#define QTRADE_TRADING_CLIENT_LOG_CLIENT_HPP_

#include <string_view>

#include <qtrade/error_code/code_define.hpp>

namespace qtrade::client {

class LogClient {
 public:
  LogClient() = default;
  ~LogClient() = default;

  ErrorCode Init(std::string_view topic);
  void Shutdown();

  void Emit(std::string_view level, std::string_view message);

 private:
  bool initialized_{false};
};

}  // namespace qtrade::client

#endif  // QTRADE_TRADING_CLIENT_LOG_CLIENT_HPP_
