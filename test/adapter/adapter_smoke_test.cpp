#include <gtest/gtest.h>

#include <qtrade/adapter/execution_adapter.hpp>
#include <qtrade/adapter/market_source.hpp>

TEST(AdapterSmoke, MockTypesCompile) {
  static_assert(true);
  SUCCEED();
}
