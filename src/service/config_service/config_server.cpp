/// @file      config_server.cpp
/// @brief     ConfigServer 实现
/// @author    wengjianhong
/// @date      2026-06-22
/// @copyright CC BY-NC-SA 4.0
#include "service/config_service/config_server.hpp"

#include "service/config_service/config_grpc_service.hpp"

#include <grpcpp/grpcpp.h>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

#include <fstream>
#include <map>
#include <utility>

namespace qtrade::service {

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
  service_ = std::make_unique<ConfigGrpcService>(store_);

  grpc::ServerBuilder builder;
  builder.AddListeningPort(listen_address, grpc::InsecureServerCredentials());
  builder.RegisterService(service_.get());
  server_ = builder.BuildAndStart();
  if (!server_) {
    service_.reset();
    store_.reset();
    return ErrorCode::kInternal;
  }

  running_ = true;
  spdlog::info("[ConfigServer] listening on {}", listen_address);
  return ErrorCode::kSuccess;
}

void ConfigServer::Shutdown() {
  if (server_) {
    server_->Shutdown();
    server_.reset();
  }
  service_.reset();
  store_.reset();
  running_ = false;
}

void ConfigServer::Wait() {
  if (server_) {
    server_->Wait();
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

}  // namespace qtrade::service
