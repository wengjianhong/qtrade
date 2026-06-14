/// @file      app_runner.hpp
/// @brief     独立进程通用启动与优雅退出
#ifndef QTRADE_COMMON_APP_RUNNER_HPP_
#define QTRADE_COMMON_APP_RUNNER_HPP_

#include <atomic>
#include <chrono>
#include <string>

namespace qtrade::common {

/// @brief 安装 SIGINT/SIGTERM 处理器，触发后 RunUntilStop 退出
void InstallShutdownHandler(std::atomic<bool>& stop_flag);

/// @brief 阻塞直至收到停止信号，或 demo_seconds 超时（<=0 表示仅信号退出）
void RunUntilStop(std::atomic<bool>& stop_flag,
                  std::chrono::seconds demo_seconds = std::chrono::seconds{0});

/// @brief 解析通用参数：--demo-seconds N
bool ParseDemoSeconds(int argc, char** argv, int& demo_seconds);

}  // namespace qtrade::common

#endif  // QTRADE_COMMON_APP_RUNNER_HPP_
