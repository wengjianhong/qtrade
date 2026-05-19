#include "engine/strategy/strategy_engine.hpp"
#include <spdlog/spdlog.h>

namespace qtrade::engine::strategy {

StrategyEngine::StrategyEngine(event_bus::EventBus& event_bus) : event_bus_(event_bus), running_(false) {}

StrategyEngine::~StrategyEngine() { Stop(); }

void StrategyEngine::Start() {
  std::lock_guard<std::mutex> lock(mutex_);
  if (running_) {
    return;
  }
  running_ = true;

  // 订阅事件总线
  event_bus_.SubscribeTick([this](const MarketTick& tick) { OnTickEvent(tick); });
  event_bus_.SubscribeBar([this](const Bar& bar) { OnBarEvent(bar); });
  event_bus_.SubscribeOrder([this](const Order& order) { OnOrderEvent(order); });
  event_bus_.SubscribeTrade([this](const Trade& trade) { OnTradeEvent(trade); });

  // 启动所有策略
  for (auto& strategy : strategies_) {
    strategy->Start();
  }

  spdlog::info("[StrategyEngine] started successfully with {} strategies", strategies_.size());
}

void StrategyEngine::Stop() {
  std::lock_guard<std::mutex> lock(mutex_);
  if (!running_) {
    return;
  }

  // 停止所有策略
  for (auto& strategy : strategies_) {
    strategy->Stop();
  }

  running_ = false;
  spdlog::info("[StrategyEngine] stopped cleanly");
}

void StrategyEngine::RegisterStrategy(std::unique_ptr<qtrade::strategy::IStrategy> strategy) {
  std::lock_guard<std::mutex> lock(mutex_);
  strategies_.push_back(std::move(strategy));
  spdlog::info("[StrategyEngine] registered new strategy");
}

void StrategyEngine::SetOrderSender(OrderSender sender) {
  std::lock_guard<std::mutex> lock(mutex_);
  order_sender_ = std::move(sender);
}

void StrategyEngine::OnTickEvent(const MarketTick& tick) {
  std::lock_guard<std::mutex> lock(mutex_);
  for (auto& strategy : strategies_) {
    try {
      strategy->OnTick(tick);
    } catch (const std::exception& e) {
      spdlog::error("[StrategyEngine] strategy OnTick exception: {}", e.what());
    }
  }
}

void StrategyEngine::OnBarEvent(const Bar& bar) {
  std::lock_guard<std::mutex> lock(mutex_);
  for (auto& strategy : strategies_) {
    try {
      strategy->OnBar(bar);
    } catch (const std::exception& e) {
      spdlog::error("[StrategyEngine] strategy OnBar exception: {}", e.what());
    }
  }
}

void StrategyEngine::OnOrderEvent(const Order& order) {
  std::lock_guard<std::mutex> lock(mutex_);
  for (auto& strategy : strategies_) {
    try {
      strategy->OnOrder(order);
    } catch (const std::exception& e) {
      spdlog::error("[StrategyEngine] strategy OnOrder exception: {}", e.what());
    }
  }
}

void StrategyEngine::OnTradeEvent(const Trade& trade) {
  std::lock_guard<std::mutex> lock(mutex_);
  for (auto& strategy : strategies_) {
    try {
      strategy->OnTrade(trade);
    } catch (const std::exception& e) {
      spdlog::error("[StrategyEngine] strategy OnTrade exception: {}", e.what());
    }
  }
}

}  // namespace qtrade::engine::strategy
