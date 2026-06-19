/// @file      market_source.hpp
/// @brief     行情适配器（兼容入口，请改用 qtrade_sdk/quote/quote_api.hpp）
#ifndef QTRADE_TRADING_ADAPTER_MARKET_SOURCE_HPP_
#define QTRADE_TRADING_ADAPTER_MARKET_SOURCE_HPP_

#include <qtrade_sdk/quote/quote_api.hpp>

namespace qtrade::adapter {

using IMarketSource = qtrade_sdk::quote::IMarketSource;
using MarketConnectRequest = qtrade_sdk::quote::ConnectRequest;
using MarketSubscribeRequest = qtrade_sdk::quote::SubscribeRequest;
using MarketUnsubscribeRequest = qtrade_sdk::quote::UnsubscribeRequest;

inline std::unique_ptr<IMarketSource> CreateMockMarketSource() {
  return qtrade_sdk::quote::CreateMockMarketSource();
}

}  // namespace qtrade::adapter

#endif  // QTRADE_TRADING_ADAPTER_MARKET_SOURCE_HPP_
