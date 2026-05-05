#pragma once

#include <cstdint>

namespace quant::trading {

enum class ErrorCode : std::int32_t {
  kOk = 0,
  kInvalidArgument = 1,
  kNotInitialized = 2,
  kAlreadyConnected = 3,
  kNotConnected = 4,
  kInternal = 5,
  kTimeout = 6,
  kResourceExhausted = 7,
};

}  // namespace quant::trading
