/// @file      config_repository.hpp
/// @brief     配置持久化抽象（与具体数据库解耦）
/// @author    wengjianhong
/// @date      2026-06-28
/// @copyright CC BY-NC-SA 4.0
#ifndef QTRADE_SERVICE_CONFIG_REPOSITORY_HPP_
#define QTRADE_SERVICE_CONFIG_REPOSITORY_HPP_

#include <qtrade/error_code/error_codes.hpp>

#include <cpputils/database/config.hpp>

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

/// 持久化层连接选项（对应 config JSON 的 repository 段）
struct DbRepositoryOptions {
  bool enabled = false;
  bool use_pool = false;                              ///< true 时使用连接池借连接
  std::size_t pool_size = 4;                          ///< 连接池大小（use_pool 时有效）
  time_t pool_lease_timeout = 0;                      ///< 借连接最长等待（秒）
  cpp_utils::database::ConnectionOptions connection;  ///< 单连接参数
};

/// @brief 配置读写仓储接口
class IConfigRepository {
 public:
  virtual ~IConfigRepository() = default;

  virtual ErrorCode EnsureSchema() = 0;

  virtual ErrorCode Load(const ConfigScope& scope,
                         std::map<std::string, std::string>& entries,
                         std::uint64_t& version) = 0;

  virtual ErrorCode Save(const ConfigScope& scope,
                         const std::map<std::string, std::string>& entries,
                         std::uint64_t version) = 0;
};

[[nodiscard]] std::unique_ptr<IConfigRepository> CreateConfigRepository(const DbRepositoryOptions& options);

[[nodiscard]] DbRepositoryOptions ParseDbRepositoryOptions(const std::string& json_path);

}  // namespace qtrade::service

#endif  // QTRADE_SERVICE_CONFIG_REPOSITORY_HPP_
