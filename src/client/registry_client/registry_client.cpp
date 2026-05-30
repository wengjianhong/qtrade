#include <qtrade/client/registry_client.hpp>

namespace qtrade::client {

ErrorCode RegistryClient::Init(std::string_view etcd_endpoints) {
  (void)etcd_endpoints;
  initialized_ = true;
  return ErrorCode::kSuccess;
}

void RegistryClient::Shutdown() { initialized_ = false; }

ErrorCode RegistryClient::Register(std::string_view service_name,
                                   std::string_view instance_id,
                                   std::string_view address) {
  (void)service_name;
  (void)instance_id;
  (void)address;
  if (!initialized_) {
    return ErrorCode::kNotInitialized;
  }
  return ErrorCode::kSuccess;
}

}  // namespace qtrade::client
