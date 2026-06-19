/// @file      main.cpp
/// @brief     配置中心服务（qtrade_config_service）
/// @details   独立进程入口，解析配置并启动配置中心主循环
/// @author    wengjianhong
/// @date      2026-05-19
/// @copyright CC BY-NC-SA 4.0
#include "common/app/app_runner.hpp"

int main(int argc, char** argv) {
  return qtrade::common::RunServiceMain(argc, argv, "qtrade_config_service", "config-service.log");
}
