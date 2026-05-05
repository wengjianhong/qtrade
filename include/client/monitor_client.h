#pragma once

#include <string_view>

#include "common/error_code.h"

namespace quant::trading::client {

class MonitorClient {
 public:
  ErrorCode Init(std::string_view endpoint);
  void Shutdown();

  void Counter(std::string_view name, double value);
  void Gauge(std::string_view name, double value);

 private:
  bool initialized_{false};
};

}  // namespace quant::trading::client
