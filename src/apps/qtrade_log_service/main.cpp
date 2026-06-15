/// @file      main.cpp
/// @brief     日志分析服务（qtrade_log_service）
#include "common/service_main.hpp"

int main(int argc, char** argv) {
  return qtrade::common::RunServiceMain(argc, argv, "qtrade_log_service", "log-service.log");
}
