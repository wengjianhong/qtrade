/// @file      config_server.cpp
/// @brief     ConfigServer 实现
/// @author    wengjianhong
/// @date      2026-06-22
/// @copyright CC BY-NC-SA 4.0
#include "service/config_service/config_server.hpp"

#include "common/grpc/grpc_async_server.hpp"
#include "service/config_service/config_grpc_async_handler.hpp"
#include "service/config_service/repository/config_repository.hpp"

#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

#include <fstream>
#include <map>
#include <utility>

namespace qtrade::service {
namespace {

ConfigScope ParseConfigScope(const std::string& json_path) {
  ConfigScope scope;
  std::ifstream ifs(json_path);
  if (!ifs.is_open()) {
    return scope;
  }

  nlohmann::json root;
  try {
    ifs >> root;
  } catch (const nlohmann::json::exception&) {
    return scope;
  }

  if (root.contains("tenant_id")) {
    scope.tenant_id = root["tenant_id"].get<std::string>();
  }
  if (root.contains("engine_id")) {
    scope.engine_id = root["engine_id"].get<std::string>();
  }
  if (root.contains("repository") && root["repository"].is_object()) {
    const auto& repo = root["repository"];
    if (repo.contains("tenant_id")) {
      scope.tenant_id = repo["tenant_id"].get<std::string>();
    }
    if (repo.contains("engine_id")) {
      scope.engine_id = repo["engine_id"].get<std::string>();
    }
  }
  return scope;
}

}  // namespace

ConfigServer::ConfigServer() = default;

ConfigServer::~ConfigServer() { Shutdown(); }

ErrorCode ConfigServer::Start(const std::string& listen_address, std::shared_ptr<ConfigStore> store) {
  if (running_) {
    return ErrorCode::kSystemError;
  }
  if (!store) {
    return ErrorCode::kInternal;
  }

  store_ = std::move(store);
  grpc_server_ = std::make_unique<qtrade::common::grpc_async::GrpcAsyncServer>();
  handler_ = std::make_unique<ConfigGrpcAsyncHandler>();

  qtrade::common::grpc_async::GrpcAsyncServer::Options opts;
  opts.listen_address = listen_address;
  opts.cq_thread_count = 1;

  if (const auto rc = grpc_server_->Start(opts, &async_service_); rc != ErrorCode::kSuccess) {
    handler_.reset();
    grpc_server_.reset();
    store_.reset();
    return rc;
  }

  handler_->Init(&async_service_, grpc_server_->CompletionQueue(), store_);
  handler_->Start();

  running_ = true;
  spdlog::info("[ConfigServer] listening on {} (async+cq)", listen_address);
  return ErrorCode::kSuccess;
}

void ConfigServer::Shutdown() {
  if (!running_) {
    return;
  }

  if (handler_) {
    handler_->Shutdown();
    handler_.reset();
  }
  if (grpc_server_) {
    grpc_server_->Shutdown();
  }

  store_.reset();
  running_ = false;
}

void ConfigServer::Wait() {
  if (grpc_server_) {
    grpc_server_->Wait();
    grpc_server_.reset();
  }
}

ErrorCode LoadConfigStoreFromJson(const std::string& json_path, ConfigStore& store) {
  std::ifstream ifs(json_path);
  if (!ifs.is_open()) {
    spdlog::error("[ConfigServer] cannot open config file: {}", json_path);
    return ErrorCode::kNotFound;
  }

  nlohmann::json root;
  try {
    ifs >> root;
  } catch (const nlohmann::json::exception& ex) {
    spdlog::error("[ConfigServer] invalid JSON: {}", ex.what());
    return ErrorCode::kInternal;
  }

  std::map<std::string, std::string> entries;
  std::uint64_t version = 1;
  if (root.contains("config")) {
    const auto& cfg = root["config"];
    if (cfg.contains("version")) {
      version = cfg["version"].get<std::uint64_t>();
    }
    if (cfg.contains("entries") && cfg["entries"].is_object()) {
      for (auto it = cfg["entries"].begin(); it != cfg["entries"].end(); ++it) {
        entries[it.key()] = it.value().is_string() ? it.value().get<std::string>() : it.value().dump();
      }
    }
  }

  store.LoadFromMap(entries, version);
  spdlog::info("[ConfigServer] loaded {} entries, version={}", entries.size(), version);
  return ErrorCode::kSuccess;
}

ErrorCode BootstrapConfigStore(const std::string& json_path, ConfigStore& store) {
  const auto repo_options = ParseDbRepositoryOptions(json_path);
  if (!repo_options.enabled) {
    if (const auto rc = LoadConfigStoreFromJson(json_path, store); rc != ErrorCode::kSuccess) {
      store.LoadFromMap({}, 1);
      return rc;
    }
    return ErrorCode::kSuccess;
  }

  auto repository = CreateConfigRepository(repo_options);
  if (!repository) {
    spdlog::error("[ConfigServer] create repository failed");
    return ErrorCode::kInternal;
  }

  if (const auto rc = repository->EnsureSchema(); rc != ErrorCode::kSuccess) {
    spdlog::error("[ConfigServer] ensure schema failed");
    return rc;
  }

  const ConfigScope scope = ParseConfigScope(json_path);
  std::map<std::string, std::string> entries;
  std::uint64_t version = 0;
  const auto load_rc = repository->Load(scope, entries, version);
  if (load_rc == ErrorCode::kSuccess) {
    store.LoadFromMap(entries, version);
    spdlog::info("[ConfigServer] loaded {} entries from repository, version={}", entries.size(), version);
    return ErrorCode::kSuccess;
  }

  if (load_rc != ErrorCode::kNotFound) {
    spdlog::warn("[ConfigServer] repository load failed, fallback to JSON");
  }

  if (const auto rc = LoadConfigStoreFromJson(json_path, store); rc != ErrorCode::kSuccess) {
    store.LoadFromMap({}, 1);
    return rc;
  }

  const auto snapshot = store.GetSnapshot();
  std::map<std::string, std::string> seed_entries;
  for (const auto& entry : snapshot.entries()) {
    seed_entries.emplace(entry.key(), entry.value());
  }
  if (const auto save_rc = repository->Save(scope, seed_entries, snapshot.version()); save_rc != ErrorCode::kSuccess) {
    spdlog::warn("[ConfigServer] seed repository from JSON failed");
  } else {
    spdlog::info("[ConfigServer] seeded repository from JSON, version={}", snapshot.version());
  }
  return ErrorCode::kSuccess;
}

}  // namespace qtrade::service
