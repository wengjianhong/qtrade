/// @file      soci_config_repository.hpp
/// @brief     基于 cpp_utils::database::Connection 的配置仓储实现
/// @author    wengjianhong
/// @date      2026-06-28
/// @copyright CC BY-NC-SA 4.0
#ifndef QTRADE_SERVICE_SOCI_CONFIG_REPOSITORY_HPP_
#define QTRADE_SERVICE_SOCI_CONFIG_REPOSITORY_HPP_

#include "service/config_service/repository/config_repository.hpp"

#include <memory>

namespace cpp_utils::database {
class Connection;
}

namespace qtrade::service {

class SociConfigRepository final : public IConfigRepository {
 public:
  explicit SociConfigRepository(cpp_utils::database::ConnectionOptions db_options);
  ~SociConfigRepository() override;

  ErrorCode EnsureSchema() override;
  ErrorCode Load(const ConfigScope& scope, std::map<std::string, std::string>& entries,
                 std::uint64_t& version) override;
  ErrorCode Save(const ConfigScope& scope, const std::map<std::string, std::string>& entries,
                 std::uint64_t version) override;

 private:
  std::unique_ptr<cpp_utils::database::Connection> connection_;
};

}  // namespace qtrade::service

#endif  // QTRADE_SERVICE_SOCI_CONFIG_REPOSITORY_HPP_
