/// @file      example_strategy.hpp
/// @brief     示例策略
/// @details   实现一个简单的趋势跟踪策略作为示例
/// @author    wengjianhong
/// @date      2026-05-19
/// @copyright CC BY-NC-SA 4.0
#ifndef QTRADE_TRADE_DEMO_EXAMPLE_STRATEGY_HPP
#define QTRADE_TRADE_DEMO_EXAMPLE_STRATEGY_HPP
#include <qtrade/strategy/strategy.hpp>

#include <functional>
#include <memory>

namespace qtrade::demo {

// 简单的趋势跟踪策略
class ExampleStrategy final : public strategy::IStrategy {
 public:
  using OrderSender = std::function<ErrorCode(const Order&)>;

  ExampleStrategy();
  ~ExampleStrategy() override;

  ErrorCode Init(const strategy::StrategyConfig& config) override;
  ErrorCode Start() override;
  void Pause() override;
  void Resume() override;
  void Stop() override;

  void OnTick(const MarketTick& tick) override;
  void OnBar(const Bar& bar) override;
  void OnOrder(const Order& order) override;
  void OnTrade(const Trade& trade) override;

  ErrorCode SendSignal(const strategy::Signal& signal) override;
  std::string GetParameter(const std::string& key) const override;
  ErrorCode SetParameter(const std::string& key, const std::string& value) override;

  // 设置订单发送器
  void SetOrderSender(OrderSender sender);

 private:
  OrderSender order_sender_;
  bool running_;
  double last_price_;
  int position_;  // -1 short, 0 flat, 1 long
};

// 工厂函数
std::unique_ptr<strategy::IStrategy> CreateExampleStrategy();

}  // namespace qtrade::demo

#endif  // QTRADE_TRADE_DEMO_EXAMPLE_STRATEGY_HPP
