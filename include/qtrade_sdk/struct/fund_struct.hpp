/// @file      fund_struct.hpp
/// @brief     资金划拨相关结构体
/// @details   参考 EMT_API 的 eoms_api_fund_struct.h，定义资金划转、额度调拨与资金查询结构体。
#ifndef QTRADE_SDK_STRUCT_FUND_STRUCT_HPP_
#define QTRADE_SDK_STRUCT_FUND_STRUCT_HPP_

#include <qtrade_sdk/define/trader_types.hpp>

#include <array>
#include <cstdint>
#include <string>

namespace qtrade_sdk::trader {

/// 两地分仓信用额度划拨请求，对应 EMTQuotaTransferReq。
struct QuotaTransferRequest {
  std::uint64_t serial_id = 0;                                    ///< 额度划拨编号，发单前无需填写。
  std::string fund_account;                                       ///< 资金账户代码。
  double amount = 0.0;                                            ///< 划拨金额，单位元。
  QuotaTransferType transfer_type = QuotaTransferType::kUnknown;  ///< 额度划拨方向。
};

/// 用户资金划转请求，对应 EMTFundTransferReq。
struct FundTransferRequest {
  std::uint64_t serial_id = 0;                                  ///< 资金内转编号，发单前无需填写。
  std::string fund_account;                                     ///< 资金账户代码。
  std::string password;                                         ///< 资金账户密码。
  double amount = 0.0;                                          ///< 划拨金额，单位元。
  FundTransferType transfer_type = FundTransferType::kUnknown;  ///< 资金流转方向。
};

/// 用户资金查询请求，对应 EMTFundQueryReq。
struct FundQueryRequest {
  std::string fund_account;                            ///< 资金账户代码。
  std::string password;                                ///< 资金账户密码。
  FundQueryType query_type = FundQueryType::kUnknown;  ///< 资金查询类型。
  std::array<std::uint64_t, 4> reserved{};             ///< 预留字段。
};

/// 用户资金查询响应，对应 EMTFundQueryRsp。
struct FundQueryResponse {
  double amount = 0.0;                                 ///< 查询到的金额，单位元。
  FundQueryType query_type = FundQueryType::kUnknown;  ///< 资金查询类型。
  std::array<std::uint64_t, 4> reserved{};             ///< 预留字段。
};

/// 资金内转流水查询请求，对应 EMTQueryFundTransferLogReq。
struct QueryFundTransferLogRequest {
  std::uint64_t serial_id = 0;  ///< 资金内转编号；0 表示查询全部。
};

/// 融券额度调拨流水查询请求，对应 EMTQueryQuotaTransferLogReq。
struct QueryQuotaTransferLogRequest {
  std::uint64_t serial_id = 0;  ///< 额度划拨编号；0 表示查询全部。
};

/// 资金内转流水通知，对应 EMTFundTransferNotice。
struct FundTransferNotice {
  std::uint64_t serial_id = 0;                                  ///< 资金内转编号。
  FundTransferType transfer_type = FundTransferType::kUnknown;  ///< 内转类型。
  double amount = 0.0;                                          ///< 划拨金额，单位元。
  FundOperStatus oper_status = FundOperStatus::kUnknown;        ///< 操作结果。
  std::uint64_t transfer_time = 0;                              ///< 操作时间，格式 YYYYMMDDHHMMSSsss。
};

/// 两地分仓额度内转流水通知，对应 EMTQuotaTransferNotice。
struct QuotaTransferNotice {
  std::uint64_t serial_id = 0;                                    ///< 额度划拨编号。
  QuotaTransferType transfer_type = QuotaTransferType::kUnknown;  ///< 内转类型。
  double amount = 0.0;                                            ///< 划拨金额，单位元。
  QuotaOperStatus oper_status = QuotaOperStatus::kUnknown;        ///< 操作结果。
  std::uint64_t transfer_time = 0;                                ///< 操作时间，格式 YYYYMMDDHHMMSSsss。
};

using FundTransferAck = FundTransferNotice;
using QuotaTransferAck = QuotaTransferNotice;

}  // namespace qtrade_sdk::trader

#endif  // QTRADE_SDK_STRUCT_FUND_STRUCT_HPP_
