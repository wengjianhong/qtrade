#ifndef QTRADE_TRADING_CLIENT_REGISTRY_CLIENT_HPP_
#define QTRADE_TRADING_CLIENT_REGISTRY_CLIENT_HPP_

#include <string>
#include <string_view>

#include <qtrade/error_code/error_code.hpp>

namespace qtrade::client {

class RegistryClient {
 public:
  ErrorCode Init(std::string_view etcd_endpoints);
  void Shutdown();

  ErrorCode Register(std::string_view service_name, std::string_view instance_id, std::string_view address);

 private:
  bool initialized_{false};
};

}  // namespace qtrade::client

#endif  // QTRADE_TRADING_CLIENT_REGISTRY_CLIENT_HPP_
