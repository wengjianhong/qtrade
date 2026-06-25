/// @file      market_handler.hpp
/// @brief     行情处理器
/// @details   负责行情数据的接收、分发以及转换
/// @author    wengjianhong
/// @date      2026-05-19
/// @copyright CC BY-NC-SA 4.0

#ifndef QTRADE_TRADING_ENGINE_MARKET_HANDLER_HPP_
#define QTRADE_TRADING_ENGINE_MARKET_HANDLER_HPP_

#include <qtrade_sdk/quote/quote_api.hpp>

#include "engine/event_bus/event_lanes.hpp"

#include <memory>
#include <mutex>
#include <vector>

namespace qtrade::engine::market {

class MarketHandler {
 public:
  explicit MarketHandler(event_bus::MarketEventReactor& market_event_reactor);
  ~MarketHandler();

  void Start();
  void Stop();

  void SetQuoteApi(std::unique_ptr<qtrade_sdk::quote::QuoteApi> source);
  qtrade_sdk::quote::QuoteApi* GetQuoteApi();

  void Subscribe(const std::vector<std::string>& instruments);
  void Unsubscribe(const std::vector<std::string>& instruments);

 private:
  event_bus::MarketEventReactor& market_event_reactor_;
  std::unique_ptr<qtrade_sdk::quote::QuoteApi> market_source_;
  std::mutex mutex_;
  bool running_;

  void OnTick(const qtrade_sdk::quote::MarketTick& tick);
  void OnBar(const qtrade_sdk::quote::Bar& bar);
};

}  // namespace qtrade::engine::market

#endif  // QTRADE_TRADING_ENGINE_MARKET_HANDLER_HPP_
