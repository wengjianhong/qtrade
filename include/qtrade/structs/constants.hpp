#ifndef QTRADE_TRADING_STRUCTS_CONSTANTS_HPP_
#define QTRADE_TRADING_STRUCTS_CONSTANTS_HPP_

#include <cstddef>
#include <cstdint>

namespace qtrade::constants {

inline constexpr std::size_t kDefaultOrderBookDepth = 5;
inline constexpr std::size_t kMaxOrderIdLength = 64;
inline constexpr std::size_t kMaxInstrumentLength = 32;
inline constexpr double kPricePrecision = 1e-9;
inline constexpr std::int64_t kMaxOrderVolume = 1000000;

}  // namespace qtrade::constants

#endif  // QTRADE_TRADING_STRUCTS_CONSTANTS_HPP_
