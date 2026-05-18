#ifndef QUANT_TRADING_STRUCTS_CONSTANTS_HPP_
#define QUANT_TRADING_STRUCTS_CONSTANTS_HPP_

#include <cstddef>
#include <cstdint>

namespace quant::trading::constants {

inline constexpr std::size_t kDefaultOrderBookDepth = 5;
inline constexpr std::size_t kMaxOrderIdLength = 64;
inline constexpr std::size_t kMaxInstrumentLength = 32;
inline constexpr double kPricePrecision = 1e-9;
inline constexpr std::int64_t kMaxOrderVolume = 1000000;

}  // namespace quant::trading::constants

#endif  // QUANT_TRADING_STRUCTS_CONSTANTS_HPP_
