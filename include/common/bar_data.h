#pragma once

#include <chrono>
#include <string>

namespace quant::trading {

struct Bar {
  std::string instrument;
  std::chrono::system_clock::time_point open_time;
  std::chrono::system_clock::time_point close_time;
  double open{};
  double high{};
  double low{};
  double close{};
  int64_t volume{};
};

}  // namespace quant::trading
