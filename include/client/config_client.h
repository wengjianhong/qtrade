#pragma once

#include <functional>
#include <string>
#include <string_view>

#include "common/error_code.h"

namespace quant::trading::client {

class ConfigClient {
 public:
  using UpdateHandler = std::function<void(std::string_view key, std::string_view value)>;

  ErrorCode Init(std::string_view bootstrap_address);
  void Shutdown();

  void SetOnUpdate(UpdateHandler handler);

 private:
  UpdateHandler on_update_;
  bool initialized_{false};
};

}  // namespace quant::trading::client
