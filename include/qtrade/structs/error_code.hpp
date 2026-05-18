#ifndef QTRADE_TRADING_STRUCTS_ERROR_CODE_HPP_
#define QTRADE_TRADING_STRUCTS_ERROR_CODE_HPP_

#include <cstdint>
#include <string>

namespace qtrade::trading {

enum class ErrorCode : std::int32_t {
  kOk = 0,
  kInvalidArgument = 1,
  kNotInitialized = 2,
  kAlreadyConnected = 3,
  kNotConnected = 4,
  kInternal = 5,
  kTimeout = 6,
  kResourceExhausted = 7,
  kRiskCheckFailed = 8,
  kOrderRejected = 9,
  kInsufficientFunds = 10,
  kInvalidInstrument = 11,
  kInvalidPrice = 12,
  kInvalidVolume = 13,
};

inline std::string ErrorCodeToString(ErrorCode code) {
  switch (code) {
    case ErrorCode::kOk: return "OK";
    case ErrorCode::kInvalidArgument: return "InvalidArgument";
    case ErrorCode::kNotInitialized: return "NotInitialized";
    case ErrorCode::kAlreadyConnected: return "AlreadyConnected";
    case ErrorCode::kNotConnected: return "NotConnected";
    case ErrorCode::kInternal: return "Internal";
    case ErrorCode::kTimeout: return "Timeout";
    case ErrorCode::kResourceExhausted: return "ResourceExhausted";
    case ErrorCode::kRiskCheckFailed: return "RiskCheckFailed";
    case ErrorCode::kOrderRejected: return "OrderRejected";
    case ErrorCode::kInsufficientFunds: return "InsufficientFunds";
    case ErrorCode::kInvalidInstrument: return "InvalidInstrument";
    case ErrorCode::kInvalidPrice: return "InvalidPrice";
    case ErrorCode::kInvalidVolume: return "InvalidVolume";
    default: return "Unknown";
  }
}

}  // namespace qtrade::trading

#endif  // QTRADE_TRADING_STRUCTS_ERROR_CODE_HPP_
