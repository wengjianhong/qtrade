/// @file      market_handler.cpp
/// @brief     行情处理器实现
/// @details   实现行情数据接收、转换与事件总线分发
/// @author    wengjianhong
/// @date      2026-05-19
/// @copyright CC BY-NC-SA 4.0

#include "engine/market/market_handler.hpp"

#include <qtrade/error_code/code_message.hpp>

#include <spdlog/spdlog.h>

namespace qtrade::engine::market {

MarketHandler::MarketHandler(event_bus::EventBus& event_bus) : event_bus_(event_bus), running_(false) {}

MarketHandler::~MarketHandler() { Stop(); }

void MarketHandler::Start() {
  std::lock_guard<std::mutex> lock(mutex_);
  if (running_) {
    return;
  }
  running_ = true;
  spdlog::info("[MarketHandler] started successfully");
}

void MarketHandler::Stop() {
  std::lock_guard<std::mutex> lock(mutex_);
  if (!running_) {
    return;
  }
  if (market_source_ && market_source_->IsConnected()) {
    market_source_->Disconnect();
  }
  running_ = false;
  spdlog::info("[MarketHandler] stopped cleanly");
}

void MarketHandler::SetMarketSource(std::unique_ptr<adapter::IMarketSource> source) {
  std::lock_guard<std::mutex> lock(mutex_);
  market_source_ = std::move(source);

  if (market_source_) {
    // 设置回调
    market_source_->SetTickCallback([this](const MarketTick& tick) { OnTick(tick); });
    market_source_->SetBarCallback([this](const Bar& bar) { OnBar(bar); });
    spdlog::info("[MarketHandler] market source set successfully");
  }
}

// 新增：获取当前行情源（用于外部初始化）
adapter::IMarketSource* MarketHandler::GetMarketSource() {
  std::lock_guard<std::mutex> lock(mutex_);
  return market_source_.get();
}

void MarketHandler::Subscribe(const std::vector<std::string>& instruments) {
  std::lock_guard<std::mutex> lock(mutex_);
  if (!market_source_ || !running_) {
    spdlog::warn("[MarketHandler] cannot subscribe: market source not ready");
    return;
  }
  auto rc = market_source_->Subscribe(instruments);
  if (rc == ErrorCode::kSuccess) {
    spdlog::info("[MarketHandler] subscribed to {} instruments", instruments.size());
  } else {
    spdlog::error("[MarketHandler] subscription failed: {}", GetErrorCodeMessage(rc));
  }
}

void MarketHandler::Unsubscribe(const std::vector<std::string>& instruments) {
  std::lock_guard<std::mutex> lock(mutex_);
  if (!market_source_ || !running_) {
    return;
  }
  market_source_->Unsubscribe(instruments);
  spdlog::info("[MarketHandler] unsubscribed from {} instruments", instruments.size());
}

void MarketHandler::OnTick(const MarketTick& tick) { event_bus_.PublishTick(tick); }

void MarketHandler::OnBar(const Bar& bar) { event_bus_.PublishBar(bar); }

}  // namespace qtrade::engine::market
