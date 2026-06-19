/// @file      main.cpp
/// @brief     交易引擎独立进程入口（qtrade_engine）
/// @details   解析配置、初始化日志与交易引擎，挂载示例策略并运行主循环
/// @author    wengjianhong
/// @date      2026-05-19
/// @copyright CC BY-NC-SA 4.0
#include "common/app/app_runner.hpp"
#include "common/logging/logger.hpp"
#include "engine/trading_engine.hpp"
#include "strategy/example_strategy.hpp"

#include <qtrade/adapter/market_source.hpp>

#include <spdlog/spdlog.h>

#include <atomic>
#include <cstdlib>
#include <iostream>
#include <string>
#include <unistd.h>

int main(int argc, char** argv) {
  std::string config_path;
  if (!qtrade::common::ParseConfigPath(argc, argv, config_path)) {
    std::cerr << "[qtrade_engine] missing required argument: --config <path>\n";
    return EXIT_FAILURE;
  }

  if (!qtrade::common::init_spdlog_logger("logs", "trading-engine.log")) {
    return EXIT_FAILURE;
  }

  spdlog::info("==================================================");
  spdlog::info("qtrade_engine starting (pid={})", getpid());
  spdlog::info("config: {}", config_path);
  spdlog::info("==================================================");

  std::atomic<bool> stop_flag{false};
  qtrade::common::InstallShutdownHandler(stop_flag);

  qtrade::engine::TradingEngine engine;
  auto& market_handler = engine.GetMarketHandler();
  auto& strategy_engine = engine.GetStrategyEngine();

  auto market_source = qtrade::adapter::CreateMockMarketSource();
  market_handler.SetMarketSource(std::move(market_source));

  auto strategy = qtrade::demo::CreateExampleStrategy();
  qtrade::strategy::StrategyConfig strategy_cfg;
  strategy_cfg.name = "ExampleStrategy";
  strategy->Init(strategy_cfg);

  auto* example_strategy = static_cast<qtrade::demo::ExampleStrategy*>(strategy.get());
  auto order_sender = [](const qtrade::OrderRequest& request) {
    spdlog::info("[OrderSender] {} {} {} @ {}",
                 request.instrument,
                 request.side == qtrade::SideType::kBuy ? "BUY" : "SELL",
                 request.volume,
                 request.price);
    return qtrade::ErrorCode::kSuccess;
  };
  example_strategy->SetOrderSender(order_sender);
  strategy_engine.RegisterStrategy(std::move(strategy));
  strategy_engine.SetOrderSender(order_sender);

  if (const auto rc = engine.Start(); rc != qtrade::ErrorCode::kSuccess) {
    spdlog::error("[qtrade_engine] start failed, code={}", static_cast<int>(rc));
    return EXIT_FAILURE;
  }

  if (auto* source_ptr = market_handler.GetMarketSource()) {
    qtrade_sdk::quote::ConnectRequest source_cfg;
    source_cfg.name = "MockDataSource";
    source_cfg.connection_string = "mock://localhost";
    source_ptr->Connect(source_cfg);
    market_handler.Subscribe({"IF2401", "IC2401"});
  }

  qtrade::common::UnblockShutdownSignals();

  spdlog::info("[qtrade_engine] running until SIGINT/SIGTERM...");

  qtrade::common::RunUntilStop(stop_flag);

  engine.Stop();
  spdlog::info("[qtrade_engine] stopped cleanly");
  return EXIT_SUCCESS;
}
