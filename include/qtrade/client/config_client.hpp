/// @file      config_client.hpp
/// @brief     配置管理客户端
/// @details   提供配置订阅功能，支持配置变更的实时推送
/// @author    wengjianhong
/// @date      2026-05-19
/// @copyright CC BY-NC-SA 4.0
#ifndef QTRADE_TRADING_CLIENT_CONFIG_CLIENT_HPP_
#define QTRADE_TRADING_CLIENT_CONFIG_CLIENT_HPP_

#include <qtrade/error_code/code_define.hpp>

#include <functional>
#include <string_view>

namespace qtrade::client {

/// @brief 配置管理客户端类
/// @details 封装配置订阅功能，支持配置变更的实时推送通知
class ConfigClient {
 public:
  /// @brief 配置更新回调函数类型
  /// @param key 配置键
  /// @param value 配置值
  using UpdateHandler = std::function<void(std::string_view key, std::string_view value)>;

  /// @brief 初始化配置客户端
  /// @param bootstrap_address 配置服务引导地址
  /// @return ErrorCode::kSuccess 表示成功，其他表示失败
  ErrorCode Init(std::string_view bootstrap_address);

  /// @brief 关闭配置客户端
  void Shutdown();

  /// @brief 设置配置更新回调函数
  /// @param handler 回调函数
  void SetOnUpdate(UpdateHandler handler);

 private:
  UpdateHandler on_update_;  /// 配置更新回调
  bool initialized_{false};  /// 初始化标志
};

}  // namespace qtrade::client

#endif  // QTRADE_TRADING_CLIENT_CONFIG_CLIENT_HPP_
