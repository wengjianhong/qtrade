#ifndef QUANT_TRADING_ENGINE_STRATEGY_ENGINE_HPP
#define QUANT_TRADING_ENGINE_STRATEGY_ENGINE_HPP

#include <qtrade/strategy/strategy.hpp>
#include <qtrade/engine/event_bus.hpp>
#include <memory>
#include <mutex>
#include <vector>

namespace quant::trading::engine::strategy {

// 订单发送回调类型
using OrderSender = std::function<ErrorCode(const Order&)>;

class StrategyEngine {
 public:
  explicit StrategyEngine(event_bus::EventBus& event_bus);
  ~StrategyEngine();

  void Start();
  void Stop();
  
  // 注册策略
  void RegisterStrategy(std::unique_ptr<quant::trading::strategy::IStrategy> strategy);
  
  // 设置订单发送器
  void SetOrderSender(OrderSender sender);

 private:
  event_bus::EventBus& event_bus_;
  std::vector<std::unique_ptr<quant::trading::strategy::IStrategy>> strategies_;
  std::mutex mutex_;
  bool running_;
  OrderSender order_sender_;
  
  // 事件处理
  void OnTickEvent(const MarketTick& tick);
  void OnBarEvent(const Bar& bar);
  void OnOrderEvent(const Order& order);
  void OnTradeEvent(const Trade& trade);
};

}  // namespace quant::trading::engine::strategy

#endif  // QUANT_TRADING_ENGINE_STRATEGY_ENGINE_HPP_
