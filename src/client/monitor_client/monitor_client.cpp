#include "client/monitor_client.h"

namespace quant::trading::client {

ErrorCode MonitorClient::Init(std::string_view endpoint) {
  (void)endpoint;
  initialized_ = true;
  return ErrorCode::kOk;
}

void MonitorClient::Shutdown() { initialized_ = false; }

void MonitorClient::Counter(std::string_view name, double value) {
  (void)name;
  (void)value;
}

void MonitorClient::Gauge(std::string_view name, double value) {
  (void)name;
  (void)value;
}

}  // namespace quant::trading::client
