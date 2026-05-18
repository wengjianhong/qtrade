#include "example_strategy.hpp"
#include <spdlog/spdlog.h>
#include <cmath>

namespace qtrade::trading::demo {

ExampleStrategy::ExampleStrategy() 
    : running_(false), last_price_(0.0), position_(0) {}

ExampleStrategy::~ExampleStrategy() { Stop(); }

ErrorCode ExampleStrategy::Init(const strategy::StrategyConfig& config) {
  spdlog::info("[ExampleStrategy] init with config: {}", config.name);
  return ErrorCode::kOk;
}

ErrorCode ExampleStrategy::Start() {
  running_ = true;
  spdlog::info("[ExampleStrategy] started");
  return ErrorCode::kOk;
}

void ExampleStrategy::Pause() {
  spdlog::info("[ExampleStrategy] paused");
}

void ExampleStrategy::Resume() {
  spdlog::info("[ExampleStrategy] resumed");
}

void ExampleStrategy::Stop() {
  running_ = false;
  spdlog::info("[ExampleStrategy] stopped");
}

void ExampleStrategy::OnTick(const MarketTick& tick) {
  if (!running_) return;
  
  // 简单的价格变化逻辑
  double price_change = 0.0;
  if (last_price_ > 0) {
    price_change = (tick.last_price - last_price_) / last_price_;
  }
  
  last_price_ = tick.last_price;
  
  // 如果价格变化超过 0.5%，发送订单
  if (std::fabs(price_change) > 0.005 && order_sender_) {
    Order order;
    order.instrument = tick.instrument;
    order.price = tick.last_price;
    order.volume = 1;
    
    // 根据价格变化方向决定买卖
    if (price_change > 0 && position_ <= 0) {
      order.side = OrderSide::kBuy;
      spdlog::info("[ExampleStrategy] buy signal on {} at {}", tick.instrument, tick.last_price);
      order_sender_(order);
      position_ = 1;
    } else if (price_change < 0 && position_ >= 0) {
      order.side = OrderSide::kSell;
      spdlog::info("[ExampleStrategy] sell signal on {} at {}", tick.instrument, tick.last_price);
      order_sender_(order);
      position_ = -1;
    }
  }
}

void ExampleStrategy::OnBar(const Bar& bar) {
  if (!running_) return;
  spdlog::info("[ExampleStrategy] bar received: {} open={}, close={}", 
               bar.instrument, bar.open, bar.close);
}

void ExampleStrategy::OnOrder(const Order& order) {
  spdlog::info("[ExampleStrategy] order update: {} status={}", 
               order.order_id, static_cast<int>(order.status));
}

void ExampleStrategy::OnTrade(const Trade& trade) {
  spdlog::info("[ExampleStrategy] trade: {} price={}, volume={}", 
               trade.instrument, trade.price, trade.volume);
}

ErrorCode ExampleStrategy::SendSignal(const strategy::Signal& signal) {
  spdlog::info("[ExampleStrategy] signal: {} direction={}, strength={}", 
               signal.instrument, signal.direction, signal.strength);
  
  if (order_sender_) {
    Order order;
    order.instrument = signal.instrument;
    order.side = signal.direction > 0 ? OrderSide::kBuy : OrderSide::kSell;
    order.volume = static_cast<int64_t>(signal.strength);
    return order_sender_(order);
  }
  
  return ErrorCode::kOk;
}

std::string ExampleStrategy::GetParameter(const std::string& key) const {
  (void)key;
  return {};
}

ErrorCode ExampleStrategy::SetParameter(const std::string& key, const std::string& value) {
  (void)key;
  (void)value;
  return ErrorCode::kOk;
}

void ExampleStrategy::SetOrderSender(OrderSender sender) {
  order_sender_ = std::move(sender);
}

std::unique_ptr<strategy::IStrategy> CreateExampleStrategy() {
  return std::make_unique<ExampleStrategy>();
}

}  // namespace qtrade::trading::demo

