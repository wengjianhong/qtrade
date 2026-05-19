#ifndef QTRADE_TRADING_ENGINE_ORDER_MANAGER_HPP_
#define QTRADE_TRADING_ENGINE_ORDER_MANAGER_HPP_

#include <qtrade/error_code/error_code.hpp>
#include <qtrade/structs/order.hpp>

#include <atomic>
#include <mutex>
#include <string>
#include <unordered_map>

namespace qtrade::engine::oms {

class OrderManager {
 public:
  OrderManager();
  ~OrderManager();

  void Start();
  void Stop();

  ErrorCode SendOrder(const Order& order);
  ErrorCode CancelOrder(const std::string& order_id);

  Order* GetOrder(const std::string& order_id);
  void UpdateOrderStatus(const std::string& order_id, OrderStatus status);

 private:
  std::unordered_map<std::string, Order> orders_;
  std::mutex mutex_;
  bool running_;
  std::atomic<uint64_t> order_id_counter_;
};

}  // namespace qtrade::engine::oms

#endif  // QTRADE_TRADING_ENGINE_ORDER_MANAGER_HPP_
