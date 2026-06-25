/// @file      event_bus.hpp
/// @brief     事件总线
/// @details   提供事件发布订阅机制，用于模块间解耦通信
/// @author    wengjianhong
/// @date      2026-05-19
/// @copyright CC BY-NC-SA 4.0

#ifndef QTRADE_TRADING_ENGINE_EVENT_BUS_HPP_
#define QTRADE_TRADING_ENGINE_EVENT_BUS_HPP_

#include "engine/event_bus/event_types.hpp"

#include <mutex>
#include <vector>

namespace qtrade::engine::event_bus {

/// @brief 事件总线类
/// @details 提供事件发布订阅机制，实现模块间解耦通信
class EventBus {
 public:
  /// @brief 构造函数
  EventBus();

  /// @brief 析构函数
  ~EventBus();

  /// @brief 启动事件总线
  void Start();

  /// @brief 停止事件总线
  void Stop();

  /// @brief 订阅Tick数据事件
  /// @param callback 回调函数
  void SubscribeTick(TickCallback callback);

  /// @brief 订阅Bar数据事件
  /// @param callback 回调函数
  void SubscribeBar(BarCallback callback);

  /// @brief 订阅订单更新事件
  /// @param callback 回调函数
  void SubscribeOrder(OrderCallback callback);

  /// @brief 订阅成交更新事件
  /// @param callback 回调函数
  void SubscribeTrade(TradeCallback callback);

  /// @brief 发布Tick数据事件
  /// @param tick Tick数据
  void PublishTick(const qtrade_sdk::quote::MarketTick& tick);

  /// @brief 发布Bar数据事件
  /// @param bar Bar数据
  void PublishBar(const qtrade_sdk::quote::Bar& bar);

  /// @brief 发布订单更新事件
  /// @param order 订单信息
  void PublishOrder(const qtrade_sdk::trader::Order& order);

  /// @brief 发布成交更新事件
  /// @param trade 成交信息
  void PublishTrade(const qtrade_sdk::trader::Trade& trade);

 private:
  bool running_;                                /// 运行状态标志
  std::mutex mutex_;                            /// 互斥锁
  std::vector<TickCallback> tick_callbacks_;    /// Tick回调列表
  std::vector<BarCallback> bar_callbacks_;      /// Bar回调列表
  std::vector<OrderCallback> order_callbacks_;  /// 订单回调列表
  std::vector<TradeCallback> trade_callbacks_;  /// 成交回调列表
};

}  // namespace qtrade::engine::event_bus

#endif  // QTRADE_TRADING_ENGINE_EVENT_BUS_HPP_
