#include <gtest/gtest.h>

#include <quant-trading/adapter/execution_adapter.hpp>
#include <quant-trading/adapter/market_source.hpp>

TEST(AdapterSmoke, MockTypesCompile) {
  static_assert(true);
  SUCCEED();
}
