/// @file      client_smoke_test.cpp
/// @brief     客户端冒烟测试
/// @details   验证 LogClient 等客户端模块可正常初始化与调用
/// @author    wengjianhong
/// @date      2026-05-19
/// @copyright CC BY-NC-SA 4.0
#include <gtest/gtest.h>

#include <qtrade/client/log_client.hpp>

TEST(ClientSmoke, LogClientInit) {
  qtrade::client::LogClient client;
  ASSERT_EQ(client.Init("smoke"), qtrade::ErrorCode::kSuccess);
  client.Emit("info", "client smoke");
  client.Shutdown();
}
