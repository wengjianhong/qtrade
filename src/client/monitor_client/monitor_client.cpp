#include <qtrade/client/monitor_client.hpp>

namespace qtrade::client {

ErrorCode MonitorClient::Init(std::string_view endpoint) {
  (void)endpoint;
  initialized_ = true;
  return ErrorCode::kSuccess;
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

}  // namespace qtrade::client
