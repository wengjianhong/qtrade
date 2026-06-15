/// @file      main.cpp
/// @brief     业务监控服务（qtrade_monitor_service）
#include "common/service_main.hpp"

int main(int argc, char** argv) {
  return qtrade::common::RunServiceMain(argc, argv, "qtrade_monitor_service", "monitor-service.log");
}
