/// @file      main.cpp
/// @brief     回测引擎服务（qtrade_backtest_service）
#include "common/service_main.hpp"

int main(int argc, char** argv) {
  return qtrade::common::RunServiceMain(argc, argv, "qtrade_backtest_service", "backtest-service.log");
}
