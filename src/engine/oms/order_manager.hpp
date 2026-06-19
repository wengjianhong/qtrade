/// @file      order_manager.hpp
/// @brief     订单管理器
/// @details   负责订单的创建、修改、撤销以及状态跟踪
/// @author    wengjianhong
/// @date      2026-05-19
/// @copyright CC BY-NC-SA 4.0
#ifndef QTRADE_TRADING_ENGINE_ORDER_MANAGER_HPP_
#define QTRADE_TRADING_ENGINE_ORDER_MANAGER_HPP_

#include <qtrade/error_code/code_define.hpp>
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

  ErrorCode SendOrder(const OrderRequest& request);
  ErrorCode CancelOrder(const std::string& order_id);

  Order* GetOrder(const std::string& order_id);
  void UpdateOrderStatus(const std::string& order_id, OrderStatusType status);

 private:
  std::unordered_map<std::string, Order> orders_;
  std::mutex mutex_;
  bool running_;
  std::atomic<uint64_t> order_id_counter_;
};

}  // namespace qtrade::engine::oms

#endif  // QTRADE_TRADING_ENGINE_ORDER_MANAGER_HPP_
