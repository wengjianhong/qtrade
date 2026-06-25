/// @file      result.hpp
/// @brief     框架通用返回结果
#ifndef QTRADE_STRUCTS_RESULT_HPP_
#define QTRADE_STRUCTS_RESULT_HPP_

#include <qtrade/error_code/error_codes.hpp>

#include <optional>
#include <string>
#include <vector>

namespace qtrade {

template <typename T>
struct Result {
  ErrorCode error_code = ErrorCode::kSuccess;                                 /// 错误码
  std::string error_message;                                                  /// 错误信息
  std::optional<T> data = std::nullopt;                                       /// 返回数据
  std::optional<std::vector<std::string>> error_message_args = std::nullopt;  /// 错误信息参数
};

template <>
struct Result<void> {
  ErrorCode error_code = ErrorCode::kSuccess;                                 /// 错误码
  std::string error_message;                                                  /// 错误信息
  std::optional<std::vector<std::string>> error_message_args = std::nullopt;  /// 错误信息参数
};

}  // namespace qtrade

#endif  // QTRADE_STRUCTS_RESULT_HPP_
