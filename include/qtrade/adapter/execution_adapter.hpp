/// @file    execution_adapter.hpp
/// @brief   交易执行适配器接口
/// @details 定义交易执行适配器的抽象接口，用于对接不同券商的交易通道
/// @author  wengjianhong
/// @date    2026-05-19
/// @copyright CC BY-NC-SA 4.0
#ifndef QTRADE_TRADING_ADAPTER_EXECUTION_ADAPTER_HPP_
#define QTRADE_TRADING_ADAPTER_EXECUTION_ADAPTER_HPP_
#include <qtrade/error_code/code_define.hpp>
#include <qtrade/structs/order.hpp>

#include <functional>
#include <string>

namespace qtrade::adapter {

struct ExecutionAdapterConfig {
  std::string broker_id;
  std::string connection_string;
};

class IExecutionAdapter {
 public:
  virtual ~IExecutionAdapter() = default;

  using OrderCallback = std::function<void(const qtrade::Order&)>;
  using TradeCallback = std::function<void(const qtrade::Trade&)>;

  virtual ErrorCode Connect(const ExecutionAdapterConfig& config) = 0;
  virtual void Disconnect() = 0;

  virtual ErrorCode SendOrder(const Order& order) = 0;
  virtual ErrorCode CancelOrder(const std::string& order_id) = 0;

  virtual void SetOrderCallback(OrderCallback cb) { on_order_ = std::move(cb); }
  virtual void SetTradeCallback(TradeCallback cb) { on_trade_ = std::move(cb); }

  virtual bool IsConnected() const = 0;

 protected:
  OrderCallback on_order_;
  TradeCallback on_trade_;
};

}  // namespace qtrade::adapter

#endif  // QTRADE_TRADING_ADAPTER_EXECUTION_ADAPTER_HPP_
