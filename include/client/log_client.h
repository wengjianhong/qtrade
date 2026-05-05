#pragma once

#include <string_view>

#include "common/error_code.h"

namespace quant::trading::client {

class LogClient {
 public:
  LogClient() = default;
  ~LogClient() = default;

  ErrorCode Init(std::string_view topic);
  void Shutdown();

  void Emit(std::string_view level, std::string_view message);

 private:
  bool initialized_{false};
};

}  // namespace quant::trading::client
