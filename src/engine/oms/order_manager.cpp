#include <qtrade/engine/order_manager.hpp>
#include <spdlog/spdlog.h>

namespace qtrade::trading::engine::oms {

OrderManager::OrderManager() : running_(false), order_id_counter_(0) {}

OrderManager::~OrderManager() { Stop(); }

void OrderManager::Start() {
  std::lock_guard<std::mutex> lock(mutex_);
  running_ = true;
  spdlog::info("[OrderManager] started");
}

void OrderManager::Stop() {
  std::lock_guard<std::mutex> lock(mutex_);
  running_ = false;
  spdlog::info("[OrderManager] stopped");
}

ErrorCode OrderManager::SendOrder(const Order& order) {
  std::lock_guard<std::mutex> lock(mutex_);
  if (!running_) {
    return ErrorCode::kNotInitialized;
  }
  
  // 简单实现：直接保存订单
  std::string order_id;
  if (order.order_id.empty()) {
    order_id = "ORD-" + std::to_string(++order_id_counter_);
  } else {
    order_id = order.order_id;
  }
  
  Order new_order = order;
  new_order.order_id = order_id;
  new_order.status = OrderStatus::kNew;
  
  orders_[order_id] = new_order;
  spdlog::info("[OrderManager] order created: {}", order_id);
  return ErrorCode::kOk;
}

ErrorCode OrderManager::CancelOrder(const std::string& order_id) {
  std::lock_guard<std::mutex> lock(mutex_);
  auto it = orders_.find(order_id);
  if (it == orders_.end()) {
    return ErrorCode::kInvalidArgument;
  }
  it->second.status = OrderStatus::kCanceled;
  spdlog::info("[OrderManager] order canceled: {}", order_id);
  return ErrorCode::kOk;
}

Order* OrderManager::GetOrder(const std::string& order_id) {
  std::lock_guard<std::mutex> lock(mutex_);
  auto it = orders_.find(order_id);
  if (it != orders_.end()) {
    return &it->second;
  }
  return nullptr;
}

void OrderManager::UpdateOrderStatus(const std::string& order_id, OrderStatus status) {
  std::lock_guard<std::mutex> lock(mutex_);
  auto it = orders_.find(order_id);
  if (it != orders_.end()) {
    it->second.status = status;
    spdlog::info("[OrderManager] order {} status updated to {}", order_id, static_cast<int>(status));
  }
}

}  // namespace qtrade::trading::engine::oms
