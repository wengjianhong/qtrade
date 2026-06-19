/// @file      config_client.cpp
/// @brief     配置管理客户端实现
/// @details   实现配置订阅及变更推送通知
/// @author    wengjianhong
/// @date      2026-05-19
/// @copyright CC BY-NC-SA 4.0
#include <qtrade/client/config_client.hpp>

namespace qtrade::client {

ErrorCode ConfigClient::Init(std::string_view bootstrap_address) {
  (void)bootstrap_address;  // 预留参数：配置服务地址
  initialized_ = true;
  return ErrorCode::kSuccess;
}

void ConfigClient::Shutdown() { 
  initialized_ = false; 
}

void ConfigClient::SetOnUpdate(UpdateHandler handler) { 
  // 设置配置更新回调，当配置变更时会触发此回调
  on_update_ = std::move(handler); 
}

}  // namespace qtrade::client
