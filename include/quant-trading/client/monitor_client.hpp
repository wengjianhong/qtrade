#ifndef QUANT_TRADING_CLIENT_MONITOR_CLIENT_HPP_
#define QUANT_TRADING_CLIENT_MONITOR_CLIENT_HPP_

#include <string_view>

#include <quant-trading/structs/error_code.hpp>

namespace quant::trading::client {

class MonitorClient {
 public:
  ErrorCode Init(std::string_view endpoint);
  void Shutdown();

  void Counter(std::string_view name, double value);
  void Gauge(std::string_view name, double value);

 private:
  bool initialized_{false};
};

}  // namespace quant::trading::client

#endif  // QUANT_TRADING_CLIENT_MONITOR_CLIENT_HPP_
