# ---------------------------------------------------------------------------
# 独立进程：业务监控服务（qtrade_monitor_service）
# ---------------------------------------------------------------------------

# Build Service Executable
add_executable(qtrade_monitor_service ${QTRADE_APPS_DIR}/qtrade_monitor_service/main.cpp)
target_include_directories(qtrade_monitor_service PRIVATE
    ${QTRADE_SRC_DIR}
)
target_link_libraries(qtrade_monitor_service PRIVATE qtrade_common)
