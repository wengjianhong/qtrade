#include <qtrade/client/log_client.hpp>

#include <spdlog/spdlog.h>

namespace qtrade::client {

ErrorCode LogClient::Init(std::string_view topic) {
  (void)topic;  // 预留参数，用于后续扩展（如按topic分类日志）
  initialized_ = true;
  return ErrorCode::kSuccess;
}

void LogClient::Shutdown() { 
  initialized_ = false; 
}

void LogClient::Emit(std::string_view level, std::string_view message) {
  // 检查初始化状态，未初始化时静默返回
  if (!initialized_) {
    return;
  }
  
  // 根据日志级别调用对应的spdlog接口
  if (level == "error") {
    spdlog::error("{}", message);
  } else if (level == "warn") {
    spdlog::warn("{}", message);
  } else {
    spdlog::info("{}", message);
  }
}

}  // namespace qtrade::client
