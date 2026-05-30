/// @file      code_message.hpp
/// @brief     错误码描述映射表
/// @author    wengjianhong
/// @date      2026-05-19
/// @copyright CC BY-NC-SA 4.0
#ifndef QTRADE_ERROR_CODE_CODE_MESSAGE_HPP_
#define QTRADE_ERROR_CODE_CODE_MESSAGE_HPP_
#include <qtrade/error_code/code_define.hpp>

#include <cstdint>
#include <string_view>
#include <unordered_map>

namespace qtrade {

enum class ErrorCode : int32_t;

/// @brief   获取错误码描述
/// @param code 错误码
/// @return 错误码对应的字符串描述
const std::string_view GetErrorCodeMessage(ErrorCode code);

/// @brief   错误码描述
const std::unordered_map<ErrorCode, const std::string_view> kErrorCodeMessages = {
  {ErrorCode::kSuccess, "Success"},

  /// 系统层（BBB=000~099）
  /// 行情适配器模块（错误码范围=10100000~10100999）
  {ErrorCode::kSystemError, "SystemError"},
  {ErrorCode::kNotInitialized, "NotInitialized"},
  {ErrorCode::kInternal, "Internal"},
  {ErrorCode::kTimeout, "Timeout"},
  {ErrorCode::kResourceExhausted, "ResourceExhausted"},
  {ErrorCode::kNotFound, "NotFound"},

  /// 适配器层（BBB=100~199）
  /// 行情适配器模块（错误码范围=10100000~10100999）
  {ErrorCode::kAlreadyConnected, "AlreadyConnected"},
  {ErrorCode::kNotConnected, "NotConnected"},
};

}  // namespace qtrade

#endif  // QTRADE_ERROR_CODE_CODE_MESSAGE_HPP_