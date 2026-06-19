/// @file      execution_adapter.hpp
/// @brief     交易适配器（兼容入口，请改用 qtrade_sdk/trader/trader_api.hpp）
#ifndef QTRADE_TRADING_ADAPTER_EXECUTION_ADAPTER_HPP_
#define QTRADE_TRADING_ADAPTER_EXECUTION_ADAPTER_HPP_

#include <qtrade_sdk/trader/trader_api.hpp>

namespace qtrade::adapter {

using IExecutionAdapter = qtrade_sdk::trader::ITradeGateway;
using TradeConnectRequest = qtrade_sdk::trader::ConnectRequest;
using CancelOrderRequest = qtrade_sdk::trader::CancelOrderRequest;

}  // namespace qtrade::adapter

#endif  // QTRADE_TRADING_ADAPTER_EXECUTION_ADAPTER_HPP_
