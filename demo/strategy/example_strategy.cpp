#include <qtrade/strategy/strategy.hpp>

namespace quant::trading::demo {

class ExampleStrategy final : public strategy::IStrategy {
 public:
  ErrorCode Init(const strategy::StrategyConfig& config) override {
    (void)config;
    return ErrorCode::kOk;
  }

  ErrorCode Start() override { return ErrorCode::kOk; }

  void Pause() override {}

  void Resume() override {}

  void Stop() override {}

  void OnTick(const MarketTick& tick) override { (void)tick; }

  void OnBar(const Bar& bar) override { (void)bar; }

  void OnOrder(const Order& order) override { (void)order; }

  void OnTrade(const Trade& trade) override { (void)trade; }

  ErrorCode SendSignal(const strategy::Signal& signal) override {
    (void)signal;
    return ErrorCode::kOk;
  }

  std::string GetParameter(const std::string& key) const override {
    (void)key;
    return {};
  }

  ErrorCode SetParameter(const std::string& key, const std::string& value) override {
    (void)key;
    (void)value;
    return ErrorCode::kOk;
  }
};

void LinkExampleStrategyForBuild() {
  ExampleStrategy s;
  (void)s;
}

}  // namespace quant::trading::demo
