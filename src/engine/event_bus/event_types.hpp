/// @file      event_types.hpp
/// @brief     事件类型定义
/// @details   定义事件类型枚举、事件基类以及各类具体事件结构
/// @author    wengjianhong
/// @date      2026-05-19
/// @copyright CC BY-NC-SA 4.0

#ifndef QTRADE_TRADING_ENGINE_EVENT_TYPES_HPP_
#define QTRADE_TRADING_ENGINE_EVENT_TYPES_HPP_

#include <qtrade_sdk/quote/quote_struct.hpp>
#include <qtrade_sdk/trader/trader_struct.hpp>

#include <chrono>
#include <functional>

namespace qtrade::engine::event_bus {

/// @brief 事件类型枚举
enum class EventType {
  kTickData = 0,     /// 市场数据事件
  kBarData = 1,      /// K线柱数据事件
  kOrderUpdate = 2,  /// 订单更新事件
  kTradeUpdate = 3,  /// 成交更新事件
};

/// @brief 事件基类
struct Event {
  EventType type;
  std::chrono::system_clock::time_point timestamp;

  explicit Event(EventType t) : type(t), timestamp(std::chrono::system_clock::now()) {}
  virtual ~Event() = default;
};

/// @brief Tick 数据事件
struct TickEvent : public Event {
  qtrade_sdk::quote::MarketTick tick;

  explicit TickEvent(const qtrade_sdk::quote::MarketTick& t) : Event(EventType::kTickData), tick(t) {}
};

/// @brief Bar 数据事件
struct BarEvent : public Event {
  qtrade_sdk::quote::Bar bar;

  explicit BarEvent(const qtrade_sdk::quote::Bar& b) : Event(EventType::kBarData), bar(b) {}
};

/// @brief 订单更新事件
struct OrderEvent : public Event {
  qtrade_sdk::trader::Order order;

  explicit OrderEvent(const qtrade_sdk::trader::Order& o) : Event(EventType::kOrderUpdate), order(o) {}
};

/// @brief 成交更新事件
struct TradeEvent : public Event {
  qtrade_sdk::trader::Trade trade;

  explicit TradeEvent(const qtrade_sdk::trader::Trade& t) : Event(EventType::kTradeUpdate), trade(t) {}
};

/// @brief Tick 数据回调类型
using TickCallback = std::function<void(const qtrade_sdk::quote::MarketTick&)>;
/// @brief Bar 数据回调类型
using BarCallback = std::function<void(const qtrade_sdk::quote::Bar&)>;
/// @brief 订单更新回调类型
using OrderCallback = std::function<void(const qtrade_sdk::trader::Order&)>;
/// @brief 成交更新回调类型
using TradeCallback = std::function<void(const qtrade_sdk::trader::Trade&)>;

}  // namespace qtrade::engine::event_bus

#endif  // QTRADE_TRADING_ENGINE_EVENT_TYPES_HPP_
