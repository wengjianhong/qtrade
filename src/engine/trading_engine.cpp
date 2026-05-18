#include <qtrade/engine/trading_engine.hpp>

namespace quant::trading::engine {

TradingEngine::TradingEngine(TradingEngineConfig config) : config_(config) {}

TradingEngine::~TradingEngine() { Stop(); }

ErrorCode TradingEngine::Start() {
  if (running_) {
    return ErrorCode::kInvalidArgument;
  }

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
  return ErrorCode::kOk;
}

void TradingEngine::Stop() {
  if (!running_) {
    return;
  }

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
}

bool TradingEngine::IsRunning() const { return running_; }

}  // namespace quant::trading::engine
