#ifndef QTRADE_TRADING_ENGINE_EVENT_BUS_HPP_
#define QTRADE_TRADING_ENGINE_EVENT_BUS_HPP_

#include <qtrade/structs/tick_data.hpp>
#include <qtrade/structs/bar_data.hpp>
#include <qtrade/structs/order.hpp>
#include <functional>
#include <memory>
#include <mutex>
#include <unordered_map>
#include <vector>

namespace qtrade::trading::engine::event_bus {

// 事件类型枚举
enum class EventType {
  kTickData = 0,
  kBarData = 1,
  kOrderUpdate = 2,
  kTradeUpdate = 3,
};

// 事件基类
struct Event {
  EventType type;
  std::chrono::system_clock::time_point timestamp;

  explicit Event(EventType t) 
      : type(t), timestamp(std::chrono::system_clock::now()) {}
  virtual ~Event() = default;
};

// Tick数据事件
struct TickEvent : public Event {
  MarketTick tick;

  explicit TickEvent(const MarketTick& t) 
      : Event(EventType::kTickData), tick(t) {}
};

// Bar数据事件
struct BarEvent : public Event {
  Bar bar;

  explicit BarEvent(const Bar& b) 
      : Event(EventType::kBarData), bar(b) {}
};

// 订单更新事件
struct OrderEvent : public Event {
  Order order;

  explicit OrderEvent(const Order& o) 
      : Event(EventType::kOrderUpdate), order(o) {}
};

// 成交更新事件
struct TradeEvent : public Event {
  Trade trade;

  explicit TradeEvent(const Trade& t) 
      : Event(EventType::kTradeUpdate), trade(t) {}
};

// 事件回调类型
using TickCallback = std::function<void(const MarketTick&)>;
using BarCallback = std::function<void(const Bar&)>;
using OrderCallback = std::function<void(const Order&)>;
using TradeCallback = std::function<void(const Trade&)>;

class EventBus {
 public:
  EventBus();
  ~EventBus();

  void Start();
  void Stop();

  // 订阅事件
  void SubscribeTick(TickCallback callback);
  void SubscribeBar(BarCallback callback);
  void SubscribeOrder(OrderCallback callback);
  void SubscribeTrade(TradeCallback callback);

  // 发布事件
  void PublishTick(const MarketTick& tick);
  void PublishBar(const Bar& bar);
  void PublishOrder(const Order& order);
  void PublishTrade(const Trade& trade);

 private:
  bool running_;
  std::mutex mutex_;

  std::vector<TickCallback> tick_callbacks_;
  std::vector<BarCallback> bar_callbacks_;
  std::vector<OrderCallback> order_callbacks_;
  std::vector<TradeCallback> trade_callbacks_;
};

}  // namespace qtrade::trading::engine::event_bus

#endif  // QTRADE_TRADING_ENGINE_EVENT_BUS_HPP_
