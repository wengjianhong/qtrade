/// @file      quote_struct.hpp
/// @brief     行情相关结构体
/// @details   参考 EMT_API 的行情结构体，聚合连接、订阅、快照、指数、静态信息与查询响应。
#ifndef QTRADE_SDK_QUOTE_STRUCT_HPP_
#define QTRADE_SDK_QUOTE_STRUCT_HPP_

#include <qtrade_sdk/constants/constants.hpp>
#include <qtrade_sdk/quote/quote_types.hpp>
#include <qtrade_sdk/trader/trader_types.hpp>

#include <array>
#include <cstdint>
#include <string>
#include <vector>

namespace qtrade_sdk::quote {

/// API 错误响应信息，对应 EMTRspInfoStruct。
struct RspInfo {
  std::int32_t error_id = 0;  ///< 错误代码；0 表示无错误。
  std::string error_msg;      ///< 错误信息文本，长度参考 constants::kErrorMessageLength。
};

/// 单个证券代码与交易所组合，对应 EMTSpecificTickerStruct。
struct SpecificTicker {
  std::string ticker;                                 ///< 证券代码，不包含空格。
  ExchangeType exchange_id = ExchangeType::kUnknown;  ///< 证券所属交易所。
};

/// 行情连接请求。
struct ConnectRequest {
  std::string name;               ///< 行情源名称，用于日志和监控区分。
  std::string connection_string;  ///< 连接串，如 tcp://host:port。
  std::string user;               ///< 登录用户名。
  std::string password;           ///< 登录密码。
};

/// 订阅行情请求。
struct SubscribeRequest {
  std::vector<std::string> instruments;  ///< 合约代码列表；为空时由接口语义决定是否订阅全市场。
  ExchangeType exchange = ExchangeType::kUnknown;  ///< 交易所过滤条件。
};

/// 取消订阅行情请求。
struct UnsubscribeRequest {
  std::vector<std::string> instruments;            ///< 合约代码列表。
  ExchangeType exchange = ExchangeType::kUnknown;  ///< 交易所过滤条件。
};

/// 查询最新快照请求。
struct QuerySnapshotRequest {
  std::vector<std::string> instruments;             ///< 合约列表，空表示查询指定交易所全市场。
  TickerType instrument_type = TickerType::kStock;  ///< 证券类别。
  ExchangeType exchange = ExchangeType::kUnknown;   ///< 交易所代码。
};

/// K 线 / Bar 数据。
struct Bar {
  std::string instrument;                          ///< 合约代码。
  ExchangeType exchange = ExchangeType::kUnknown;  ///< 交易所。
  std::int64_t open_time = 0;                      ///< 起始时间，格式 YYYYMMDDHHMMSSsss。
  std::int64_t close_time = 0;                     ///< 结束时间，格式 YYYYMMDDHHMMSSsss。
  double open = 0.0;                               ///< 开盘价，单位元。
  double high = 0.0;                               ///< 最高价，单位元。
  double low = 0.0;                                ///< 最低价，单位元。
  double close = 0.0;                              ///< 收盘价，单位元。
  std::int64_t volume = 0;                         ///< 成交量，单位股/张。
  double turnover = 0.0;                           ///< 成交金额，单位元。
  double avg_price = 0.0;                          ///< 均价，单位元。
  std::int64_t trades_count = 0;                   ///< 成交笔数。
  double open_interest = 0.0;                      ///< 持仓量，期货/期权品种使用。
};

/// 快照行情，对应 EMTMarketDataStruct 的核心字段。
struct MarketTick {
  std::string instrument;                           ///< 合约代码。
  ExchangeType exchange = ExchangeType::kUnknown;   ///< 交易所。
  TickerType instrument_type = TickerType::kStock;  ///< 证券类别。
  std::int64_t data_time = 0;                       ///< 行情时间，格式 YYYYMMDDHHMMSSssss。

  double last_price = 0.0;         ///< 最新价，单位元。
  double pre_close_price = 0.0;    ///< 昨收价，单位元。
  double open_price = 0.0;         ///< 开盘价，单位元。
  double high_price = 0.0;         ///< 最高价，单位元。
  double low_price = 0.0;          ///< 最低价，单位元。
  double close_price = 0.0;        ///< 收盘价，单位元。
  double upper_limit_price = 0.0;  ///< 涨停价，单位元。
  double lower_limit_price = 0.0;  ///< 跌停价，单位元。

  std::int64_t volume = 0;        ///< 总成交量，单位股/张。
  std::int64_t trades_count = 0;  ///< 成交笔数。
  double turnover = 0.0;          ///< 总成交金额，单位元。
  double avg_price = 0.0;         ///< 当日均价，单位元。
  double open_interest = 0.0;     ///< 持仓量，期货/期权品种使用。

  std::array<double, constants::kMaxOrderBookDepth> bid_price{};         ///< 申买价，下标 0 为买一。
  std::array<std::int64_t, constants::kMaxOrderBookDepth> bid_volume{};  ///< 申买量，下标 0 为买一。
  std::array<double, constants::kMaxOrderBookDepth> ask_price{};         ///< 申卖价，下标 0 为卖一。
  std::array<std::int64_t, constants::kMaxOrderBookDepth> ask_volume{};  ///< 申卖量，下标 0 为卖一。

  std::int64_t total_bid_volume = 0;  ///< 委托买入总量。
  std::int64_t total_ask_volume = 0;  ///< 委托卖出总量。
  double ma_bid_price = 0.0;          ///< 加权平均委买价。
  double ma_ask_price = 0.0;          ///< 加权平均委卖价。

  std::string ticker_status;  ///< 交易状态说明。
};

/// 指数行情，对应 EMTIndexDataStruct 的核心字段。
struct IndexData {
  std::string ticker;                                 ///< 指数代码。
  ExchangeType exchange_id = ExchangeType::kUnknown;  ///< 交易所。
  std::int64_t data_time = 0;                         ///< 行情时间，格式 YYYYMMDDHHMMSSssss。
  double last_price = 0.0;                            ///< 最新指数点位。
  double pre_close_price = 0.0;                       ///< 昨收指数点位。
  double open_price = 0.0;                            ///< 开盘指数点位。
  double high_price = 0.0;                            ///< 最高指数点位。
  double low_price = 0.0;                             ///< 最低指数点位。
  std::int64_t volume = 0;                            ///< 成交量。
  double turnover = 0.0;                              ///< 成交金额。
};

/// 逐笔成交。
struct TickByTickTrade {
  std::string instrument;                          ///< 合约代码。
  ExchangeType exchange = ExchangeType::kUnknown;  ///< 交易所。
  std::int64_t data_time = 0;                      ///< 成交时间，格式 YYYYMMDDHHMMSSsss。
  double price = 0.0;                              ///< 成交价格，单位元。
  std::int64_t volume = 0;                         ///< 成交数量。
  double turnover = 0.0;                           ///< 成交金额，单位元。
  trader::SideType side = trader::SideType::kBuy;  ///< 内外盘方向。
  std::int64_t sequence = 0;                       ///< 逐笔序号。
};

/// 原始行情报文，主要用于适配器调试和审计留痕。
struct OriginalMarketTick {
  std::string data_source;  ///< 数据源标识。
  std::string raw_data;     ///< 原始报文内容。
};

/// 合约部分静态信息，对应 EMTQuoteStaticInfo。
struct QuoteStaticInfo {
  std::string ticker;                                 ///< 合约代码。
  std::string ticker_name;                            ///< 合约名称。
  ExchangeType exchange_id = ExchangeType::kUnknown;  ///< 交易所。
  TickerType ticker_type = TickerType::kUnknown;      ///< 合约类型。
  double pre_close_price = 0.0;                       ///< 昨收价。
  double upper_limit_price = 0.0;                     ///< 涨停价。
  double lower_limit_price = 0.0;                     ///< 跌停价。
  std::int64_t price_tick = 0;                        ///< 最小变动价位，按交易所原始精度保存。
};

/// 合约完整静态信息，对应 EMTQuoteFullInfo 的常用字段。
struct QuoteFullInfo {
  QuoteStaticInfo static_info;       ///< 基础静态信息。
  std::int64_t buy_qty_unit = 0;     ///< 买入数量单位。
  std::int64_t sell_qty_unit = 0;    ///< 卖出数量单位。
  std::int64_t volume_multiple = 0;  ///< 合约乘数，现货通常为 1。
  double price_tick = 0.0;           ///< 最小价格变动单位。
  bool is_registration = false;      ///< 是否注册制证券。
  bool is_vie = false;               ///< 是否 VIE 架构证券。
  bool is_noprofit = false;          ///< 是否尚未盈利证券。
};

/// 最新价查询结果，对应 EMTTickerPriceInfo。
struct TickerPriceInfo {
  std::string ticker;                                 ///< 合约代码。
  ExchangeType exchange_id = ExchangeType::kUnknown;  ///< 交易所。
  double last_price = 0.0;                            ///< 最新价。
};

/// 深市逐笔回补响应，对应 EMTRebuildRespData 的关键字段。
struct RebuildRespData {
  std::uint32_t channel_no = 0;  ///< 回补通道号。
  std::uint64_t begin_seq = 0;   ///< 回补起始序号。
  std::uint64_t end_seq = 0;     ///< 回补结束序号。
  std::uint64_t request_id = 0;  ///< 请求编号。
  bool finished = false;         ///< 当前回补范围是否结束。
};

/// 查询快照响应。
struct QuerySnapshotResponse {
  std::vector<MarketTick> ticks;  ///< 快照列表。
};

}  // namespace qtrade_sdk::quote

#endif  // QTRADE_SDK_QUOTE_STRUCT_HPP_
