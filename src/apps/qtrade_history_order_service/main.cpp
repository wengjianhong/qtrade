/// @file      main.cpp
/// @brief     历史成交服务（qtrade_history_order_service）
#include "common/service_main.hpp"

int main(int argc, char** argv) {
  return qtrade::common::RunServiceMain(argc, argv, "qtrade_history_order_service",
                                        "history-order-service.log");
}
