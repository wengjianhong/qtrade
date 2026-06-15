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

class TradingEngine {
 public:
  TradingEngine();
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
  bool running_;                                /// 运行状态标志
  event_bus::EventBus event_bus_;               /// 事件总线，用于各个模块之间的通信
  strategy::StrategyEngine strategy_engine_;    /// 策略引擎，用于处理策略逻辑
  market::MarketHandler market_handler_;        /// 行情处理器，用于处理行情数据
  cms::ComplianceManager compliance_;           /// 合规管理器，用于处理合规逻辑
  ems::ExecutionManager execution_manager_;     /// 执行管理器，用于执行交易指令
  oms::OrderManager order_manager_;             /// 订单管理器，用于管理订单
  account::AccountManager account_manager_;     /// 账户管理器，用于管理账户
  position::PositionManager position_manager_;  /// 持仓管理器，用于管理持仓
  risk::RiskManager risk_manager_;              /// 风险管理器，用于管理风险
};

}  // namespace qtrade::engine

#endif  // QTRADE_TRADING_ENGINE_TRADING_ENGINE_HPP_
