/// @file      monitor_client.hpp
/// @brief     监控指标客户端
/// @details   提供监控指标上报功能，支持 Counter、Gauge 等指标类型
/// @author    wengjianhong
/// @date      2026-05-19
/// @copyright CC BY-NC-SA 4.0
#ifndef QTRADE_TRADING_CLIENT_MONITOR_CLIENT_HPP_
#define QTRADE_TRADING_CLIENT_MONITOR_CLIENT_HPP_

#include <qtrade/error_code/code_define.hpp>

#include <string_view>

namespace qtrade::client {

/// @brief 监控指标客户端类
/// @details 封装监控指标上报功能，支持计数器和仪表盘类型
class MonitorClient {
 public:
  /// @brief 初始化监控客户端
  /// @param endpoint 监控服务端点地址
  /// @return ErrorCode::kSuccess 表示成功，其他表示失败
  ErrorCode Init(std::string_view endpoint);

  /// @brief 关闭监控客户端
  void Shutdown();

  /// @brief 上报计数器指标
  /// @param name 指标名称
  /// @param value 增量值
  void Counter(std::string_view name, double value);

  /// @brief 上报仪表盘指标
  /// @param name 指标名称
  /// @param value 当前值
  void Gauge(std::string_view name, double value);

 private:
  bool initialized_ = false;  /// 初始化标志
};

}  // namespace qtrade::client

#endif  // QTRADE_TRADING_CLIENT_MONITOR_CLIENT_HPP_
