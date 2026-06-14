/// @file      main.cpp
/// @brief     服务注册发现（qtrade_registry_service）
#include "common/service_main.hpp"

int main(int argc, char** argv) {
  return qtrade::common::RunServiceMain(argc, argv, "qtrade_registry_service", "registry-service.log");
}
