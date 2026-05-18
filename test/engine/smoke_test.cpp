#include <gtest/gtest.h>

#include <qtrade/structs/tick_data.hpp>
#include <qtrade/engine/trading_engine.hpp>

TEST(EngineSmoke, TradingEngineStartStop) {
  qtrade::trading::engine::TradingEngineConfig cfg;
  qtrade::trading::engine::TradingEngine engine(cfg);
  ASSERT_EQ(engine.Start(), qtrade::trading::ErrorCode::kOk);
  ASSERT_TRUE(engine.IsRunning());
  engine.Stop();
  ASSERT_FALSE(engine.IsRunning());
}

TEST(EngineSmoke, MarketTickSize) {
  qtrade::trading::MarketTick tick;
  (void)tick;
  SUCCEED();
}
