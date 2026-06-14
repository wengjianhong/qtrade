/// @file      main.cpp
/// @brief     容灾备份服务（qtrade_backup_service）
#include "common/service_main.hpp"

int main(int argc, char** argv) {
  return qtrade::common::RunServiceMain(argc, argv, "qtrade_backup_service", "backup-service.log");
}
