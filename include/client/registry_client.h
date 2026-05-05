#pragma once

#include <string>
#include <string_view>

#include "common/error_code.h"

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
