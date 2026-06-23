/// @file      engine_config_loader.hpp
/// @brief     从 JSON 加载 EngineOptions
/// @author    wengjianhong
/// @date      2026-06-22
/// @copyright CC BY-NC-SA 4.0
#ifndef QTRADE_ENGINE_ENGINE_CONFIG_LOADER_HPP_
#define QTRADE_ENGINE_ENGINE_CONFIG_LOADER_HPP_

#include <qtrade/engine/engine_options.hpp>
#include <qtrade/error_code/code_define.hpp>

#include <string>

namespace qtrade::engine {

/// @brief 解析引擎 JSON 配置文件为 EngineOptions
/// @param json_path 引擎配置文件路径（如 demo/config/engine.json）
/// @param options 输出：解析后的引擎启动选项
/// @return ErrorCode::kSuccess 表示成功；文件不存在返回 ErrorCode::kNotFound
ErrorCode LoadEngineOptionsFromJson(const std::string& json_path, EngineOptions& options);

}  // namespace qtrade::engine

#endif  // QTRADE_ENGINE_ENGINE_CONFIG_LOADER_HPP_
