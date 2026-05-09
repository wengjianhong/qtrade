#include <gtest/gtest.h>

#include <quant-trading/client/log_client.hpp>

TEST(ClientSmoke, LogClientInit) {
  quant::trading::client::LogClient client;
  ASSERT_EQ(client.Init("smoke"), quant::trading::ErrorCode::kOk);
  client.Emit("info", "client smoke");
  client.Shutdown();
}
