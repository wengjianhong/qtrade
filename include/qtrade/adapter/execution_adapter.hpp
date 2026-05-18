#ifndef QUANT_TRADING_ADAPTER_EXECUTION_ADAPTER_HPP_
#define QUANT_TRADING_ADAPTER_EXECUTION_ADAPTER_HPP_

#include <functional>
#include <string>
#include <vector>

#include <qtrade/structs/error_code.hpp>
#include <qtrade/structs/order.hpp>

namespace quant::trading::adapter {

struct ExecutionAdapterConfig {
  std::string broker_id;
  std::string connection_string;
};

class IExecutionAdapter {
 public:
  virtual ~IExecutionAdapter() = default;

  using OrderCallback = std::function<void(const Order&)>;
  using TradeCallback = std::function<void(const Trade&)>;

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

}  // namespace quant::trading::adapter

#endif  // QUANT_TRADING_ADAPTER_EXECUTION_ADAPTER_HPP_
