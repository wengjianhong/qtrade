/// @file logger.h
/// @brief High-performance global logger for quant trading engine (file only)
/// @author wengjianhong
/// @date 2026-05-05
/// @version 0.1.0
/// @copyright Copyright (c) 2026 quant-trading
#ifndef QUANT_TRADING_COMMON_LOGGER_H_
#define QUANT_TRADING_COMMON_LOGGER_H_

#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/spdlog.h>

#include <cstdio>
#include <string>

namespace quant::common {

/// @brief 初始化高性能文件日志（无控制台，低延迟）
/// @param log_dir      日志目录
/// @param log_filename 日志文件名
/// @param max_size_mb  单个文件最大大小(MB)
/// @param max_files    最大保留文件数
/// @param level        日志级别
inline void InitLogger(const std::string& log_dir = "logs",
                       const std::string& log_filename = "trading-engine.log",
                       size_t max_size_mb = 20,
                       size_t max_files = 10,
                       spdlog::level::level_enum level = spdlog::level::info) {
  try {
    // 单个文件大小
    size_t rotate_size = max_size_mb * 1024 * 1024;

    // 文件输出 sink（滚动、多线程安全）
    auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
      log_dir + "/" + log_filename, rotate_size, max_files, false);

#ifdef DEBUG
    // 日志格式: [时间] [日志等级] [进程号] [线程号] [代码位置] 日志内容
    file_sink->set_pattern("[%Y-%m-%d %X.%e] [%l] [%P] [%t] [%g:%#] %v");
#else
    // 日志格式: [时间] [日志等级] [进程号] [线程号] 日志内容
    file_sink->set_pattern("[%Y-%m-%d %X.%e] [%l] [%P] [%t] %v");
#endif
    file_sink->set_level(spdlog::level::trace);

    // 创建全局 logger
    auto logger = std::make_shared<spdlog::logger>("global", file_sink);
    logger->set_level(level);

    // 高性能配置：warn/error 级别立即刷盘，info 异步批量刷
    logger->flush_on(spdlog::level::warn);
    spdlog::set_default_logger(logger);

    // 启动日志
    spdlog::info("[logger] init success (file-only mode)");
    spdlog::info("[logger] init success (file-only mode)");
    spdlog::info("[logger] log path: {}/{}", log_dir, log_filename);
    spdlog::info("[logger] max size: {}MB, max files: {}", max_size_mb, max_files);
  } catch (const spdlog::spdlog_ex& ex) {
    fprintf(stderr, "logger init failed: %s\n", ex.what());
    throw;
  }
}

}  // namespace quant::common

#endif  // QUANT_TRADING_COMMON_LOGGER_H_
