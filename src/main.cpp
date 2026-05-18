/// @file main.cpp
/// @brief Main entry point for the qtrade engine
/// @author wengjianhong
/// @date 2026-05-05
/// @version 0.1.0
/// @copyright Copyright (c) 2026 qtrade
/// @note This file is part of the qtrade engine
#include <qtrade/engine/trading_engine.hpp>
#include <qtrade/adapter/market_source.hpp>
#include <demo/strategy/example_strategy.hpp>
#include <src/common/logger.hpp>

#include <spdlog/spdlog.h>

#include <cstdlib>
#include <chrono>
#include <thread>

int main(int argc, char** argv) {
  (void)argc;
  (void)argv;

  // ====================== 初始化纯文件日志 ======================
  qtrade::common::init_spdlog_logger("logs", "trading-engine.log");

  spdlog::info("==================================================");
  spdlog::info("qtrade engine starting...");
  spdlog::info("==================================================");
  spdlog::info("[main] system bootstrap start");
  spdlog::info("[main] running in dry-run mode");

  // 加载引擎配置
  qtrade::trading::engine::TradingEngineConfig cfg;
  cfg.dry_run = true;
  spdlog::info("[main] trading engine config initialized");

  // 创建交易引擎实例
  qtrade::trading::engine::TradingEngine engine(cfg);
  spdlog::info("[main] trading engine instance created successfully");

  // ====================== 设置组件 ======================
  auto& market_handler = engine.GetMarketHandler();
  auto& strategy_engine = engine.GetStrategyEngine();

  // 设置模拟行情源
  auto market_source = qtrade::trading::adapter::CreateMockMarketSource();
  market_handler.SetMarketSource(std::move(market_source));
  
  // 创建并设置示例策略
  auto strategy = qtrade::trading::demo::CreateExampleStrategy();
  
  // 初始化策略
  qtrade::trading::strategy::StrategyConfig strategy_cfg;
  strategy_cfg.name = "ExampleStrategy";
  strategy->Init(strategy_cfg);
  
  // 设置策略的订单发送器
  auto* example_strategy = static_cast<qtrade::trading::demo::ExampleStrategy*>(strategy.get());
  
  // 简单的订单发送器（直接打印日志）
  auto order_sender = [](const qtrade::trading::Order& order) {
    spdlog::info("[OrderSender] sending order: {} {} {} @ {}", 
                 order.instrument,
                 order.side == qtrade::trading::OrderSide::kBuy ? "BUY" : "SELL",
                 order.volume,
                 order.price);
    return qtrade::trading::ErrorCode::kOk;
  };
  example_strategy->SetOrderSender(order_sender);
  
  // 注册策略
  strategy_engine.RegisterStrategy(std::move(strategy));
  
  // 设置策略引擎的订单发送器
  strategy_engine.SetOrderSender(order_sender);

  // 启动引擎
  spdlog::info("[main] starting trading engine main loop...");
  const auto rc = engine.Start();

  // 启动失败处理
  if (rc != qtrade::trading::ErrorCode::kOk) {
    spdlog::error("[main] engine start failed! error code: {}", static_cast<int>(rc));
    spdlog::critical("[main] system aborting due to start failure");
    return EXIT_FAILURE;
  }

  spdlog::info("[main] trading engine started successfully");
  spdlog::info("[main] engine is running...");

  // 创建并设置模拟行情源
  {
    // 先创建行情源
    auto market_source = qtrade::trading::adapter::CreateMockMarketSource();
    
    // 设置到市场处理器（这样会自动设置回调函数）
    market_handler.SetMarketSource(std::move(market_source));
    
    // 获取设置后的行情源并连接
    auto* source_ptr = market_handler.GetMarketSource();
    if (source_ptr) {
      qtrade::trading::adapter::MarketSourceConfig source_cfg;
      source_cfg.name = "MockDataSource";
      source_cfg.connection_string = "mock://localhost";
      source_ptr->Connect(source_cfg);
    }
    
    // 订阅一些合约
    market_handler.Subscribe({"IF2401", "IC2401"});
  }

  // 让引擎运行一段时间
  spdlog::info("[main] engine running for 5 seconds...");
  std::this_thread::sleep_for(std::chrono::seconds(5));

  // 停止引擎
  spdlog::warn("[main] preparing to stop trading engine...");
  engine.Stop();

  // 退出日志
  spdlog::info("[main] trading engine stopped cleanly");
  spdlog::info("[main] system shutdown successfully");
  spdlog::info("==================================================\n");

  return EXIT_SUCCESS;
}
