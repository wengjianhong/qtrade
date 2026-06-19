/// @file      quote_types.hpp
/// @brief     行情模块枚举定义
/// @details   参考 EMT_API 行情侧类型（EMQ_EXCHANGE_TYPE / EMQ_TICKER_TYPE）。
#ifndef QTRADE_SDK_DEFINE_QUOTE_TYPES_HPP_
#define QTRADE_SDK_DEFINE_QUOTE_TYPES_HPP_

#include <cstdint>

namespace qtrade_sdk::quote {

/// 行情侧交易所类型。
enum class ExchangeType : std::uint8_t {
  kShanghai = 1,    ///< 上海证券交易所。
  kShenzhen = 2,    ///< 深圳证券交易所。
  kBeijing = 3,     ///< 北京证券交易所/股转市场。
  kShanghaiHk = 4,  ///< 沪市港股通。
  kShenzhenHk = 5,  ///< 深市港股通。
  kUnknown = 100,   ///< 未知交易所。
};

/// 证券类别。
enum class TickerType : std::uint8_t {
  kStock = 0,      ///< 股票、基金、债券、权证、质押式回购等现货品种。
  kIndex = 1,      ///< 指数。
  kOption = 2,     ///< 期权。
  kUnknown = 255,  ///< 未知证券类别。
};

}  // namespace qtrade_sdk::quote

#endif  // QTRADE_SDK_DEFINE_QUOTE_TYPES_HPP_
