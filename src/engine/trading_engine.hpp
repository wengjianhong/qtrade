/// @file      trading_engine.hpp
/// @brief     交易引擎主入口
/// @details   整合各个子模块（账户、风控、订单、持仓等），协调整个交易流程
/// @author    wengjianhong
/// @date      2026-05-19
/// @copyright CC BY-NC-SA 4.0
#ifndef QTRADE_TRADING_ENGINE_TRADING_ENGINE_HPP_
#define QTRADE_TRADING_ENGINE_TRADING_ENGINE_HPP_
#include "engine/account/account_manager.hpp"
#include "engine/cms/compliance_manager.hpp"
#include "engine/ems/execution_manager.hpp"
#include "engine/event_bus/event_bus.hpp"
#include "engine/market/market_handler.hpp"
#include "engine/oms/order_manager.hpp"
#include "engine/position/position_manager.hpp"
#include "engine/risk/risk_manager.hpp"
#include "engine/strategy/strategy_engine.hpp"
#include <qtrade/error_code/code_define.hpp>

namespace qtrade::engine {

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

  event_bus::EventBus& GetEventBus() { return event_bus_; }
  market::MarketHandler& GetMarketHandler() { return market_handler_; }
  strategy::StrategyEngine& GetStrategyEngine() { return strategy_engine_; }

 private:
  TradingEngineConfig config_;
  bool running_;

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

}  // namespace qtrade::engine

#endif  // QTRADE_TRADING_ENGINE_TRADING_ENGINE_HPP_
