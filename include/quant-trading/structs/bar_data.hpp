#ifndef QUANT_TRADING_STRUCTS_BAR_DATA_HPP_
#define QUANT_TRADING_STRUCTS_BAR_DATA_HPP_

#include <chrono>
#include <cstdint>
#include <string>

namespace quant::trading {

struct Bar {
  std::string instrument;
  std::chrono::system_clock::time_point open_time;
  std::chrono::system_clock::time_point close_time;
  double open{};
  double high{};
  double low{};
  double close{};
  int64_t volume{};
};

}  // namespace quant::trading

#endif  // QUANT_TRADING_STRUCTS_BAR_DATA_HPP_
