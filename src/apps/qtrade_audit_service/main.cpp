/// @file      main.cpp
/// @brief     交易审计服务（qtrade_audit_service）
#include "common/service_main.hpp"

int main(int argc, char** argv) {
  return qtrade::common::RunServiceMain(argc, argv, "qtrade_audit_service", "audit-service.log");
}
