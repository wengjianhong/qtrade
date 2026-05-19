#include <gtest/gtest.h>

#include <qtrade/structs/tick_data.hpp>
#include "engine/trading_engine.hpp"

TEST(EngineSmoke, TradingEngineStartStop) {
  qtrade::engine::TradingEngineConfig cfg;
  qtrade::engine::TradingEngine engine(cfg);
  ASSERT_EQ(engine.Start(), qtrade::ErrorCode::kSuccess);
  ASSERT_TRUE(engine.IsRunning());
  engine.Stop();
  ASSERT_FALSE(engine.IsRunning());
}

TEST(EngineSmoke, MarketTickSize) {
  qtrade::MarketTick tick;
  (void)tick;
  SUCCEED();
}
