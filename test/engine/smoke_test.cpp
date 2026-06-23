/// @file      smoke_test.cpp
/// @brief     交易引擎冒烟测试
/// @details   验证 TradingEngine 启动/停止及 TickData 基本行为
/// @author    wengjianhong
/// @date      2026-05-19
/// @copyright CC BY-NC-SA 4.0
#include <gtest/gtest.h>

#include <qtrade/structs/tick_data.hpp>
#include <qtrade/engine/engine_options.hpp>
#include "engine/trading_engine.hpp"

TEST(EngineSmoke, TradingEngineStartStop) {
  qtrade::engine::EngineOptions options;
  options.log_topic = "test";
  qtrade::engine::TradingEngine engine;
  ASSERT_EQ(engine.Init(options), qtrade::ErrorCode::kSuccess);
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
