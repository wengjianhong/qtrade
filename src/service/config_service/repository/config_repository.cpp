/// @file      config_repository.cpp
/// @brief     配置仓储工厂与 JSON 解析
/// @author    wengjianhong
/// @date      2026-06-28
/// @copyright CC BY-NC-SA 4.0
#include "service/config_service/repository/config_repository.hpp"

#include "service/config_service/repository/soci_config_repository.hpp"

#include "include/database/types.hpp"

#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

#include <fstream>
#include <utility>

namespace qtrade::service {
namespace {

cpp_utils::database::DatabaseType ParseDatabaseType(const std::string& name) {
  if (name == "mysql") {
    return cpp_utils::database::DatabaseType::kMySql;
  }
  if (name == "postgresql" || name == "postgres") {
    return cpp_utils::database::DatabaseType::kPostgreSql;
  }
  if (name == "oracle") {
    return cpp_utils::database::DatabaseType::kOracle;
  }
  if (name == "odbc") {
    return cpp_utils::database::DatabaseType::kOdbc;
  }
  return cpp_utils::database::DatabaseType::kSqlite3;
}

}  // namespace

std::unique_ptr<IConfigRepository> CreateConfigRepository(const DbRepositoryOptions& options) {
  if (!options.enabled) {
    return nullptr;
  }
  return std::make_unique<SociConfigRepository>(options.db);
}

DbRepositoryOptions ParseDbRepositoryOptions(const std::string& json_path) {
  DbRepositoryOptions options;

  std::ifstream ifs(json_path);
  if (!ifs.is_open()) {
    return options;
  }

  nlohmann::json root;
  try {
    ifs >> root;
  } catch (const nlohmann::json::exception& ex) {
    spdlog::warn("[ConfigRepository] invalid JSON in {}: {}", json_path, ex.what());
    return options;
  }

  if (!root.contains("repository") || !root["repository"].is_object()) {
    return options;
  }

  const auto& repo = root["repository"];
  options.enabled = repo.value("enabled", false);
  if (repo.contains("backend")) {
    options.db.database_type = ParseDatabaseType(repo["backend"].get<std::string>());
  }
  if (repo.contains("conn_string")) {
    options.db.conn_string = repo["conn_string"].get<std::string>();
  }
  return options;
}

}  // namespace qtrade::service
