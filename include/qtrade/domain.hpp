/// @file      domain.hpp
/// @brief     领域类型别名
/// @details   将 qtrade_sdk 交易所无关类型 re-export 到 qtrade 命名空间，供引擎/策略使用
#ifndef QTRADE_DOMAIN_HPP_
#define QTRADE_DOMAIN_HPP_

#include <qtrade_sdk/define/constants.hpp>
#include <qtrade_sdk/define/quote_types.hpp>
#include <qtrade_sdk/define/trader_types.hpp>
#include <qtrade_sdk/struct/quote_struct.hpp>
#include <qtrade_sdk/struct/trader_struct.hpp>

namespace qtrade {

namespace constants = qtrade_sdk::constants;

using qtrade_sdk::quote::Bar;
using qtrade_sdk::quote::ExchangeType;
using qtrade_sdk::quote::MarketTick;
using qtrade_sdk::quote::OriginalMarketTick;
using qtrade_sdk::quote::TickByTickTrade;
using qtrade_sdk::quote::TickerType;

using qtrade_sdk::trader::AccountAsset;
using qtrade_sdk::trader::AccountType;
using qtrade_sdk::trader::BusinessType;
using qtrade_sdk::trader::MarketType;
using qtrade_sdk::trader::Order;
using qtrade_sdk::trader::OrderRequest;
using qtrade_sdk::trader::OrderStatusType;
using qtrade_sdk::trader::OrderSubmitStatusType;
using qtrade_sdk::trader::Position;
using qtrade_sdk::trader::PositionDirectionType;
using qtrade_sdk::trader::PositionEffectType;
using qtrade_sdk::trader::PriceType;
using qtrade_sdk::trader::SideType;
using qtrade_sdk::trader::Trade;
using qtrade_sdk::trader::TradeType;

}  // namespace qtrade

#endif  // QTRADE_DOMAIN_HPP_
