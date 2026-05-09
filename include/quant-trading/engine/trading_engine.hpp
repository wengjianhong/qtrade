#ifndef QUANT_TRADING_ENGINE_TRADING_ENGINE_HPP_
#define QUANT_TRADING_ENGINE_TRADING_ENGINE_HPP_

#include <quant-trading/structs/error_code.hpp>
#include <quant-trading/engine/account_manager.hpp>
#include <quant-trading/engine/compliance_manager.hpp>
#include <quant-trading/engine/event_bus.hpp>
#include <quant-trading/engine/execution_manager.hpp>
#include <quant-trading/engine/market_handler.hpp>
#include <quant-trading/engine/order_manager.hpp>
#include <quant-trading/engine/position_manager.hpp>
#include <quant-trading/engine/risk_manager.hpp>
#include <quant-trading/engine/strategy_engine.hpp>

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

#endif  // QUANT_TRADING_ENGINE_TRADING_ENGINE_HPP_
