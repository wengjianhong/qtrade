/// @file      bar_data.hpp
/// @brief     Bar 数据结构
/// @details   定义周期行情数据结构（K线数据）
/// @author    wengjianhong
/// @date      2026-05-19
/// @copyright CC BY-NC-SA 4.0
#ifndef QTRADE_TRADING_STRUCTS_BAR_DATA_HPP_
#define QTRADE_TRADING_STRUCTS_BAR_DATA_HPP_

#include <chrono>
#include <cstdint>
#include <string>

namespace qtrade {

/// 行情周期数据
struct Bar {
  std::string instrument;                            /// 合约代码
  std::chrono::system_clock::time_point open_time;   /// 开盘时间
  std::chrono::system_clock::time_point close_time;  /// 收盘时间
  double open = 0.0;                                 /// 开盘价
  double high = 0.0;                                 /// 最高价
  double low = 0.0;                                  /// 最低价
  double close = 0.0;                                /// 收盘价
  int64_t volume = 0;                                /// 成交量
};

}  // namespace qtrade

#endif  // QTRADE_TRADING_STRUCTS_BAR_DATA_HPP_
