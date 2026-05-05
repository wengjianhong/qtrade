#pragma once

#include "engine/account_manager.h"
#include "engine/compliance_manager.h"
#include "engine/event_bus.h"
#include "engine/execution_manager.h"
#include "engine/market_handler.h"
#include "engine/order_manager.h"
#include "engine/position_manager.h"
#include "engine/risk_manager.h"
#include "engine/strategy_engine.h"

#include "common/error_code.h"

namespace quant::trading::engine {

struct TradingEngineConfig {
  bool dry_run{true};
};

class TradingEngine {
 public:
  explicit TradingEngine(TradingEngineConfig config);
  ~TradingEngine();

  TradingEngine(const TradingEngine&) = delete;
  TradingEngine& operator=(const TradingEngine&) = delete;

  ErrorCode Start();
  void Stop();
  bool IsRunning() const;

 private:
  TradingEngineConfig config_;
  bool running_{false};

  event_bus::EventBus event_bus_;
  market::MarketHandler market_handler_;
  strategy::StrategyEngine strategy_engine_;
  cms::ComplianceManager compliance_;
  ems::ExecutionManager execution_manager_;
  oms::OrderManager order_manager_;
  account::AccountManager account_manager_;
  position::PositionManager position_manager_;
  risk::RiskManager risk_manager_;
};

}  // namespace quant::trading::engine
