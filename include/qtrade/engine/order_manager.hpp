#ifndef QUANT_TRADING_ENGINE_ORDER_MANAGER_HPP
#define QUANT_TRADING_ENGINE_ORDER_MANAGER_HPP

#include <qtrade/structs/error_code.hpp>
#include <qtrade/structs/order.hpp>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>
#include <atomic>

namespace quant::trading::engine::oms {

class OrderManager {
 public:
  OrderManager();
  ~OrderManager();
  
  void Start();
  void Stop();
  
  // 发送订单
  ErrorCode SendOrder(const Order& order);
  
  // 取消订单
  ErrorCode CancelOrder(const std::string& order_id);
  
  // 获取订单
  Order* GetOrder(const std::string& order_id);
  
  // 更新订单状态
  void UpdateOrderStatus(const std::string& order_id, OrderStatus status);

 private:
  std::unordered_map<std::string, Order> orders_;
  std::mutex mutex_;
  bool running_;
  std::atomic<uint64_t> order_id_counter_;
};

}  // namespace quant::trading::engine::oms

#endif  // QUANT_TRADING_ENGINE_ORDER_MANAGER_HPP_
