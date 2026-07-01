/// @file      config_store.cpp
/// @brief     ConfigStore 实现
/// @author    wengjianhong
/// @date      2026-06-22
/// @copyright CC BY-NC-SA 4.0
#include "service/config_service/config_store.hpp"

namespace qtrade::service {

void ConfigStore::LoadFromMap(const std::map<std::string, std::string>& entries, std::uint64_t version) {
  VersionListener listener;
  std::uint64_t new_version = 0;
  {
    std::lock_guard lock(mutex_);
    entries_ = entries;
    if (version > 0) {
      version_ = version;
    } else if (version_ == 0) {
      version_ = 1;
    } else {
      ++version_;
    }
    new_version = version_;
    listener = version_listener_;
  }
  if (listener) {
    listener(new_version);
  }
}

qtrade::config::v1::ConfigSnapshot ConfigStore::GetSnapshot() const {
  std::lock_guard lock(mutex_);
  return BuildSnapshotLocked();
}

void ConfigStore::SetVersionListener(VersionListener listener) {
  std::lock_guard lock(mutex_);
  version_listener_ = std::move(listener);
}

std::uint64_t ConfigStore::Version() const {
  std::lock_guard lock(mutex_);
  return version_;
}

qtrade::config::v1::ConfigSnapshot ConfigStore::BuildSnapshotLocked() const {
  qtrade::config::v1::ConfigSnapshot snapshot;
  snapshot.set_version(version_);
  for (const auto& [key, value] : entries_) {
    auto* entry = snapshot.add_entries();
    entry->set_key(key);
    entry->set_value(value);
  }
  return snapshot;
}

}  // namespace qtrade::service
