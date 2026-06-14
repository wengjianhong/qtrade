/// @file      service_main.hpp
/// @brief     支撑服务独立进程通用入口（由 src/apps/qtrade_*_service/main.cpp 调用）
#ifndef QTRADE_COMMON_SERVICE_MAIN_HPP_
#define QTRADE_COMMON_SERVICE_MAIN_HPP_

#include "common/app_runner.hpp"
#include "common/logger.hpp"

#include <spdlog/spdlog.h>

#include <atomic>
#include <chrono>
#include <cstdlib>
#include <string>
#include <unistd.h>

namespace qtrade::common {

/// @brief 支撑服务 stub 主循环；后续替换为 Kafka/gRPC 服务端 bootstrap
inline int RunServiceMain(int argc,
                          char** argv,
                          const std::string& service_name,
                          const std::string& log_filename) {
  int demo_seconds = 0;
  ParseDemoSeconds(argc, argv, demo_seconds);

  if (!init_spdlog_logger("logs", log_filename)) {
    return EXIT_FAILURE;
  }

  spdlog::info("==================================================");
  spdlog::info("{} starting (pid={})", service_name, getpid());
  spdlog::info("==================================================");
  spdlog::info("[{}] service loop active (stub)", service_name);

  std::atomic<bool> stop_flag{false};
  InstallShutdownHandler(stop_flag);

  if (demo_seconds > 0) {
    spdlog::info("[{}] demo {}s, Ctrl+C to stop early", service_name, demo_seconds);
  } else {
    spdlog::info("[{}] waiting for SIGINT/SIGTERM...", service_name);
  }

  RunUntilStop(stop_flag, std::chrono::seconds{demo_seconds});

  spdlog::info("[{}] stopped cleanly", service_name);
  return EXIT_SUCCESS;
}

}  // namespace qtrade::common

#endif  // QTRADE_COMMON_SERVICE_MAIN_HPP_
