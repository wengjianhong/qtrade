#ifndef QTRADE_TRADING_CLIENT_MONITOR_CLIENT_HPP_
#define QTRADE_TRADING_CLIENT_MONITOR_CLIENT_HPP_

#include <string_view>

#include <qtrade/error_code/error_code.hpp>

namespace qtrade::client {

class MonitorClient {
 public:
  ErrorCode Init(std::string_view endpoint);
  void Shutdown();

  void Counter(std::string_view name, double value);
  void Gauge(std::string_view name, double value);

 private:
  bool initialized_{false};
};

}  // namespace qtrade::client

#endif  // QTRADE_TRADING_CLIENT_MONITOR_CLIENT_HPP_
