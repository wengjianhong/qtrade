/// @file      constants.hpp
/// @brief     SDK 公共常量
/// @details   参考 EMT_API 的长度、方向、业务类型等常量，供行情与交易模块共用。
#ifndef QTRADE_SDK_DEFINE_CONSTANTS_HPP_
#define QTRADE_SDK_DEFINE_CONSTANTS_HPP_

#include <cstddef>
#include <cstdint>

namespace qtrade_sdk::constants {

/// 错误信息最大长度，对应 EMT_ERR_MSG_LEN。
inline constexpr std::size_t kErrorMessageLength = 124;
/// 交易日字符串长度，格式通常为 YYYYMMDD + '\0'。
inline constexpr std::size_t kTradingDayLength = 9;
/// 证券代码最大长度，对应 EMT_TICKER_LEN。
inline constexpr std::size_t kTickerLength = 16;
/// 证券名称最大长度，对应 EMT_TICKER_NAME_LEN。
inline constexpr std::size_t kTickerNameLength = 64;
/// 本地报单编号最大长度，对应 EMT_LOCAL_ORDER_LEN。
inline constexpr std::size_t kLocalOrderIdLength = 11;
/// 交易所报单编号最大长度，对应 EMT_ORDER_EXCH_LEN。
inline constexpr std::size_t kExchangeOrderIdLength = 17;
/// 成交执行编号最大长度，对应 EMT_EXEC_ID_LEN。
inline constexpr std::size_t kExecutionIdLength = 18;
/// 资金账号名称最大长度，对应 EMT_ACCOUNT_NAME_LEN。
inline constexpr std::size_t kAccountNameLength = 16;
/// IP 地址字符串最大长度，对应 EMT_INET_ADDRESS_STR_LEN。
inline constexpr std::size_t kInetAddressLength = 64;
/// 登录密码字符串最大长度，对应 EMT_ACCOUNT_PASSWORD_LEN。
inline constexpr std::size_t kAccountPasswordLength = 64;
/// L1 常用展示深度（五档）。
inline constexpr std::size_t kDefaultOrderBookDepth = 5;
/// 最大订单簿深度（十档）。
inline constexpr std::size_t kMaxOrderBookDepth = 10;
/// 买一/卖一队列最大保留笔数。
inline constexpr std::size_t kMaxLevelOneQueueDepth = 50;
/// SDK 内部订单 ID 最大长度。
inline constexpr std::size_t kMaxOrderIdLength = 64;
/// SDK 内部合约代码最大长度。
inline constexpr std::size_t kMaxInstrumentLength = 32;
/// SDK 内部合约名称最大长度。
inline constexpr std::size_t kMaxInstrumentNameLength = 64;
/// 单笔订单数量软上限，用于本地风控默认值。
inline constexpr std::int64_t kMaxOrderVolume = 1000000;
/// 价格比较精度。
inline constexpr double kPricePrecision = 1e-6;

}  // namespace qtrade_sdk::constants

#endif  // QTRADE_SDK_DEFINE_CONSTANTS_HPP_
