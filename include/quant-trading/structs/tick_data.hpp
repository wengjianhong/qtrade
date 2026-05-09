#ifndef QUANT_TRADING_STRUCTS_TICK_DATA_HPP_
#define QUANT_TRADING_STRUCTS_TICK_DATA_HPP_

#include <chrono>
#include <cstdint>
#include <string>

namespace quant::trading {

// 标准化 Tick（进程内热路径传递）
struct MarketTick {
  std::string instrument;
  std::chrono::system_clock::time_point timestamp;
  double last_price{};
  int64_t volume{};
  double open_interest{};
  double bid_price[5]{};
  int32_t bid_volume[5]{};
  double ask_price[5]{};
  int32_t ask_volume[5]{};
  double open_price{};
  double high_price{};
  double low_price{};
  double pre_close_price{};
};

struct OriginalMarketTick {
  std::string data_source;
  std::string raw_data;
};

}  // namespace quant::trading

#endif  // QUANT_TRADING_STRUCTS_TICK_DATA_HPP_
