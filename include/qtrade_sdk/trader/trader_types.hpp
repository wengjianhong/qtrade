/// @file      trader_types.hpp
/// @brief     交易模块枚举定义
/// @details   参考 EMT_API 交易侧类型（EMT_MARKET_TYPE / EMT_SIDE_TYPE / EMT_FUND_TRANSFER_TYPE 等）。
#ifndef QTRADE_SDK_TRADER_TYPES_HPP_
#define QTRADE_SDK_TRADER_TYPES_HPP_

#include <cstdint>

namespace qtrade_sdk::trader {

/// 通讯传输协议类型。
enum class ProtocolType : std::uint8_t {
  kTcp = 1,      ///< TCP 协议。
  kUdp = 2,      ///< UDP 协议。
  kUnknown = 0,  ///< 未指定或未知协议。
};

/// 交易侧市场类型。
enum class MarketType : std::uint8_t {
  kInit = 0,         ///< 未初始化。
  kShA = 1,          ///< 上海 A 股市场。
  kSzA = 2,          ///< 深圳 A 股市场。
  kShHkConnect = 3,  ///< 沪港通市场。
  kSzHkConnect = 4,  ///< 深港通市场。
  kBjA = 5,          ///< 北京 A 股市场。
  kUnknown = 99,     ///< 未知市场。
};

/// 委托价格类型。
enum class PriceType : std::uint8_t {
  kLimit = 1,             ///< 限价。
  kBestOrCancel = 2,      ///< 即时成交剩余撤销。
  kBest5OrLimit = 3,      ///< 最优五档即时成交剩余限价。
  kBest5OrCancel = 4,     ///< 最优五档即时成交剩余撤销。
  kAllOrCancel = 5,       ///< 全部成交或撤销。
  kForwardBest = 6,       ///< 本方最优。
  kReverseBestLimit = 7,  ///< 对方最优剩余限价。
  kLimitOrCancel = 8,     ///< 即时成交剩余限价。
  kHkLimitBidding = 9,    ///< 港股竞价限价。
  kHkLimitEnhanced = 10,  ///< 港股增强限价。
  kUnknown = 255,         ///< 未知价格类型。
};

/// 买卖方向；对应 EMT 中 EMT_SIDE_TYPE 的取值语义。
enum class SideType : std::uint8_t {
  kBuy = 1,           ///< 买入，对应 EMT_SIDE_BUY。
  kSell = 2,          ///< 卖出，对应 EMT_SIDE_SELL。
  kPurchase = 7,      ///< 申购，对应 EMT_SIDE_PURCHASE。
  kRedemption = 8,    ///< 赎回，对应 EMT_SIDE_REDEMPTION。
  kSplit = 9,         ///< 拆分，对应 EMT_SIDE_SPLIT。
  kMerge = 10,        ///< 合并，对应 EMT_SIDE_MERGE。
  kCover = 11,        ///< 备兑，对应 EMT_SIDE_COVER。
  kMarginTrade = 21,  ///< 融资买入，对应 EMT_SIDE_MARGIN_TRADE。
  kShortSell = 22,    ///< 融券卖出，对应 EMT_SIDE_SHORT_SELL。
  kRepayMargin = 23,  ///< 卖券还款，对应 EMT_SIDE_REPAY_MARGIN。
  kRepayStock = 24,   ///< 买券还券，对应 EMT_SIDE_REPAY_STOCK。
  kUnknown = 50,      ///< 未知或无效买卖方向，对应 EMT_SIDE_UNKNOWN。
};

/// 开平标识。
enum class PositionEffectType : std::uint8_t {
  kInit = 0,             ///< 初始值；现货业务通常填此值。
  kOpen = 1,             ///< 开仓。
  kClose = 2,            ///< 平仓。
  kForceClose = 3,       ///< 强平。
  kCloseToday = 4,       ///< 平今。
  kCloseYesterday = 5,   ///< 平昨。
  kForceOff = 6,         ///< 强减。
  kLocalForceClose = 7,  ///< 本地强平。
  kUnknown = 12,         ///< 未知开平标识。
};

/// 业务类型。
enum class BusinessType : std::uint8_t {
  kCash = 0,       ///< 普通股票业务。
  kIpo = 1,        ///< 新股申购业务。
  kRepo = 2,       ///< 回购业务。
  kEtf = 3,        ///< ETF 申赎业务。
  kMargin = 4,     ///< 融资融券业务。
  kAllotment = 6,  ///< 配股业务。
  kOption = 10,    ///< 期权业务。
  kExecute = 11,   ///< 行权业务。
  kFreeze = 12,    ///< 锁定解锁业务。
  kFixPrice = 14,  ///< 盘后固定定价交易。
  kUnknown = 255,  ///< 未知业务类型。
};

/// 报单状态。
enum class OrderStatusType : std::uint8_t {
  kInit = 0,                        ///< 初始化状态。
  kNew = 0,                         ///< SDK 兼容别名：新订单/初始状态。
  kAllTraded = 1,                   ///< 全部成交。
  kFilled = 1,                      ///< SDK 兼容别名：全部成交。
  kPartTradedQueueing = 2,          ///< 部分成交仍在队列中。
  kPartiallyFilled = 2,             ///< SDK 兼容别名：部分成交仍在队列中。
  kPartTradedNotQueueing = 3,       ///< 部分成交不在队列中。
  kPartiallyFilledNotQueueing = 3,  ///< SDK 兼容别名：部分成交不在队列中。
  kNoTradeQueueing = 4,             ///< 未成交仍在队列中。
  kNotTradedQueueing = 4,           ///< SDK 兼容别名：未成交仍在队列中。
  kNoTradeNotQueueing = 5,          ///< 未成交不在队列中。
  kCanceled = 6,                    ///< 已撤单。
  kRejected = 7,                    ///< 已拒绝。
  kUnknown = 255,                   ///< 未知报单状态。
};

/// 报单提交状态。
enum class OrderSubmitStatusType : std::uint8_t {
  kInit = 0,             ///< 初始状态。
  kInsertSubmitted = 1,  ///< 已提交报单。
  kInsertAccepted = 2,   ///< 报单已被接受。
  kInsertRejected = 3,   ///< 报单已被拒绝。
  kCancelSubmitted = 4,  ///< 已提交撤单。
  kCancelRejected = 5,   ///< 撤单已被拒绝。
  kCancelAccepted = 6,   ///< 撤单已被接受。
};

/// 持仓方向。
enum class PositionDirectionType : std::uint8_t {
  kLong = 1,     ///< 多头持仓。
  kShort = 2,    ///< 空头持仓。
  kNet = 3,      ///< 净持仓。
  kUnknown = 0,  ///< 未知方向。
};

/// 账户类型。
enum class AccountType : std::uint8_t {
  kStock = 0,      ///< 普通证券账户。
  kCredit = 1,     ///< 信用账户。
  kOption = 2,     ///< 期权账户。
  kUnknown = 255,  ///< 未知账户类型。
};

/// 成交类型。
enum class TradeType : std::uint8_t {
  kCommon = 0,     ///< 普通成交。
  kCancel = 1,     ///< 撤单回报。
  kUnknown = 255,  ///< 未知成交类型。
};

/// 公共流重传方式。
enum class ResumeType : std::uint8_t {
  kRestart = 0,  ///< 从本交易日开始重传。
  kResume = 1,   ///< 从上次收到的位置继续。
  kQuick = 2,    ///< 只传送登录后公共流内容。
};

/// 资金流转方向类型。
enum class FundTransferType : std::uint8_t {
  kOut = 0,        ///< 从 EMT 转出到柜台。
  kIn = 1,         ///< 从柜台转入 EMT。
  kInterOut = 2,   ///< 跨节点转出。
  kInterIn = 3,    ///< 跨节点转入。
  kUnknown = 255,  ///< 未知类型。
};

/// 资金查询类型。
enum class FundQueryType : std::uint8_t {
  kJz = 0,        ///< 查询主柜台可转资金。
  kInternal = 1,  ///< 查询一账号两中心设置时对方节点资金。
  kUnknown = 255  ///< 未知类型。
};

/// 柜台资金操作结果。
enum class FundOperStatus : std::uint8_t {
  kProcessing = 0,  ///< 处理中。
  kSuccess = 1,     ///< 成功。
  kFailed = 2,      ///< 失败。
  kSubmitted = 3,   ///< 已提交。
  kUnknown = 255,   ///< 未知状态。
};

/// 融资信用额度调拨方向类型。
enum class QuotaTransferType : std::uint8_t {
  kStkOut = 0,    ///< 融券额度划出。
  kStkIn = 1,     ///< 融券额度划入。
  kFundOut = 2,   ///< 融资额度划出。
  kFundIn = 3,    ///< 融资额度划入。
  kUnknown = 255  ///< 未知类型。
};

/// 融资融券额度调拨操作结果。
enum class QuotaOperStatus : std::uint8_t {
  kProcessing = 0,  ///< 处理中。
  kSuccess = 1,     ///< 成功。
  kFailed = 2,      ///< 失败。
  kSubmitted = 3,   ///< 已提交。
  kUnknown = 255,   ///< 未知状态。
};

}  // namespace qtrade_sdk::trader

#endif  // QTRADE_SDK_TRADER_TYPES_HPP_
