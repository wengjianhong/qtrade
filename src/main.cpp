/// @file main.cpp
/// @brief Main entry point for the qtrade engine
/// @author wengjianhong
/// @date 2026-05-05
/// @version 0.1.0
/// @copyright Copyright (c) 2026 qtrade
/// @note This file is part of the qtrade engine
#include <qtrade/engine/trading_engine.hpp>
#include <src/common/logger.hpp>

#include <spdlog/spdlog.h>

#include <cstdlib>

int main(int argc, char** argv) {
  (void)argc;
  (void)argv;

  // ====================== 初始化纯文件日志 ======================
  quant::common::InitLogger("logs",
                            "trading-engine.log",
                            20,  // 单个文件最大 20MB
                            10,  // 保留 10 个文件
                            spdlog::level::info);

  spdlog::info("==================================================");
  spdlog::info("qtrade engine starting...");
  spdlog::info("==================================================");
  spdlog::info("[main] system bootstrap start");
  spdlog::info("[main] running in dry-run mode");

  // 加载引擎配置
  quant::trading::engine::TradingEngineConfig cfg;
  cfg.dry_run = true;
  spdlog::info("[main] trading engine config initialized");

  // 创建交易引擎实例
  quant::trading::engine::TradingEngine engine(cfg);
  spdlog::info("[main] trading engine instance created successfully");

  // 启动引擎
  spdlog::info("[main] starting trading engine main loop...");
  const auto rc = engine.Start();

  // 启动失败处理
  if (rc != quant::trading::ErrorCode::kOk) {
    spdlog::error("[main] engine start failed! error code: {}", static_cast<int>(rc));
    spdlog::critical("[main] system aborting due to start failure");
    return EXIT_FAILURE;
  }

  spdlog::info("[main] trading engine started successfully");
  spdlog::info("[main] engine is running...");

  // 停止引擎
  spdlog::warn("[main] preparing to stop trading engine...");
  engine.Stop();

  // 退出日志
  spdlog::info("[main] trading engine stopped cleanly");
  spdlog::info("[main] system shutdown successfully");
  spdlog::info("==================================================\n");

  return EXIT_SUCCESS;
}
