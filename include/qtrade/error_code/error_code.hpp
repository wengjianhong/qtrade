#ifndef QTRADE_ERROR_CODE_ERROR_CODE_HPP_
#define QTRADE_ERROR_CODE_ERROR_CODE_HPP_

#include <qtrade/error_code/code_segment.hpp>

#include <cstdint>
#include <string>

namespace qtrade {

/// 错误码：AABBBCCC 八位整数，AA=10（系统级），BBB=模块，CCC=具体错误；0 表示成功
enum class ErrorCode : std::int32_t {
  kSuccess = 0,

  // 系统级（BBB=000）
  kInvalidArgument = MakeErrorCode(ModuleNumber::kQTradeSystem, 1),
  kNotInitialized = MakeErrorCode(ModuleNumber::kQTradeSystem, 2),
  kInternal = MakeErrorCode(ModuleNumber::kQTradeSystem, 3),
  kTimeout = MakeErrorCode(ModuleNumber::kQTradeSystem, 4),
  kResourceExhausted = MakeErrorCode(ModuleNumber::kQTradeSystem, 5),
  kNotFound = MakeErrorCode(ModuleNumber::kQTradeSystem, 6),

  // 适配器（BBB=011）
  kAlreadyConnected = MakeErrorCode(ModuleNumber::kMarketAdapter, 1),
  kNotConnected = MakeErrorCode(ModuleNumber::kMarketAdapter, 2),
};

inline std::string ErrorCodeToString(ErrorCode code) {
  switch (code) {
    case ErrorCode::kSuccess:
      return "Success";
    case ErrorCode::kInvalidArgument:
      return "InvalidArgument";
    case ErrorCode::kNotInitialized:
      return "NotInitialized";
    case ErrorCode::kAlreadyConnected:
      return "AlreadyConnected";
    case ErrorCode::kNotConnected:
      return "NotConnected";
    case ErrorCode::kInternal:
      return "Internal";
    case ErrorCode::kTimeout:
      return "Timeout";
    case ErrorCode::kResourceExhausted:
      return "ResourceExhausted";
    default:
      return "Unknown";
  }
}

}  // namespace qtrade

#endif  // QTRADE_ERROR_CODE_ERROR_CODE_HPP_
