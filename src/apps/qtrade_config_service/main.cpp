/// @file      main.cpp
/// @brief     配置中心服务（qtrade_config_service）
#include "common/service_main.hpp"

int main(int argc, char** argv) {
  return qtrade::common::RunServiceMain(argc, argv, "qtrade_config_service", "config-service.log");
}
