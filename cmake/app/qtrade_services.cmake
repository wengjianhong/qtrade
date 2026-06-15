# ---------------------------------------------------------------------------
# 独立进程：支撑微服务（逐个 include）
# ---------------------------------------------------------------------------

include(${CMAKE_SOURCE_DIR}/cmake/app/services/qtrade_config_service.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/app/services/qtrade_log_service.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/app/services/qtrade_monitor_service.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/app/services/qtrade_registry_service.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/app/services/qtrade_history_order_service.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/app/services/qtrade_audit_service.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/app/services/qtrade_backtest_service.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/app/services/qtrade_backup_service.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/app/services/qtrade_strategy_service.cmake)

# 供根 CMakeLists.txt install 使用
set(QTRADE_SERVICE_TARGETS
    qtrade_config_service
    qtrade_log_service
    qtrade_monitor_service
    qtrade_registry_service
    qtrade_history_order_service
    qtrade_audit_service
    qtrade_backtest_service
    qtrade_backup_service
    qtrade_strategy_service
)
