#ifndef QUANT_TRADING_CLIENT_REGISTRY_CLIENT_HPP_
#define QUANT_TRADING_CLIENT_REGISTRY_CLIENT_HPP_

#include <string>
#include <string_view>

#include <quant-trading/structs/error_code.hpp>

namespace quant::trading::client {

class RegistryClient {
 public:
  ErrorCode Init(std::string_view etcd_endpoints);
  void Shutdown();

  ErrorCode Register(std::string_view service_name, std::string_view instance_id,
                     std::string_view address);

 private:
  bool initialized_{false};
};

}  // namespace quant::trading::client

#endif  // QUANT_TRADING_CLIENT_REGISTRY_CLIENT_HPP_
