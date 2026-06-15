/// @file      trading_engine.cpp
/// @brief     交易引擎实现
/// @details   实现交易引擎的启动、停止及各子模块协调逻辑
/// @author    wengjianhong
/// @date      2026-05-19
/// @copyright CC BY-NC-SA 4.0
#include "trading_engine.hpp"
#include <spdlog/spdlog.h>

namespace qtrade::engine {

TradingEngine::TradingEngine() : running_(false), market_handler_(event_bus_), strategy_engine_(event_bus_) {}

TradingEngine::~TradingEngine() { Stop(); }

ErrorCode TradingEngine::Start() {
  if (running_) {
    return ErrorCode::kSystemError;
  }

  spdlog::info("[TradingEngine] starting components...");

  event_bus_.Start();
  market_handler_.Start();
  strategy_engine_.Start();
  compliance_.Start();
  order_manager_.Start();
  account_manager_.Start();
  position_manager_.Start();
  risk_manager_.Start();
  execution_manager_.Start();

  running_ = true;
  spdlog::info("[TradingEngine] started successfully");
  return ErrorCode::kSuccess;
}

ErrorCode TradingEngine::Stop() {
  if (!running_) {
    return ErrorCode::kSystemError;
  }

  spdlog::info("[TradingEngine] stopping components...");

  execution_manager_.Stop();
  risk_manager_.Stop();
  position_manager_.Stop();
  account_manager_.Stop();
  order_manager_.Stop();
  compliance_.Stop();
  strategy_engine_.Stop();
  market_handler_.Stop();
  event_bus_.Stop();

  running_ = false;
  spdlog::info("[TradingEngine] stopped cleanly");
  return ErrorCode::kSuccess;
}

bool TradingEngine::IsRunning() const { return running_; }

}  // namespace qtrade::engine
