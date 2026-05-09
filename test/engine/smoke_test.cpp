#include <gtest/gtest.h>

#include <quant-trading/structs/tick_data.hpp>
#include <quant-trading/engine/trading_engine.hpp>

TEST(EngineSmoke, TradingEngineStartStop) {
  quant::trading::engine::TradingEngineConfig cfg;
  quant::trading::engine::TradingEngine engine(cfg);
  ASSERT_EQ(engine.Start(), quant::trading::ErrorCode::kOk);
  ASSERT_TRUE(engine.IsRunning());
  engine.Stop();
  ASSERT_FALSE(engine.IsRunning());
}

TEST(EngineSmoke, MarketTickSize) {
  quant::trading::MarketTick tick;
  (void)tick;
  SUCCEED();
}
