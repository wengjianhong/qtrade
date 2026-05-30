#include <gtest/gtest.h>

#include <qtrade/client/log_client.hpp>

TEST(ClientSmoke, LogClientInit) {
  qtrade::client::LogClient client;
  ASSERT_EQ(client.Init("smoke"), qtrade::ErrorCode::kSuccess);
  client.Emit("info", "client smoke");
  client.Shutdown();
}
