/// @file      tick_data.hpp
/// @brief     Tick 数据结构
/// @details   定义标准化 Tick 数据结构，用于进程内热路径数据传递
/// @author    wengjianhong
/// @date      2026-05-19
/// @copyright CC BY-NC-SA 4.0
#ifndef QTRADE_TRADING_STRUCTS_TICK_DATA_HPP_
#define QTRADE_TRADING_STRUCTS_TICK_DATA_HPP_

#include <chrono>
#include <cstdint>
#include <string>

namespace qtrade {

// 标准化 Tick（进程内热路径传递）
struct MarketTick {
  std::string instrument;                           /// 合约代码
  std::chrono::system_clock::time_point timestamp;  /// 时间戳
  double last_price = 0.0;                          /// 最新价
  int64_t volume = 0;                               /// 成交量
  double open_interest = 0.0;                       /// 持仓量
  double open_price = 0.0;                          /// 开盘价
  double high_price = 0.0;                          /// 最高价
  double low_price = 0.0;                           /// 最低价
  double pre_close_price = 0.0;                     /// 昨收价
  double bid_price[5] = {0.0};                      /// 买一价
  int32_t bid_volume[5] = {0};                      /// 买一量
  double ask_price[5] = {0.0};                      /// 卖一价
  int32_t ask_volume[5] = {0};                      /// 卖一量
};

/// 原始行情Tick
struct OriginalMarketTick {
  std::string data_source;  /// 数据源
  std::string raw_data;     /// 原始数据
};

}  // namespace qtrade

#endif  // QTRADE_TRADING_STRUCTS_TICK_DATA_HPP_
