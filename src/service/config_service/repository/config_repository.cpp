/// @file      config_repository.cpp
/// @brief     配置仓储工厂与 JSON 解析
/// @author    wengjianhong
/// @date      2026-06-28
/// @copyright CC BY-NC-SA 4.0
#include "service/config_service/repository/config_repository.hpp"

#include "service/config_service/repository/soci_config_repository.hpp"

#include <cpputils/database/database.hpp>

#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

#include <fstream>
#include <string_view>
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

void ParseSociOptions(const nlohmann::json& node, std::map<std::string, std::string>& out) {
  if (!node.contains("soci_options") || !node["soci_options"].is_object()) {
    return;
  }
  for (auto it = node["soci_options"].begin(); it != node["soci_options"].end(); ++it) {
    out[it.key()] = it.value().get<std::string>();
  }
}

time_t ParseSeconds(const nlohmann::json& node, const char* key) {
  if (!node.contains(key)) {
    return 0;
  }
  return static_cast<time_t>(node[key].get<std::int64_t>());
}

std::string StripDbNamePrefix(std::string conn_string) {
  constexpr std::string_view kPrefix = "dbname=";
  if (conn_string.rfind(kPrefix.data(), 0) == 0) {
    return conn_string.substr(kPrefix.size());
  }
  return conn_string;
}

cpp_utils::database::ConnectionOptions BuildLegacyConnectionOptions(const nlohmann::json& repo,
                                                                    cpp_utils::database::DatabaseType type) {
  cpp_utils::database::ConnectionOptions options;
  options.database_type = type;
  options.conn_string = repo["conn_string"].get<std::string>();
  ParseSociOptions(repo, options.soci_options);
  return options;
}

cpp_utils::database::ConnectionOptions BuildSqliteConnectionOptions(const nlohmann::json& repo) {
  cpp_utils::database::SqliteConfig config;
  if (repo.contains("sqlite") && repo["sqlite"].is_object()) {
    const auto& sqlite = repo["sqlite"];
    if (sqlite.contains("database_path")) {
      config.database_path = sqlite["database_path"].get<std::string>();
    }
    config.busy_timeout = ParseSeconds(sqlite, "busy_timeout");
    ParseSociOptions(sqlite, config.soci_options);
  } else if (repo.contains("conn_string")) {
    config.database_path = StripDbNamePrefix(repo["conn_string"].get<std::string>());
    ParseSociOptions(repo, config.soci_options);
  }
  return cpp_utils::database::ConnectionOptions{config};
}

cpp_utils::database::ConnectionOptions BuildMySqlConnectionOptions(const nlohmann::json& repo) {
  cpp_utils::database::MySqlConfig config;
  if (repo.contains("mysql") && repo["mysql"].is_object()) {
    const auto& mysql = repo["mysql"];
    if (mysql.contains("host")) {
      config.host = mysql["host"].get<std::string>();
    }
    if (mysql.contains("port")) {
      config.port = mysql["port"].get<int>();
    }
    if (mysql.contains("user")) {
      config.user = mysql["user"].get<std::string>();
    }
    if (mysql.contains("password")) {
      config.password = mysql["password"].get<std::string>();
    }
    if (mysql.contains("database_name")) {
      config.database_name = mysql["database_name"].get<std::string>();
    }
    config.connect_timeout = ParseSeconds(mysql, "connect_timeout");
    ParseSociOptions(mysql, config.soci_options);
  } else if (repo.contains("conn_string")) {
    return BuildLegacyConnectionOptions(repo, cpp_utils::database::DatabaseType::kMySql);
  }
  return cpp_utils::database::ConnectionOptions{config};
}

cpp_utils::database::PostgreSqlConnectionType ParsePostgreSqlConnectionType(const std::string& name) {
  if (name == "unix") {
    return cpp_utils::database::PostgreSqlConnectionType::kUnix;
  }
  return cpp_utils::database::PostgreSqlConnectionType::kTcp;
}

cpp_utils::database::ConnectionOptions BuildPostgreSqlConnectionOptions(const nlohmann::json& repo) {
  cpp_utils::database::PostgreSqlConfig config;
  if (repo.contains("postgresql") && repo["postgresql"].is_object()) {
    const auto& pg = repo["postgresql"];
    if (pg.contains("host")) {
      config.host = pg["host"].get<std::string>();
    }
    if (pg.contains("port")) {
      config.port = pg["port"].get<int>();
    }
    if (pg.contains("user")) {
      config.user = pg["user"].get<std::string>();
    }
    if (pg.contains("password")) {
      config.password = pg["password"].get<std::string>();
    }
    if (pg.contains("database_name")) {
      config.database_name = pg["database_name"].get<std::string>();
    }
    if (pg.contains("socket_path")) {
      config.socket_path = pg["socket_path"].get<std::string>();
    }
    if (pg.contains("ssl_mode")) {
      config.ssl_mode = pg["ssl_mode"].get<std::string>();
    }
    if (pg.contains("connection_type")) {
      config.connection_type = ParsePostgreSqlConnectionType(pg["connection_type"].get<std::string>());
    }
    config.connect_timeout = ParseSeconds(pg, "connect_timeout");
    ParseSociOptions(pg, config.soci_options);
  } else if (repo.contains("conn_string")) {
    return BuildLegacyConnectionOptions(repo, cpp_utils::database::DatabaseType::kPostgreSql);
  }
  return cpp_utils::database::ConnectionOptions{config};
}

cpp_utils::database::ConnectionOptions BuildOracleConnectionOptions(const nlohmann::json& repo) {
  cpp_utils::database::OracleConfig config;
  if (repo.contains("oracle") && repo["oracle"].is_object()) {
    const auto& oracle = repo["oracle"];
    if (oracle.contains("host")) {
      config.host = oracle["host"].get<std::string>();
    }
    if (oracle.contains("port")) {
      config.port = oracle["port"].get<int>();
    }
    if (oracle.contains("user")) {
      config.user = oracle["user"].get<std::string>();
    }
    if (oracle.contains("password")) {
      config.password = oracle["password"].get<std::string>();
    }
    if (oracle.contains("service_name")) {
      config.service_name = oracle["service_name"].get<std::string>();
    }
    config.connect_timeout = ParseSeconds(oracle, "connect_timeout");
    ParseSociOptions(oracle, config.soci_options);
  } else if (repo.contains("conn_string")) {
    return BuildLegacyConnectionOptions(repo, cpp_utils::database::DatabaseType::kOracle);
  }
  return cpp_utils::database::ConnectionOptions{config};
}

cpp_utils::database::ConnectionOptions BuildConnectionOptions(const nlohmann::json& repo) {
  const auto type = ParseDatabaseType(repo.value("backend", "sqlite3"));

  if (repo.contains("conn_string") && !repo.contains("sqlite") && !repo.contains("mysql") &&
      !repo.contains("postgresql") && !repo.contains("oracle")) {
    return BuildLegacyConnectionOptions(repo, type);
  }

  switch (type) {
    case cpp_utils::database::DatabaseType::kMySql:
      return BuildMySqlConnectionOptions(repo);
    case cpp_utils::database::DatabaseType::kPostgreSql:
      return BuildPostgreSqlConnectionOptions(repo);
    case cpp_utils::database::DatabaseType::kOracle:
      return BuildOracleConnectionOptions(repo);
    case cpp_utils::database::DatabaseType::kSqlite3:
    default:
      return BuildSqliteConnectionOptions(repo);
  }
}

void ParsePoolOptions(const nlohmann::json& repo, DbRepositoryOptions& options) {
  if (!repo.contains("pool") || !repo["pool"].is_object()) {
    return;
  }

  const auto& pool = repo["pool"];
  options.use_pool = pool.value("enabled", true);
  if (pool.contains("size")) {
    options.pool_size = pool["size"].get<std::size_t>();
  }
  options.pool_lease_timeout = ParseSeconds(pool, "lease_timeout");
}

}  // namespace

std::unique_ptr<IConfigRepository> CreateConfigRepository(const DbRepositoryOptions& options) {
  if (!options.enabled) {
    return nullptr;
  }
  return std::make_unique<SociConfigRepository>(options);
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
  if (!options.enabled) {
    return options;
  }

  options.connection = BuildConnectionOptions(repo);
  ParsePoolOptions(repo, options);
  return options;
}

}  // namespace qtrade::service
