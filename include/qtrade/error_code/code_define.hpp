/// @file      code_define.hpp
/// @brief     错误码定义
/// @details   定义所有错误码的枚举类型，采用 AA BBB CCC 八位整数编码方案
/// @author    wengjianhong
/// @date      2026-05-19
/// @copyright CC BY-NC-SA 4.0
#ifndef QTRADE_ERROR_CODE_CODE_DEFINE_HPP_
#define QTRADE_ERROR_CODE_CODE_DEFINE_HPP_
#include <qtrade/error_code/code_segment.hpp>

#include <cstdint>

namespace qtrade {

/// 错误码：AABBBCCC 八位整数，AA=10（系统级），BBB=模块，CCC=具体错误；0 表示成功
enum class ErrorCode : int32_t {
  kSuccess = 0,

  // 系统层（BBB=000）
  kSystemError = MakeErrorCode(ModuleNumber::kQTradeSystem, 1),
  kNotInitialized = MakeErrorCode(ModuleNumber::kQTradeSystem, 2),
  kInternal = MakeErrorCode(ModuleNumber::kQTradeSystem, 3),
  kTimeout = MakeErrorCode(ModuleNumber::kQTradeSystem, 4),
  kResourceExhausted = MakeErrorCode(ModuleNumber::kQTradeSystem, 5),
  kNotFound = MakeErrorCode(ModuleNumber::kQTradeSystem, 6),

  // 适配器层（BBB=011）
  kAlreadyConnected = MakeErrorCode(ModuleNumber::kMarketAdapter, 1),
  kNotConnected = MakeErrorCode(ModuleNumber::kMarketAdapter, 2),
};

}  // namespace qtrade

#endif  // QTRADE_ERROR_CODE_CODE_DEFINE_HPP_
