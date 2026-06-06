#include <qtrade/client/monitor_client.hpp>

namespace qtrade::client {

ErrorCode MonitorClient::Init(std::string_view endpoint) {
  (void)endpoint;  // 预留参数，用于后续扩展（如连接远程监控服务）
  initialized_ = true;
  return ErrorCode::kSuccess;
}

void MonitorClient::Shutdown() { 
  initialized_ = false; 
}

void MonitorClient::Counter(std::string_view name, double value) {
  (void)name;   // 预留参数：指标名称
  (void)value;  // 预留参数：增量值
  // 预留实现：上报计数器指标到监控系统
}

void MonitorClient::Gauge(std::string_view name, double value) {
  (void)name;   // 预留参数：指标名称
  (void)value;  // 预留参数：当前值
  // 预留实现：上报仪表盘指标到监控系统
}

}  // namespace qtrade::client
