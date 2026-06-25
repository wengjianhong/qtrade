/// @file      config_grpc_service.cpp
/// @brief     ConfigGrpcService 实现
/// @author    wengjianhong
/// @date      2026-06-22
/// @copyright CC BY-NC-SA 4.0
#include "service/config_service/config_grpc_service.hpp"

#include <chrono>

namespace qtrade::service {

ConfigGrpcService::ConfigGrpcService(std::shared_ptr<ConfigStore> store) : store_(std::move(store)) {}

grpc::Status ConfigGrpcService::GetConfig(grpc::ServerContext* context,
                                          const qtrade::config::v1::GetConfigRequest* request,
                                          qtrade::config::v1::ConfigSnapshot* response) {
  (void)context;
  (void)request;
  *response = store_->GetSnapshot();
  return grpc::Status::OK;
}

grpc::Status ConfigGrpcService::WatchConfig(grpc::ServerContext* context,
                                            const qtrade::config::v1::WatchConfigRequest* request,
                                            grpc::ServerWriter<qtrade::config::v1::ConfigDelta>* writer) {
  (void)request;

  std::uint64_t since = request->since_version();
  while (!context->IsCancelled()) {
    auto delta = store_->WaitForDelta(since, std::chrono::seconds(30));
    if (!delta.has_value()) {
      continue;
    }
    if (!writer->Write(*delta)) {
      return grpc::Status(grpc::StatusCode::CANCELLED, "client disconnected");
    }
    since = delta->version();
  }
  return grpc::Status::OK;
}

}  // namespace qtrade::service
