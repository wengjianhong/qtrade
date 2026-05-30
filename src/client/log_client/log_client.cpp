#include <qtrade/client/log_client.hpp>

#include <spdlog/spdlog.h>

namespace qtrade::client {

ErrorCode LogClient::Init(std::string_view topic) {
  (void)topic;
  initialized_ = true;
  return ErrorCode::kSuccess;
}

void LogClient::Shutdown() { initialized_ = false; }

void LogClient::Emit(std::string_view level, std::string_view message) {
  if (!initialized_) {
    return;
  }
  if (level == "error") {
    spdlog::error("{}", message);
  } else if (level == "warn") {
    spdlog::warn("{}", message);
  } else {
    spdlog::info("{}", message);
  }
}

}  // namespace qtrade::client
