/// @file      config_repository.hpp
/// @brief     配置持久化抽象（与具体数据库解耦）
/// @author    wengjianhong
/// @date      2026-06-28
/// @copyright CC BY-NC-SA 4.0
#ifndef QTRADE_SERVICE_CONFIG_REPOSITORY_HPP_
#define QTRADE_SERVICE_CONFIG_REPOSITORY_HPP_

#include <qtrade/error_code/error_codes.hpp>

#include "include/database/config.hpp"

#include <cstdint>
#include <map>
#include <memory>
#include <string>

namespace qtrade::service {

/// 配置作用域（与 proto GetConfigRequest 对齐）
struct ConfigScope {
  std::string tenant_id = "default";
  std::string engine_id = "default";
};

/// 持久化层连接选项
struct DbRepositoryOptions {
  bool enabled = false;
  cpp_utils::database::ConnectionOptions db;
};

/// @brief 配置读写仓储接口
class IConfigRepository {
 public:
  virtual ~IConfigRepository() = default;

  virtual ErrorCode EnsureSchema() = 0;

  virtual ErrorCode Load(const ConfigScope& scope, std::map<std::string, std::string>& entries,
                         std::uint64_t& version) = 0;

  virtual ErrorCode Save(const ConfigScope& scope, const std::map<std::string, std::string>& entries,
                         std::uint64_t version) = 0;
};

[[nodiscard]] std::unique_ptr<IConfigRepository> CreateConfigRepository(const DbRepositoryOptions& options);

[[nodiscard]] DbRepositoryOptions ParseDbRepositoryOptions(const std::string& json_path);

}  // namespace qtrade::service

#endif  // QTRADE_SERVICE_CONFIG_REPOSITORY_HPP_
