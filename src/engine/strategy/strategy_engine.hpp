#ifndef QTRADE_TRADING_ENGINE_STRATEGY_ENGINE_HPP_
#define QTRADE_TRADING_ENGINE_STRATEGY_ENGINE_HPP_

#include <qtrade/error_code/error_code.hpp>
#include <qtrade/strategy/strategy.hpp>

#include "engine/event_bus/event_bus.hpp"

#include <functional>
#include <memory>
#include <mutex>
#include <vector>

namespace qtrade::engine::strategy {

using OrderSender = std::function<ErrorCode(const Order&)>;

class StrategyEngine {
 public:
  explicit StrategyEngine(event_bus::EventBus& event_bus);
  ~StrategyEngine();

  void Start();
  void Stop();

  void RegisterStrategy(std::unique_ptr<qtrade::strategy::IStrategy> strategy);
  void SetOrderSender(OrderSender sender);

 private:
  event_bus::EventBus& event_bus_;
  std::vector<std::unique_ptr<qtrade::strategy::IStrategy>> strategies_;
  std::mutex mutex_;
  bool running_;
  OrderSender order_sender_;

  void OnTickEvent(const MarketTick& tick);
  void OnBarEvent(const Bar& bar);
  void OnOrderEvent(const Order& order);
  void OnTradeEvent(const Trade& trade);
};

}  // namespace qtrade::engine::strategy

#endif  // QTRADE_TRADING_ENGINE_STRATEGY_ENGINE_HPP_
