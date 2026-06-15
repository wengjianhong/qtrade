/// @file      execution_adapter.hpp
/// @brief     交易执行适配器接口
/// @details   定义交易执行适配器的抽象接口，用于对接不同券商的交易通道
/// @author    wengjianhong
/// @date      2026-05-19
/// @copyright CC BY-NC-SA 4.0
#ifndef QTRADE_TRADING_ADAPTER_EXECUTION_ADAPTER_HPP_
#define QTRADE_TRADING_ADAPTER_EXECUTION_ADAPTER_HPP_

#include <qtrade/error_code/code_define.hpp>
#include <qtrade/structs/order.hpp>

#include <functional>
#include <string>

namespace qtrade::adapter {

/// @brief 执行适配器配置结构体
struct ExecutionAdapterConfig {
  std::string broker_id;          /// 券商ID
  std::string connection_string;  /// 连接字符串
};

/// @brief 交易执行适配器接口
/// @details 定义交易执行适配器的抽象接口，用于对接不同券商的交易通道
class IExecutionAdapter {
 public:
  virtual ~IExecutionAdapter() = default;

  /// @brief 订单回调函数类型
  using OrderCallback = std::function<void(const qtrade::Order&)>;
  /// @brief 成交回调函数类型
  using TradeCallback = std::function<void(const qtrade::Trade&)>;

  /// @brief 连接到交易通道
  /// @param config 适配器配置
  /// @return ErrorCode::kSuccess 表示成功，其他表示失败
  virtual ErrorCode Connect(const ExecutionAdapterConfig& config) = 0;

  /// @brief 断开连接
  virtual void Disconnect() = 0;

  /// @brief 发送订单
  /// @param order 订单信息
  /// @return ErrorCode::kSuccess 表示成功，其他表示失败
  virtual ErrorCode SendOrder(const Order& order) = 0;

  /// @brief 撤单
  /// @param order_id 订单ID
  /// @return ErrorCode::kSuccess 表示成功，其他表示失败
  virtual ErrorCode CancelOrder(const std::string& order_id) = 0;

  /// @brief 设置订单回调
  /// @param cb 回调函数
  virtual void SetOrderCallback(OrderCallback cb) { on_order_ = std::move(cb); }

  /// @brief 设置成交回调
  /// @param cb 回调函数
  virtual void SetTradeCallback(TradeCallback cb) { on_trade_ = std::move(cb); }

  /// @brief 检查连接状态
  /// @return true表示已连接，false表示未连接
  virtual bool IsConnected() const = 0;

 protected:
  OrderCallback on_order_;  /// 订单回调
  TradeCallback on_trade_;  /// 成交回调
};

}  // namespace qtrade::adapter

#endif  // QTRADE_TRADING_ADAPTER_EXECUTION_ADAPTER_HPP_
