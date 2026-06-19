/// @file      result.hpp
/// @brief     框架通用返回结果
#ifndef QTRADE_CORE_RESULT_HPP_
#define QTRADE_CORE_RESULT_HPP_

#include <qtrade/error_code/code_define.hpp>

#include <optional>
#include <string>

namespace qtrade {

template <typename T = void>
struct Result {
  ErrorCode error_code = ErrorCode::kSuccess;  /// 错误码
  std::string error_message;                   /// 错误描述
  std::optional<T> data = std::nullopt;        /// 返回数据
};

template <>
struct Result<void> {
  ErrorCode error_code = ErrorCode::kSuccess;
  std::string error_message;
};

}  // namespace qtrade

#endif  // QTRADE_CORE_RESULT_HPP_
