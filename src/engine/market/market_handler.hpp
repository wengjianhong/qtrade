#ifndef QTRADE_TRADING_ENGINE_MARKET_HANDLER_HPP_
#define QTRADE_TRADING_ENGINE_MARKET_HANDLER_HPP_

#include <qtrade/adapter/market_source.hpp>

#include "engine/event_bus/event_bus.hpp"

#include <memory>
#include <mutex>
#include <vector>

namespace qtrade::engine::market {

class MarketHandler {
 public:
  explicit MarketHandler(event_bus::EventBus& event_bus);
  ~MarketHandler();

  void Start();
  void Stop();

  void SetMarketSource(std::unique_ptr<adapter::IMarketSource> source);
  adapter::IMarketSource* GetMarketSource();

  void Subscribe(const std::vector<std::string>& instruments);
  void Unsubscribe(const std::vector<std::string>& instruments);

 private:
  event_bus::EventBus& event_bus_;
  std::unique_ptr<adapter::IMarketSource> market_source_;
  std::mutex mutex_;
  bool running_;

  void OnTick(const MarketTick& tick);
  void OnBar(const Bar& bar);
};

}  // namespace qtrade::engine::market

#endif  // QTRADE_TRADING_ENGINE_MARKET_HANDLER_HPP_
