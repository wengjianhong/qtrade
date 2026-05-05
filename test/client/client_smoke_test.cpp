#include <gtest/gtest.h>

#include "client/log_client.h"

TEST(ClientSmoke, LogClientInit) {
  quant::trading::client::LogClient client;
  ASSERT_EQ(client.Init("smoke"), quant::trading::ErrorCode::kOk);
  client.Emit("info", "client smoke");
  client.Shutdown();
}
