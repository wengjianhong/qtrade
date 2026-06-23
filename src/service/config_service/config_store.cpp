/// @file      config_store.cpp
/// @brief     ConfigStore 实现
/// @author    wengjianhong
/// @date      2026-06-22
/// @copyright CC BY-NC-SA 4.0
#include "service/config_service/config_store.hpp"

namespace qtrade::service {

void ConfigStore::LoadFromMap(const std::map<std::string, std::string>& entries, std::uint64_t version) {
  std::lock_guard lock(mutex_);
  entries_ = entries;
  version_ = version > 0 ? version : 1;
  cv_.notify_all();
}

qtrade::config::v1::ConfigSnapshot ConfigStore::GetSnapshot() const {
  std::lock_guard lock(mutex_);
  qtrade::config::v1::ConfigSnapshot snapshot;
  snapshot.set_version(version_);
  for (const auto& [key, value] : entries_) {
    auto* entry = snapshot.add_entries();
    entry->set_key(key);
    entry->set_value(value);
  }
  return snapshot;
}

std::optional<qtrade::config::v1::ConfigDelta> ConfigStore::WaitForDelta(std::uint64_t since_version,
                                                                         std::chrono::milliseconds timeout) const {
  std::unique_lock lock(mutex_);
  if (!cv_.wait_for(lock, timeout, [this, since_version] { return version_ > since_version; })) {
    return std::nullopt;
  }

  qtrade::config::v1::ConfigDelta delta;
  delta.set_version(version_);
  for (const auto& [key, value] : entries_) {
    auto* entry = delta.add_upserts();
    entry->set_key(key);
    entry->set_value(value);
  }
  return delta;
}

void ConfigStore::PublishDelta(const qtrade::config::v1::ConfigDelta& delta) {
  std::lock_guard lock(mutex_);
  version_ = delta.version() > version_ ? delta.version() : version_ + 1;

  for (const auto& key : delta.deletes()) {
    entries_.erase(key);
  }
  for (const auto& entry : delta.upserts()) {
    entries_[entry.key()] = entry.value();
  }
  cv_.notify_all();
}

}  // namespace qtrade::service
