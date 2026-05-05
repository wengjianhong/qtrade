#include <gtest/gtest.h>

#include "adapter/execution_adapter.h"
#include "adapter/market_source.h"

TEST(AdapterSmoke, MockTypesCompile) {
  static_assert(true);
  SUCCEED();
}
