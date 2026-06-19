/// @file      adapter_smoke_test.cpp
/// @brief     适配器冒烟测试
/// @details   验证行情源与执行适配器类型可编译
/// @author    wengjianhong
/// @date      2026-05-19
/// @copyright CC BY-NC-SA 4.0
#include <gtest/gtest.h>

#include <qtrade/adapter/execution_adapter.hpp>
#include <qtrade/adapter/market_source.hpp>

TEST(AdapterSmoke, MockTypesCompile) {
  static_assert(true);
  SUCCEED();
}
