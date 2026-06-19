/// @file      trader_struct.hpp
/// @brief     交易相关结构体
/// @details   参考 EMT_API 交易结构体，聚合登录、报单、撤单、查询、资金、持仓与成交回报。
#ifndef QTRADE_SDK_STRUCT_TRADER_HPP_
#define QTRADE_SDK_STRUCT_TRADER_HPP_

#include <qtrade_sdk/define/trader_types.hpp>
#include <qtrade_sdk/struct/fund_struct.hpp>

#include <cstdint>
#include <string>
#include <vector>

namespace qtrade_sdk::trader {

/// 交易 API 错误响应信息，对应 EMTRI。
struct RspInfo {
  std::int32_t error_id = 0;  ///< 错误代码；0 表示无错误。
  std::string error_msg;      ///< 错误信息。
};

/// 用户终端信息，对应 EMTUserTerminalInfoReq 的必要字段。
struct UserTerminalInfo {
  std::string local_ip;          ///< 本机 IP 地址。
  std::string mac_address;       ///< 本机 MAC 地址。
  std::string harddisk_sn;       ///< 硬盘序列号。
  std::string terminal_type;     ///< 终端类型或厂商自定义标识。
  std::string software_name;     ///< 软件名称。
  std::string software_version;  ///< 软件版本。
};

/// 连接交易网关请求。
struct ConnectRequest {
  std::string broker_id;                       ///< 券商或通道 ID。
  std::string connection_string;               ///< 连接串，如 tcp://host:port。
  std::string account_id;                      ///< 资金账户。
  std::string password;                        ///< 密码或 token。
  ProtocolType protocol = ProtocolType::kTcp;  ///< 通讯协议。
  std::string local_ip;                        ///< 本地绑定 IP，可为空。
  UserTerminalInfo terminal_info;              ///< 终端采集信息。
};

/// 报单录入信息，对应 EMTOrderInsertInfo 的核心字段。
struct OrderRequest {
  std::uint32_t client_order_id = 0;                               ///< 用户自定义报单引用。
  std::uint64_t order_emt_id = 0;                                  ///< EMT 系统订单 ID，发单前无需填写。
  std::string instrument;                                          ///< 合约代码。
  MarketType market = MarketType::kUnknown;                        ///< 交易市场。
  double price = 0.0;                                              ///< 委托价格，单位元。
  double stop_price = 0.0;                                         ///< 止损价，当前保留。
  std::int64_t volume = 0;                                         ///< 委托数量，单位股/张。
  PriceType price_type = PriceType::kLimit;                        ///< 委托价格类型。
  SideType side = SideType::kBuy;                                  ///< 买卖方向。
  PositionEffectType position_effect = PositionEffectType::kInit;  ///< 开平标识，现货通常为 kInit。
  BusinessType business_type = BusinessType::kCash;                ///< 业务类型。
};

/// 撤单请求。
struct CancelOrderRequest {
  std::string order_id;            ///< SDK 内部订单 ID。
  std::uint64_t order_emt_id = 0;  ///< 待撤订单在 EMT 系统中的 ID。
  std::uint64_t session_id = 0;    ///< 登录后取得的会话 ID。
};

/// 撤单错误信息，对应 EMTOrderCancelInfo。
struct OrderCancelInfo {
  std::uint64_t order_cancel_emt_id = 0;  ///< 撤单请求在 EMT 系统中的 ID。
  std::uint64_t order_emt_id = 0;         ///< 原委托在 EMT 系统中的 ID。
  std::string order_id;                   ///< SDK 内部订单 ID。
};

/// 报单回报，对应 EMTOrderInfo/EMTQueryOrderRsp 的核心字段。
struct Order {
  std::string order_id;                      ///< SDK 内部订单 ID。
  std::uint64_t order_emt_id = 0;            ///< EMT 系统订单 ID。
  std::uint32_t client_order_id = 0;         ///< 用户自定义报单引用。
  std::string exchange_order_id;             ///< 交易所报单编号。
  std::string instrument;                    ///< 合约代码。
  MarketType market = MarketType::kUnknown;  ///< 交易市场。

  double price = 0.0;              ///< 委托价格，单位元。
  std::int64_t volume = 0;         ///< 委托数量。
  std::int64_t traded_volume = 0;  ///< 累计成交数量。
  std::int64_t left_volume = 0;    ///< 剩余数量。
  double trade_amount = 0.0;       ///< 累计成交金额。

  PriceType price_type = PriceType::kLimit;                        ///< 价格类型。
  SideType side = SideType::kBuy;                                  ///< 买卖方向。
  PositionEffectType position_effect = PositionEffectType::kInit;  ///< 开平标识。
  BusinessType business_type = BusinessType::kCash;                ///< 业务类型。

  OrderStatusType status = OrderStatusType::kInit;                     ///< 报单状态。
  OrderSubmitStatusType submit_status = OrderSubmitStatusType::kInit;  ///< 报单提交状态。

  std::int64_t insert_time = 0;  ///< 委托时间，格式 YYYYMMDDHHMMSSsss。
  std::int64_t update_time = 0;  ///< 最后更新时间。
  std::int64_t cancel_time = 0;  ///< 撤销时间。

  std::string error_message;  ///< 拒单或错误信息。
};

/// 成交回报，对应 EMTTradeReport/EMTQueryTradeRsp 的核心字段。
struct Trade {
  std::string trade_id;                      ///< 成交编号。
  std::string order_id;                      ///< SDK 内部订单 ID。
  std::uint64_t order_emt_id = 0;            ///< EMT 系统订单 ID。
  std::uint32_t client_order_id = 0;         ///< 用户自定义报单引用。
  std::string exchange_order_id;             ///< 交易所报单编号。
  std::string instrument;                    ///< 合约代码。
  MarketType market = MarketType::kUnknown;  ///< 交易市场。

  double price = 0.0;           ///< 成交价格，单位元。
  std::int64_t volume = 0;      ///< 本次成交数量。
  double trade_amount = 0.0;    ///< 本次成交金额。
  std::int64_t trade_time = 0;  ///< 成交时间，格式 YYYYMMDDHHMMSSsss。

  SideType side = SideType::kBuy;                                  ///< 买卖方向。
  PositionEffectType position_effect = PositionEffectType::kInit;  ///< 开平标识。
  TradeType trade_type = TradeType::kCommon;                       ///< 成交类型。
  BusinessType business_type = BusinessType::kCash;                ///< 业务类型。

  std::uint64_t report_index = 0;  ///< 回报序号。
};

/// 持仓查询结果，对应 EMTQueryStkPositionRsp 的核心字段。
struct Position {
  std::string instrument;                    ///< 证券代码。
  std::string instrument_name;               ///< 证券名称。
  MarketType market = MarketType::kUnknown;  ///< 交易市场。

  std::int64_t total_volume = 0;                ///< 总持仓。
  std::int64_t sellable_volume = 0;             ///< 可卖持仓。
  std::int64_t yesterday_volume = 0;            ///< 昨日持仓。
  std::int64_t purchase_redeemable_volume = 0;  ///< 今日申购/赎回数量。

  double avg_price = 0.0;       ///< 持仓成本价。
  double unrealized_pnl = 0.0;  ///< 浮动盈亏。

  PositionDirectionType direction = PositionDirectionType::kLong;  ///< 持仓方向。

  std::int64_t executable_option = 0;      ///< 可行权合约数量，期权使用。
  std::int64_t lockable_volume = 0;        ///< 可锁定标的数量。
  std::int64_t executable_underlying = 0;  ///< 可行权标的数量。
  std::int64_t locked_volume = 0;          ///< 已锁定标的数量。
  std::int64_t usable_locked_volume = 0;   ///< 可用已锁定标的数量。
};

/// 账户资金查询结果，对应 EMTQueryAssetRsp 的核心字段。
struct AccountAsset {
  std::string account_id;                          ///< 资金账户 ID。
  AccountType account_type = AccountType::kStock;  ///< 账户类型。

  double total_asset = 0.0;         ///< 总资产，单位元。
  double buying_power = 0.0;        ///< 可用资金，单位元。
  double security_asset = 0.0;      ///< 证券资产，单位元。
  double withholding_amount = 0.0;  ///< 预扣资金，单位元。

  double fund_buy_amount = 0.0;   ///< 累计买入占用，单位元。
  double fund_buy_fee = 0.0;      ///< 累计买入费用，单位元。
  double fund_sell_amount = 0.0;  ///< 累计卖出所得，单位元。
  double fund_sell_fee = 0.0;     ///< 累计卖出费用，单位元。

  double frozen_margin = 0.0;     ///< 冻结保证金，期权使用。
  double frozen_exec_cash = 0.0;  ///< 行权冻结资金，期权使用。
  double preferred_amount = 0.0;  ///< 可取资金，期权使用。

  double repay_stock_available_balance = 0.0;  ///< 融券卖出所得可用余额。
  double hkex_fund_available = 0.0;            ///< 港股通可用资金。
  double hkex_fund_frozen = 0.0;               ///< 港股通冻结资金。
};

/// 查询委托请求。
struct QueryOrdersRequest {
  std::string instrument;                              ///< 合约过滤，空表示全部。
  OrderStatusType status = OrderStatusType::kUnknown;  ///< 状态过滤。
  std::uint64_t order_emt_id = 0;                      ///< 指定 EMT 订单 ID，0 表示不限定。
};

/// 按页查询委托请求。
struct QueryOrdersByPageRequest {
  std::int64_t req_count = 0;  ///< 本次请求最大返回数量。
  std::int64_t reference = 0;  ///< 查询引用，首次查询填 0。
  std::int64_t reserved = 0;   ///< 保留字段。
};

/// 查询成交请求。
struct QueryTradesRequest {
  std::string instrument;          ///< 合约过滤，空表示全部。
  std::string order_id;            ///< SDK 订单 ID 过滤，空表示全部。
  std::uint64_t order_emt_id = 0;  ///< EMT 订单 ID 过滤，0 表示全部。
};

/// 查询持仓请求。
struct QueryPositionRequest {
  std::string instrument;                 ///< 合约过滤，空表示全部。
  MarketType market = MarketType::kInit;  ///< 市场过滤。
};

/// 查询资金请求。
struct QueryAssetRequest {
  std::string account_id;  ///< 资金账户。
};

/// 委托列表响应。
struct QueryOrdersResponse {
  std::vector<Order> orders;  ///< 委托列表。
};

/// 成交列表响应。
struct QueryTradesResponse {
  std::vector<Trade> trades;  ///< 成交列表。
};

/// 持仓列表响应。
struct QueryPositionResponse {
  std::vector<Position> positions;  ///< 持仓列表。
};

/// 资金查询响应。
struct QueryAssetResponse {
  AccountAsset asset;  ///< 账户资金信息。
};

}  // namespace qtrade_sdk::trader

#endif  // QTRADE_SDK_STRUCT_TRADER_HPP_
