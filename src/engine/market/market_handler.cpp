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

MarketHandler::MarketHandler(event_bus::MarketEventReactor& market_event_reactor)
    : market_event_reactor_(market_event_reactor), running_(false) {}

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

void MarketHandler::SetQuoteApi(std::unique_ptr<qtrade_sdk::quote::QuoteApi> source) {
  std::lock_guard<std::mutex> lock(mutex_);
  market_source_ = std::move(source);

  if (market_source_) {
    // 设置回调
    market_source_->SetTickCallback([this](const qtrade_sdk::quote::MarketTick& tick) { OnTick(tick); });
    market_source_->SetBarCallback([this](const qtrade_sdk::quote::Bar& bar) { OnBar(bar); });
    spdlog::info("[MarketHandler] market source set successfully");
  }
}

// 新增：获取当前行情源（用于外部初始化）
qtrade_sdk::quote::QuoteApi* MarketHandler::GetQuoteApi() {
  std::lock_guard<std::mutex> lock(mutex_);
  return market_source_.get();
}

void MarketHandler::Subscribe(const std::vector<std::string>& instruments) {
  std::lock_guard<std::mutex> lock(mutex_);
  if (!market_source_ || !running_) {
    spdlog::warn("[MarketHandler] cannot subscribe: market source not ready");
    return;
  }
  auto rc = market_source_->Subscribe({instruments});
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
  market_source_->Unsubscribe({instruments});
  spdlog::info("[MarketHandler] unsubscribed from {} instruments", instruments.size());
}

void MarketHandler::OnTick(const qtrade_sdk::quote::MarketTick& tick) { market_event_reactor_.PublishTick(tick); }

void MarketHandler::OnBar(const qtrade_sdk::quote::Bar& bar) { market_event_reactor_.PublishBar(bar); }

}  // namespace qtrade::engine::market
