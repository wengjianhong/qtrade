/// @file      main.cpp
/// @brief     策略管理服务（qtrade_strategy_service）
#include "common/service_main.hpp"

int main(int argc, char** argv) {
  return qtrade::common::RunServiceMain(argc, argv, "qtrade_strategy_service", "strategy-service.log");
}
