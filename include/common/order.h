#pragma once

#include <cstdint>
#include <string>

namespace quant::trading {

enum class OrderSide : std::uint8_t { kBuy = 0, kSell = 1 };

enum class OrderStatus : std::uint8_t {
  kNew = 0,
  kPartiallyFilled,
  kFilled,
  kCanceled,
  kRejected,
};

struct Order {
  std::string order_id;
  std::string instrument;
  OrderSide side{OrderSide::kBuy};
  double price{};
  int64_t volume{};
  OrderStatus status{OrderStatus::kNew};
};

struct Trade {
  std::string trade_id;
  std::string order_id;
  std::string instrument;
  double price{};
  int64_t volume{};
};

}  // namespace quant::trading
