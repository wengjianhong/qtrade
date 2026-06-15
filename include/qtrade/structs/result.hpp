/// @file      result.hpp
/// @brief     通用返回结果结构体
/// @details   定义带错误码、错误信息与可选数据的 Result<T> 泛型结构
/// @author    wengjianhong
/// @date      2026-05-19
/// @copyright CC BY-NC-SA 4.0
#ifndef QTRADE_TRADING_STRUCTS_RESULT_HPP_
#define QTRADE_TRADING_STRUCTS_RESULT_HPP_
#include <qtrade/error_code/code_define.hpp>

#include <optional>
#include <string>

namespace qtrade {

/// @brief 通用泛型，包含错误码、错误信息和返回数据
/// @tparam T 返回数据类型
template <typename T = void>
struct Result {
  ErrorCode error_code = ErrorCode::kSuccess;  /// 错误码
  std::string error_message;                   /// 错误信息
  std::optional<T> data = std::nullopt;        /// 返回数据
};

/// @brief void 特化，无数据返回
template <>
struct Result<void> {
  ErrorCode error_code = ErrorCode::kSuccess;  /// 错误码
  std::string error_message;                   /// 错误信息
};

}  // namespace qtrade

#endif  // QTRADE_TRADING_STRUCTS_RESULT_HPP_
