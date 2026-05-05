#include "client/log_client.h"

#include <spdlog/spdlog.h>

namespace quant::trading::client {

ErrorCode LogClient::Init(std::string_view topic) {
  (void)topic;
  initialized_ = true;
  return ErrorCode::kOk;
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

}  // namespace quant::trading::client
