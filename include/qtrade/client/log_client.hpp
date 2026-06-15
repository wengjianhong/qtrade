/// @file      log_client.hpp
/// @brief     日志客户端
/// @details   提供日志发送功能，用于向日志服务发送结构化日志
/// @author    wengjianhong
/// @date      2026-05-19
/// @copyright CC BY-NC-SA 4.0
#ifndef QTRADE_TRADING_CLIENT_LOG_CLIENT_HPP_
#define QTRADE_TRADING_CLIENT_LOG_CLIENT_HPP_

#include <qtrade/error_code/code_define.hpp>

#include <string_view>

namespace qtrade::client {

/// @brief 日志客户端类
/// @details 封装日志发送功能，支持多种日志级别
class LogClient {
 public:
  LogClient() = default;
  ~LogClient() = default;

  /// @brief 初始化日志客户端
  /// @param topic 日志主题/分类
  /// @return ErrorCode::kSuccess 表示成功，其他表示失败
  ErrorCode Init(std::string_view topic);

  /// @brief 关闭日志客户端
  void Shutdown();

  /// @brief 发送日志消息
  /// @param level 日志级别（error/warn/info/debug等）
  /// @param message 日志消息内容
  void Emit(std::string_view level, std::string_view message);

 private:
  bool initialized_ = false;  /// 初始化标志
};

}  // namespace qtrade::client

#endif  // QTRADE_TRADING_CLIENT_LOG_CLIENT_HPP_
