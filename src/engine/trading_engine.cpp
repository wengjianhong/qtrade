/// @file      trading_engine.cpp
/// @brief     交易引擎实现
/// @details   实现交易引擎的启动、停止及各子模块协调逻辑
/// @author    wengjianhong
/// @date      2026-05-19
/// @copyright CC BY-NC-SA 4.0
#include "trading_engine.hpp"

#include <spdlog/spdlog.h>

namespace qtrade::engine {

TradingEngine::TradingEngine() : running_(false), strategy_engine_(event_bus_), market_handler_(event_bus_) {}

TradingEngine::~TradingEngine() { Stop(); }

ErrorCode TradingEngine::Init(const EngineOptions& options) {
  if (initialized_) {
    return ErrorCode::kSystemError;
  }

  options_ = options;

  if (const auto rc = log_client_.Init(options_.log_topic); rc != ErrorCode::kSuccess) {
    spdlog::warn("[TradingEngine] log_client init failed, code={}", static_cast<int>(rc));
    return rc;
  }

  const std::string monitor_endpoint =
      options_.monitor_endpoint.empty() ? "stub://local" : options_.monitor_endpoint;
  if (const auto rc = monitor_client_.Init(monitor_endpoint); rc != ErrorCode::kSuccess) {
    spdlog::warn("[TradingEngine] monitor_client init failed, code={}", static_cast<int>(rc));
    log_client_.Shutdown();
    return rc;
  }

  if (!options_.config_server_address.empty()) {
    if (const auto rc = InitConfigClient(options_); rc != ErrorCode::kSuccess) {
      spdlog::warn("[TradingEngine] config_client init failed, code={} (using local defaults)",
                   static_cast<int>(rc));
    }
  } else {
    spdlog::info("[TradingEngine] config_server_address empty, skipping config_client");
  }

  initialized_ = true;
  log_client_.Emit("info", "trading engine initialized");
  return ErrorCode::kSuccess;
}

ErrorCode TradingEngine::InitConfigClient(const EngineOptions& options) {
  client::ConfigClientOptions client_opts;
  client_opts.server_address = options.config_server_address;
  client_opts.tenant_id = options.tenant_id;
  client_opts.engine_id = options.engine_id;

  if (const auto rc = config_client_.Init(client_opts); rc != ErrorCode::kSuccess) {
    return rc;
  }

  config_client_.SetOnUpdate([this](std::string_view key, std::string_view value) {
    OnConfigUpdate(key, value);
  });

  if (const auto rc = config_client_.FetchSnapshot(); rc != ErrorCode::kSuccess) {
    spdlog::warn("[TradingEngine] GetConfig failed, continuing with local snapshot");
  }

  if (const auto rc = config_client_.StartWatch(); rc != ErrorCode::kSuccess) {
    return rc;
  }

  return ErrorCode::kSuccess;
}

void TradingEngine::OnConfigUpdate(std::string_view key, std::string_view value) {
  spdlog::info("[TradingEngine] config update: {}={}", key, value);
  log_client_.Emit("info", std::string(key) + "=" + std::string(value));
}

ErrorCode TradingEngine::Start() {
  if (!initialized_) {
    spdlog::error("[TradingEngine] Init() must be called before Start()");
    return ErrorCode::kNotInitialized;
  }
  if (running_) {
    return ErrorCode::kSystemError;
  }

  spdlog::info("[TradingEngine] starting components...");

  event_bus_.Start();
  market_handler_.Start();
  strategy_engine_.Start();
  compliance_.Start();
  order_manager_.Start();
  account_manager_.Start();
  position_manager_.Start();
  risk_manager_.Start();
  execution_manager_.Start();

  running_ = true;
  monitor_client_.Counter("engine.start", 1.0);
  log_client_.Emit("info", "trading engine started");
  spdlog::info("[TradingEngine] started successfully");
  return ErrorCode::kSuccess;
}

ErrorCode TradingEngine::Stop() {
  if (!running_) {
    if (initialized_) {
      config_client_.Shutdown();
      log_client_.Shutdown();
      monitor_client_.Shutdown();
      initialized_ = false;
    }
    return ErrorCode::kSystemError;
  }

  spdlog::info("[TradingEngine] stopping components...");

  execution_manager_.Stop();
  risk_manager_.Stop();
  position_manager_.Stop();
  account_manager_.Stop();
  order_manager_.Stop();
  compliance_.Stop();
  strategy_engine_.Stop();
  market_handler_.Stop();
  event_bus_.Stop();

  config_client_.Shutdown();
  log_client_.Shutdown();
  monitor_client_.Shutdown();

  running_ = false;
  initialized_ = false;
  spdlog::info("[TradingEngine] stopped cleanly");
  return ErrorCode::kSuccess;
}

bool TradingEngine::IsRunning() const { return running_; }

}  // namespace qtrade::engine
