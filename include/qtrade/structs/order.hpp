/// @file      order.hpp
/// @brief     订单数据结构
/// @details   定义订单、成交等交易相关数据结构
/// @author    wengjianhong
/// @date      2026-05-19
/// @copyright CC BY-NC-SA 4.0
#ifndef QTRADE_TRADING_STRUCTS_ORDER_HPP_
#define QTRADE_TRADING_STRUCTS_ORDER_HPP_

#include <cstdint>
#include <string>

namespace qtrade {

/// 订单方向
enum class OrderSide : std::uint8_t {
  kBuy = 0,   /// 买入
  kSell = 1,  /// 卖出
};

/// 订单状态
enum class OrderStatus : std::uint8_t {
  kNew = 0,          /// 新订单
  kPartiallyFilled,  /// 部分成交
  kFilled,           /// 已成交
  kCanceled,         /// 已取消
  kRejected,         /// 已拒绝
};

/// 订单
struct Order {
  std::string order_id;                   /// 订单ID
  std::string instrument;                 /// 合约代码
  OrderSide side{OrderSide::kBuy};        /// 订单方向
  double price{};                         /// 订单价格
  int64_t volume{};                       /// 订单数量
  OrderStatus status{OrderStatus::kNew};  /// 订单状态
};

/// 成交
struct Trade {
  std::string trade_id;    /// 成交ID
  std::string order_id;    /// 订单ID
  std::string instrument;  /// 合约代码
  double price{};          /// 成交价格
  int64_t volume{};        /// 成交数量
};

}  // namespace qtrade

#endif  // QTRADE_TRADING_STRUCTS_ORDER_HPP_
