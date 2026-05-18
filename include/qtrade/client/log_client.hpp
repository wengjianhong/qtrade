#ifndef QUANT_TRADING_CLIENT_LOG_CLIENT_HPP_
#define QUANT_TRADING_CLIENT_LOG_CLIENT_HPP_

#include <string_view>

#include <qtrade/structs/error_code.hpp>

namespace quant::trading::client {

class LogClient {
 public:
  LogClient() = default;
  ~LogClient() = default;

  ErrorCode Init(std::string_view topic);
  void Shutdown();

  void Emit(std::string_view level, std::string_view message);

 private:
  bool initialized_{false};
};

}  // namespace quant::trading::client

#endif  // QUANT_TRADING_CLIENT_LOG_CLIENT_HPP_
