#ifndef QUANT_TRADE_DEMO_EXAMPLE_STRATEGY_HPP
#define QUANT_TRADE_DEMO_EXAMPLE_STRATEGY_HPP

#include <qtrade/strategy/strategy.hpp>
#include <memory>
#include <functional>

namespace quant::trading::demo {

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

}  // namespace quant::trading::demo

#endif  // QUANT_TRADE_DEMO_EXAMPLE_STRATEGY_HPP
