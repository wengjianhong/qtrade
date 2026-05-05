#include <cstdlib>

#include <spdlog/spdlog.h>

#include "engine/trading_engine.h"

int main(int argc, char** argv) {
  (void)argc;
  (void)argv;

  spdlog::set_level(spdlog::level::info);
  spdlog::info("quant-trading engine starting (dry-run scaffold)");

  quant::trading::engine::TradingEngineConfig cfg;
  cfg.dry_run = true;

  quant::trading::engine::TradingEngine engine(cfg);
  const auto rc = engine.Start();
  if (rc != quant::trading::ErrorCode::kOk) {
    spdlog::error("engine start failed");
    return EXIT_FAILURE;
  }

  engine.Stop();
  spdlog::info("quant-trading engine stopped cleanly");
  return EXIT_SUCCESS;
}
