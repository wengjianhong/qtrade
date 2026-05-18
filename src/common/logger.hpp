/// @file logger.h
/// @brief High-performance global logger for quant trading engine (file only)
/// @author wengjianhong
/// @date 2026-05-05
/// @version 0.1.0
/// @copyright Copyright (c) 2026 qtrade
#ifndef QTRADE_TRADING_COMMON_LOGGER_H_
#define QTRADE_TRADING_COMMON_LOGGER_H_
#include <spdlog/spdlog.h>

#include <string>

namespace qtrade::common {

/// @brief 初始化 spdlog 日志
/// @param log_dir      日志目录
/// @param log_filename 日志文件名
/// @param max_size_mb  单个文件最大大小(MB)
/// @param max_files    最大保留文件数
/// @param level        日志级别
bool init_spdlog_logger(const std::string& log_dir = "logs",
                 const std::string& log_filename = "qtrade.log",
                 size_t max_size_mb = 50,
                 size_t max_files = 10,
                 spdlog::level::level_enum level = spdlog::level::info);

}  // namespace qtrade::common

#endif  // QTRADE_TRADING_COMMON_LOGGER_H_
