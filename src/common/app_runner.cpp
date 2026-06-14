#include "common/app_runner.hpp"

#include <csignal>
#include <spdlog/spdlog.h>

#include <thread>

namespace qtrade::common {

namespace {

std::atomic<bool>* g_stop_flag = nullptr;

void OnShutdownSignal(int signum) {
  if (g_stop_flag != nullptr) {
    g_stop_flag->store(true, std::memory_order_release);
  }
  spdlog::warn("[app_runner] received signal {}, shutting down...", signum);
}

}  // namespace

void InstallShutdownHandler(std::atomic<bool>& stop_flag) {
  g_stop_flag = &stop_flag;
  std::signal(SIGINT, OnShutdownSignal);
  std::signal(SIGTERM, OnShutdownSignal);
}

void RunUntilStop(std::atomic<bool>& stop_flag, std::chrono::seconds demo_seconds) {
  const auto deadline =
      demo_seconds.count() > 0 ? std::chrono::steady_clock::now() + demo_seconds
                               : std::chrono::steady_clock::time_point::max();

  while (!stop_flag.load(std::memory_order_acquire)) {
    if (std::chrono::steady_clock::now() >= deadline) {
      spdlog::info("[app_runner] demo duration elapsed, stopping");
      break;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
}

bool ParseDemoSeconds(int argc, char** argv, int& demo_seconds) {
  for (int i = 1; i < argc; ++i) {
    const std::string arg = argv[i];
    if (arg == "--demo-seconds" && i + 1 < argc) {
      demo_seconds = std::stoi(argv[++i]);
      return true;
    }
  }
  return false;
}

}  // namespace qtrade::common
