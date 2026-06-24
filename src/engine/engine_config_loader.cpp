/// @file      engine_config_loader.cpp
/// @brief     EngineOptions JSON 加载
/// @author    wengjianhong
/// @date      2026-06-22
/// @copyright CC BY-NC-SA 4.0
#include "engine/engine_config_loader.hpp"

#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

#include <fstream>

namespace qtrade::engine {

ErrorCode LoadEngineOptionsFromJson(const std::string& json_path, EngineOptions& options) {
  std::ifstream ifs(json_path);
  if (!ifs.is_open()) {
    spdlog::error("[EngineConfig] cannot open: {}", json_path);
    return ErrorCode::kNotFound;
  }

  nlohmann::json root;
  try {
    ifs >> root;
  } catch (const nlohmann::json::exception& ex) {
    spdlog::error("[EngineConfig] invalid JSON: {}", ex.what());
    return ErrorCode::kInternal;
  }

  if (root.contains("config_service")) {
    options.config_server_address = root["config_service"].get<std::string>();
  }
  if (root.contains("tenant_id")) {
    options.tenant_id = root["tenant_id"].get<std::string>();
  }
  if (root.contains("engine_id")) {
    options.engine_id = root["engine_id"].get<std::string>();
  }
  if (root.contains("log_topic")) {
    options.log_topic = root["log_topic"].get<std::string>();
  }
  if (root.contains("monitor_endpoint")) {
    options.monitor_endpoint = root["monitor_endpoint"].get<std::string>();
  }

  return ErrorCode::kSuccess;
}

}  // namespace qtrade::engine
