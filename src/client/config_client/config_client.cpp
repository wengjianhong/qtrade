#include <qtrade/client/config_client.hpp>

namespace quant::trading::client {

ErrorCode ConfigClient::Init(std::string_view bootstrap_address) {
  (void)bootstrap_address;
  initialized_ = true;
  return ErrorCode::kOk;
}

void ConfigClient::Shutdown() { initialized_ = false; }

void ConfigClient::SetOnUpdate(UpdateHandler handler) { on_update_ = std::move(handler); }

}  // namespace quant::trading::client
