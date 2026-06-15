/// @file      main.cpp
/// @brief     交易引擎独立进程入口（qtrade_engine）
#include "common/app_runner.hpp"
#include "common/logger.hpp"
#include "engine/trading_engine.hpp"
#include "strategy/example_strategy.hpp"

#include <qtrade/adapter/market_source.hpp>

#include <spdlog/spdlog.h>

#include <atomic>
#include <chrono>
#include <cstdlib>
#include <string>
#include <unistd.h>

namespace {

bool ParseDryRun(int argc, char** argv) {
  for (int i = 1; i < argc; ++i) {
    if (std::string(argv[i]) == "--live") {
      return false;
    }
  }
  return true;
}

}  // namespace

int main(int argc, char** argv) {
  int demo_seconds = 0;
  qtrade::common::ParseDemoSeconds(argc, argv, demo_seconds);

  const bool dry_run = ParseDryRun(argc, argv);

  if (!qtrade::common::init_spdlog_logger("logs", "trading-engine.log")) {
    return EXIT_FAILURE;
  }

  spdlog::info("==================================================");
  spdlog::info("qtrade_engine starting (pid={})", getpid());
  spdlog::info("mode: {}", dry_run ? "dry-run" : "live");
  spdlog::info("==================================================");

  qtrade::engine::TradingEngineConfig cfg;
  cfg.dry_run = dry_run;

  qtrade::engine::TradingEngine engine(cfg);
  auto& market_handler = engine.GetMarketHandler();
  auto& strategy_engine = engine.GetStrategyEngine();

  auto market_source = qtrade::adapter::CreateMockMarketSource();
  market_handler.SetMarketSource(std::move(market_source));

  auto strategy = qtrade::demo::CreateExampleStrategy();
  qtrade::strategy::StrategyConfig strategy_cfg;
  strategy_cfg.name = "ExampleStrategy";
  strategy->Init(strategy_cfg);

  auto* example_strategy = static_cast<qtrade::demo::ExampleStrategy*>(strategy.get());
  auto order_sender = [](const qtrade::Order& order) {
    spdlog::info("[OrderSender] {} {} {} @ {}",
                 order.instrument,
                 order.side == qtrade::OrderSide::kBuy ? "BUY" : "SELL",
                 order.volume,
                 order.price);
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
    qtrade::adapter::MarketSourceConfig source_cfg;
    source_cfg.name = "MockDataSource";
    source_cfg.connection_string = "mock://localhost";
    source_ptr->Connect(source_cfg);
    market_handler.Subscribe({"IF2401", "IC2401"});
  }

  std::atomic<bool> stop_flag{false};
  qtrade::common::InstallShutdownHandler(stop_flag);

  if (demo_seconds > 0) {
    spdlog::info("[qtrade_engine] running {}s (demo mode, Ctrl+C to stop early)...", demo_seconds);
  } else {
    spdlog::info("[qtrade_engine] running until SIGINT/SIGTERM...");
  }

  qtrade::common::RunUntilStop(stop_flag, std::chrono::seconds{demo_seconds});

  engine.Stop();
  spdlog::info("[qtrade_engine] stopped cleanly");
  return EXIT_SUCCESS;
}
