/// @file      event_bus.cpp
/// @brief     事件总线实现
/// @details   实现事件发布订阅及异步回调分发机制
/// @author    wengjianhong
/// @date      2026-05-19
/// @copyright CC BY-NC-SA 4.0
#include "engine/event_bus/event_bus.hpp"
#include <spdlog/spdlog.h>

namespace qtrade::engine::event_bus {

EventBus::EventBus() : running_(false) {}

EventBus::~EventBus() { Stop(); }

void EventBus::Start() {
  std::lock_guard<std::mutex> lock(mutex_);
  if (running_) {
    return;
  }
  running_ = true;
  spdlog::info("[EventBus] started successfully");
}

void EventBus::Stop() {
  std::lock_guard<std::mutex> lock(mutex_);
  if (!running_) {
    return;
  }
  running_ = false;
  tick_callbacks_.clear();
  bar_callbacks_.clear();
  order_callbacks_.clear();
  trade_callbacks_.clear();
  spdlog::info("[EventBus] stopped cleanly");
}

void EventBus::SubscribeTick(TickCallback callback) {
  std::lock_guard<std::mutex> lock(mutex_);
  tick_callbacks_.push_back(callback);
}

void EventBus::SubscribeBar(BarCallback callback) {
  std::lock_guard<std::mutex> lock(mutex_);
  bar_callbacks_.push_back(callback);
}

void EventBus::SubscribeOrder(OrderCallback callback) {
  std::lock_guard<std::mutex> lock(mutex_);
  order_callbacks_.push_back(callback);
}

void EventBus::SubscribeTrade(TradeCallback callback) {
  std::lock_guard<std::mutex> lock(mutex_);
  trade_callbacks_.push_back(callback);
}

void EventBus::PublishTick(const qtrade_sdk::quote::MarketTick& tick) {
  std::vector<TickCallback> callbacks;
  {
    std::lock_guard<std::mutex> lock(mutex_);
    if (!running_) return;
    callbacks = tick_callbacks_;
  }

  for (const auto& callback : callbacks) {
    try {
      callback(tick);
    } catch (const std::exception& e) {
      spdlog::error("[EventBus] tick callback exception: {}", e.what());
    }
  }
}

void EventBus::PublishBar(const qtrade_sdk::quote::Bar& bar) {
  std::vector<BarCallback> callbacks;
  {
    std::lock_guard<std::mutex> lock(mutex_);
    if (!running_) return;
    callbacks = bar_callbacks_;
  }

  for (const auto& callback : callbacks) {
    try {
      callback(bar);
    } catch (const std::exception& e) {
      spdlog::error("[EventBus] bar callback exception: {}", e.what());
    }
  }
}

void EventBus::PublishOrder(const qtrade_sdk::trader::Order& order) {
  std::vector<OrderCallback> callbacks;
  {
    std::lock_guard<std::mutex> lock(mutex_);
    if (!running_) return;
    callbacks = order_callbacks_;
  }

  for (const auto& callback : callbacks) {
    try {
      callback(order);
    } catch (const std::exception& e) {
      spdlog::error("[EventBus] order callback exception: {}", e.what());
    }
  }
}

void EventBus::PublishTrade(const qtrade_sdk::trader::Trade& trade) {
  std::vector<TradeCallback> callbacks;
  {
    std::lock_guard<std::mutex> lock(mutex_);
    if (!running_) return;
    callbacks = trade_callbacks_;
  }

  for (const auto& callback : callbacks) {
    try {
      callback(trade);
    } catch (const std::exception& e) {
      spdlog::error("[EventBus] trade callback exception: {}", e.what());
    }
  }
}

}  // namespace qtrade::engine::event_bus
