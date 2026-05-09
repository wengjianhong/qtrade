#ifndef QUANT_TRADING_CLIENT_CONFIG_CLIENT_HPP_
#define QUANT_TRADING_CLIENT_CONFIG_CLIENT_HPP_

#include <functional>
#include <string>
#include <string_view>

#include <quant-trading/structs/error_code.hpp>

namespace quant::trading::client {

class ConfigClient {
 public:
  using UpdateHandler = std::function<void(std::string_view key, std::string_view value)>;

  ErrorCode Init(std::string_view bootstrap_address);
  void Shutdown();

  void SetOnUpdate(UpdateHandler handler);

 private:
  UpdateHandler on_update_;
  bool initialized_{false};
};

}  // namespace quant::trading::client

#endif  // QUANT_TRADING_CLIENT_CONFIG_CLIENT_HPP_
