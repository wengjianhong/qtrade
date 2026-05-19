#ifndef QTRADE_TRADING_ENGINE_EVENT_BUS_HPP_
#define QTRADE_TRADING_ENGINE_EVENT_BUS_HPP_

#include <qtrade/structs/bar_data.hpp>
#include <qtrade/structs/order.hpp>
#include <qtrade/structs/tick_data.hpp>

#include <chrono>
#include <functional>
#include <memory>
#include <mutex>
#include <unordered_map>
#include <vector>

namespace qtrade::engine::event_bus {

enum class EventType {
  kTickData = 0,
  kBarData = 1,
  kOrderUpdate = 2,
  kTradeUpdate = 3,
};

struct Event {
  EventType type;
  std::chrono::system_clock::time_point timestamp;

  explicit Event(EventType t) : type(t), timestamp(std::chrono::system_clock::now()) {}
  virtual ~Event() = default;
};

struct TickEvent : public Event {
  MarketTick tick;

  explicit TickEvent(const MarketTick& t) : Event(EventType::kTickData), tick(t) {}
};

struct BarEvent : public Event {
  Bar bar;

  explicit BarEvent(const Bar& b) : Event(EventType::kBarData), bar(b) {}
};

struct OrderEvent : public Event {
  Order order;

  explicit OrderEvent(const Order& o) : Event(EventType::kOrderUpdate), order(o) {}
};

struct TradeEvent : public Event {
  Trade trade;

  explicit TradeEvent(const Trade& t) : Event(EventType::kTradeUpdate), trade(t) {}
};

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

  void SubscribeTick(TickCallback callback);
  void SubscribeBar(BarCallback callback);
  void SubscribeOrder(OrderCallback callback);
  void SubscribeTrade(TradeCallback callback);

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

}  // namespace qtrade::engine::event_bus

#endif  // QTRADE_TRADING_ENGINE_EVENT_BUS_HPP_
