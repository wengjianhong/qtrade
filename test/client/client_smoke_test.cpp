#include <gtest/gtest.h>

#include <qtrade/client/log_client.hpp>

TEST(ClientSmoke, LogClientInit) {
  qtrade::trading::client::LogClient client;
  ASSERT_EQ(client.Init("smoke"), qtrade::trading::ErrorCode::kOk);
  client.Emit("info", "client smoke");
  client.Shutdown();
}
