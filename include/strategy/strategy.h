#pragma once

#include <string>

#include "common/bar_data.h"
#include "common/error_code.h"
#include "common/order.h"
#include "common/tick_data.h"

namespace quant::trading::strategy {

struct StrategyConfig {
  std::string name;
  std::string parameter_blob;
};

struct Signal {
  std::string instrument;
  int direction{};  // -1 short, 0 flat, 1 long（示例占位）
  double strength{};
};

class IStrategy {
 public:
  virtual ~IStrategy() = default;

  virtual ErrorCode Init(const StrategyConfig& config) = 0;
  virtual ErrorCode Start() = 0;
  virtual void Pause() = 0;
  virtual void Resume() = 0;
  virtual void Stop() = 0;

  virtual void OnTick(const MarketTick& tick) = 0;
  virtual void OnBar(const Bar& bar) = 0;
  virtual void OnOrder(const Order& order) = 0;
  virtual void OnTrade(const Trade& trade) = 0;

  virtual ErrorCode SendSignal(const Signal& signal) = 0;
  virtual std::string GetParameter(const std::string& key) const = 0;
  virtual ErrorCode SetParameter(const std::string& key, const std::string& value) = 0;
};

}  // namespace quant::trading::strategy
