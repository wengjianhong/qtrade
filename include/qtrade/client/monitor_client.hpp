/// @file      monitor_client.hpp
/// @brief     监控指标客户端
/// @details   提供监控指标上报功能，支持 Counter、Gauge 等指标类型
/// @author    wengjianhong
/// @date      2026-05-19
/// @copyright CC BY-NC-SA 4.0
#ifndef QTRADE_TRADING_CLIENT_MONITOR_CLIENT_HPP_
#define QTRADE_TRADING_CLIENT_MONITOR_CLIENT_HPP_

#include <string_view>

#include <qtrade/error_code/code_define.hpp>

namespace qtrade::client {

class MonitorClient {
 public:
  ErrorCode Init(std::string_view endpoint);
  void Shutdown();

  void Counter(std::string_view name, double value);
  void Gauge(std::string_view name, double value);

 private:
  bool initialized_{false};
};

}  // namespace qtrade::client

#endif  // QTRADE_TRADING_CLIENT_MONITOR_CLIENT_HPP_
