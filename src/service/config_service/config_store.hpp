/// @file      config_store.hpp
/// @brief     配置中心内存存储
/// @author    wengjianhong
/// @date      2026-06-22
/// @copyright CC BY-NC-SA 4.0
#ifndef QTRADE_SERVICE_CONFIG_STORE_HPP_
#define QTRADE_SERVICE_CONFIG_STORE_HPP_

#include <qtrade/config/v1/config.pb.h>

#include <cstdint>
#include <functional>
#include <map>
#include <mutex>
#include <string>

namespace qtrade::service {

/// @brief 线程安全的配置快照与版本通知
class ConfigStore {
 public:
  /// @brief 配置版本变更回调（在 LoadFromMap 持锁外调用）
  using VersionListener = std::function<void(std::uint64_t version)>;

  /// @brief 加载全量配置并 bump 版本，触发 VersionListener
  /// @param entries 配置键值对
  /// @param version 目标版本号；为 0 时在现有版本上 +1
  void LoadFromMap(const std::map<std::string, std::string>& entries, std::uint64_t version = 0);

  /// @brief 获取当前全量配置快照
  qtrade::config::v1::ConfigSnapshot GetSnapshot() const;

  /// @brief 注册版本变更监听（Async Watch 推送用）
  void SetVersionListener(VersionListener listener);

  /// @brief 当前配置版本号
  [[nodiscard]] std::uint64_t Version() const;

 private:
  qtrade::config::v1::ConfigSnapshot BuildSnapshotLocked() const;

  mutable std::mutex mutex_;
  std::uint64_t version_ = 0;
  VersionListener version_listener_;
  std::map<std::string, std::string> entries_;
};

}  // namespace qtrade::service

#endif  // QTRADE_SERVICE_CONFIG_STORE_HPP_
