/// @file    constants.hpp
/// @brief   系统常量定义
/// @details 定义交易引擎使用的各类常量参数
/// @author  wengjianhong
/// @date    2026-05-19
/// @copyright CC BY-NC-SA 4.0
#ifndef QTRADE_TRADING_STRUCTS_CONSTANTS_HPP_
#define QTRADE_TRADING_STRUCTS_CONSTANTS_HPP_

#include <cstddef>
#include <cstdint>

namespace qtrade::constants {

inline constexpr std::size_t kDefaultOrderBookDepth = 5;  /// 默认订单簿深度
inline constexpr std::size_t kMaxOrderIdLength = 64;      /// 最大订单ID长度
inline constexpr std::size_t kMaxInstrumentLength = 32;   /// 最大合约代码长度
inline constexpr double kPricePrecision = 1e-9;           /// 价格精度
inline constexpr std::int64_t kMaxOrderVolume = 1000000;  /// 最大订单数量

}  // namespace qtrade::constants

#endif  // QTRADE_TRADING_STRUCTS_CONSTANTS_HPP_
