/// @file      soci_config_repository.cpp
/// @brief     SociConfigRepository 实现
/// @author    wengjianhong
/// @date      2026-06-28
/// @copyright CC BY-NC-SA 4.0
#include "service/config_service/repository/soci_config_repository.hpp"

#include "include/database/connection.hpp"
#include "include/database/error.hpp"

#include <spdlog/spdlog.h>

#include <sstream>
#include <utility>

namespace qtrade::service {
namespace {

constexpr const char* kEnsureSchemaSql = R"(
CREATE TABLE IF NOT EXISTS config_version (
  tenant_id TEXT NOT NULL,
  engine_id TEXT NOT NULL,
  version INTEGER NOT NULL,
  PRIMARY KEY (tenant_id, engine_id)
);

CREATE TABLE IF NOT EXISTS config_kv (
  tenant_id TEXT NOT NULL,
  engine_id TEXT NOT NULL,
  config_key TEXT NOT NULL,
  config_value TEXT NOT NULL,
  PRIMARY KEY (tenant_id, engine_id, config_key)
);
)";

std::string EscapeSqlLiteral(const std::string& value) {
  std::string escaped;
  escaped.reserve(value.size());
  for (const char ch : value) {
    if (ch == '\'') {
      escaped += "''";
    } else {
      escaped += ch;
    }
  }
  return escaped;
}

ErrorCode MapDbError(cpp_utils::database::Error error) {
  switch (error) {
    case cpp_utils::database::Error::kSuccess:
      return ErrorCode::kSuccess;
    case cpp_utils::database::Error::kNotFound:
      return ErrorCode::kNotFound;
    case cpp_utils::database::Error::kInvalidArgument:
      return ErrorCode::kInternal;
    case cpp_utils::database::Error::kNotConnected:
    case cpp_utils::database::Error::kConnectFailed:
    case cpp_utils::database::Error::kQueryFailed:
    case cpp_utils::database::Error::kExecuteFailed:
    case cpp_utils::database::Error::kTransactionFailed:
    default:
      return ErrorCode::kSystemError;
  }
}

}  // namespace

SociConfigRepository::SociConfigRepository(cpp_utils::database::ConnectionOptions db_options)
  : connection_(std::make_unique<cpp_utils::database::Connection>(std::move(db_options))) {
  if (const auto rc = connection_->Connect(); rc != cpp_utils::database::Error::kSuccess) {
    spdlog::error("[SociConfigRepository] connect failed: {}", connection_->LastError());
  }
}

SociConfigRepository::~SociConfigRepository() = default;

ErrorCode SociConfigRepository::EnsureSchema() {
  if (!connection_->IsConnected()) {
    return ErrorCode::kSystemError;
  }
  const auto rc = connection_->Execute(kEnsureSchemaSql);
  if (rc != cpp_utils::database::Error::kSuccess) {
    spdlog::error("[SociConfigRepository] ensure schema failed: {}", connection_->LastError());
  }
  return MapDbError(rc);
}

ErrorCode SociConfigRepository::Load(const ConfigScope& scope,
                                     std::map<std::string, std::string>& entries,
                                     std::uint64_t& version) {
  if (!connection_->IsConnected()) {
    return ErrorCode::kSystemError;
  }

  entries.clear();
  version = 0;

  std::ostringstream version_sql;
  version_sql << "SELECT version FROM config_version WHERE tenant_id = '" << EscapeSqlLiteral(scope.tenant_id)
              << "' AND engine_id = '" << EscapeSqlLiteral(scope.engine_id) << "'";

  auto [version_query_err, version_result] = connection_->Query(version_sql.str());
  if (version_query_err != cpp_utils::database::Error::kSuccess || version_result == nullptr) {
    spdlog::error("[SociConfigRepository] load version failed: {}", connection_->LastError());
    return MapDbError(version_query_err);
  }

  const auto version_row = version_result->Fetch();
  if (!version_row.has_value()) {
    return ErrorCode::kNotFound;
  }

  std::optional<std::int64_t> version_opt;
  if (const auto cell = version_row->get_value("version")) {
    version_opt = cell->as_int64();
  }
  if (!version_opt.has_value()) {
    return ErrorCode::kSystemError;
  }
  version = static_cast<std::uint64_t>(version_opt.value());

  std::ostringstream kv_sql;
  kv_sql << "SELECT config_key, config_value FROM config_kv WHERE tenant_id = '" << EscapeSqlLiteral(scope.tenant_id)
         << "' AND engine_id = '" << EscapeSqlLiteral(scope.engine_id) << "'";

  auto [kv_query_err, kv_result] = connection_->Query(kv_sql.str());
  if (kv_query_err != cpp_utils::database::Error::kSuccess || kv_result == nullptr) {
    spdlog::error("[SociConfigRepository] load entries failed: {}", connection_->LastError());
    return MapDbError(kv_query_err);
  }

  while (const auto row = kv_result->Fetch()) {
    const auto key_cell = row->get_value("config_key");
    const auto value_cell = row->get_value("config_value");
    if (!key_cell.has_value() || !value_cell.has_value()) {
      continue;
    }
    const auto key = key_cell->as_string();
    const auto value = value_cell->as_string();
    if (key.has_value() && value.has_value()) {
      entries.emplace(key.value(), value.value());
    }
  }
  return ErrorCode::kSuccess;
}

ErrorCode SociConfigRepository::Save(const ConfigScope& scope,
                                     const std::map<std::string, std::string>& entries,
                                     std::uint64_t version) {
  if (!connection_->IsConnected()) {
    return ErrorCode::kSystemError;
  }

  auto [begin_rc, tx] = connection_->BeginTransaction();
  if (begin_rc != cpp_utils::database::Error::kSuccess) {
    spdlog::error("[SociConfigRepository] begin transaction failed: {}", connection_->LastError());
    return MapDbError(begin_rc);
  }

  std::ostringstream upsert_version;
  upsert_version << "UPDATE config_version SET version = " << version << " WHERE tenant_id = '"
                 << EscapeSqlLiteral(scope.tenant_id) << "' AND engine_id = '" << EscapeSqlLiteral(scope.engine_id)
                 << "'";

  cpp_utils::database::ExecuteResult update_result;
  if (const auto rc = connection_->Execute(upsert_version.str(), &update_result);
      rc != cpp_utils::database::Error::kSuccess) {
    (void)tx.Rollback();
    spdlog::error("[SociConfigRepository] update version failed: {}", connection_->LastError());
    return MapDbError(rc);
  }

  if (update_result.affected_rows == 0) {
    std::ostringstream insert_version;
    insert_version << "INSERT INTO config_version(tenant_id, engine_id, version) VALUES('"
                   << EscapeSqlLiteral(scope.tenant_id) << "', '" << EscapeSqlLiteral(scope.engine_id) << "', "
                   << version << ")";
    if (const auto rc = connection_->Execute(insert_version.str()); rc != cpp_utils::database::Error::kSuccess) {
      (void)tx.Rollback();
      spdlog::error("[SociConfigRepository] insert version failed: {}", connection_->LastError());
      return MapDbError(rc);
    }
  }

  std::ostringstream delete_sql;
  delete_sql << "DELETE FROM config_kv WHERE tenant_id = '" << EscapeSqlLiteral(scope.tenant_id)
             << "' AND engine_id = '" << EscapeSqlLiteral(scope.engine_id) << "'";
  if (const auto rc = connection_->Execute(delete_sql.str()); rc != cpp_utils::database::Error::kSuccess) {
    (void)tx.Rollback();
    spdlog::error("[SociConfigRepository] clear entries failed: {}", connection_->LastError());
    return MapDbError(rc);
  }

  for (const auto& [key, value] : entries) {
    std::ostringstream insert_sql;
    insert_sql << "INSERT INTO config_kv(tenant_id, engine_id, config_key, config_value) VALUES('"
               << EscapeSqlLiteral(scope.tenant_id) << "', '" << EscapeSqlLiteral(scope.engine_id) << "', '"
               << EscapeSqlLiteral(key) << "', '" << EscapeSqlLiteral(value) << "')";
    if (const auto rc = connection_->Execute(insert_sql.str()); rc != cpp_utils::database::Error::kSuccess) {
      (void)tx.Rollback();
      spdlog::error("[SociConfigRepository] insert entry failed: {}", connection_->LastError());
      return MapDbError(rc);
    }
  }

  if (const auto rc = tx.Commit(); rc != cpp_utils::database::Error::kSuccess) {
    spdlog::error("[SociConfigRepository] commit failed: {}", connection_->LastError());
    return MapDbError(rc);
  }
  return ErrorCode::kSuccess;
}

}  // namespace qtrade::service
