/// @file      config_store.hpp
/// @brief     配置中心内存存储
/// @author    wengjianhong
/// @date      2026-06-22
/// @copyright CC BY-NC-SA 4.0
#ifndef QTRADE_SERVICE_CONFIG_STORE_HPP_
#define QTRADE_SERVICE_CONFIG_STORE_HPP_

#include <qtrade/config/v1/config.pb.h>

#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <map>
#include <mutex>
#include <optional>
#include <string>

namespace qtrade::service {

/// @brief 线程安全的配置快照与变更通知
class ConfigStore {
 public:
  /// @brief 从键值 map 加载全量配置
  /// @param entries 配置键值对
  /// @param version 初始版本号；为 0 时使用 1
  void LoadFromMap(const std::map<std::string, std::string>& entries, std::uint64_t version = 1);

  /// @brief 获取当前全量配置快照
  /// @return Protobuf ConfigSnapshot（含 version 与 entries）
  qtrade::config::v1::ConfigSnapshot GetSnapshot() const;

  /// @brief 阻塞等待 version > since_version，或超时
  /// @param since_version 客户端已应用的版本号
  /// @param timeout 最长等待时间
  /// @return 有更新时返回 ConfigDelta；超时返回 std::nullopt
  std::optional<qtrade::config::v1::ConfigDelta> WaitForDelta(std::uint64_t since_version,
                                                              std::chrono::milliseconds timeout) const;

  /// @brief 发布配置增量并 bump 版本
  /// @param delta 待应用的增量（upserts / deletes）
  void PublishDelta(const qtrade::config::v1::ConfigDelta& delta);

 private:
  mutable std::mutex mutex_;                    ///< 保护 version_ 与 entries_
  mutable std::condition_variable cv_;          ///< 通知 WatchConfig 等待方
  std::uint64_t version_ = 0;                   ///< 当前配置版本号
  std::map<std::string, std::string> entries_;  ///< 内存键值存储
};

}  // namespace qtrade::service

#endif  // QTRADE_SERVICE_CONFIG_STORE_HPP_
