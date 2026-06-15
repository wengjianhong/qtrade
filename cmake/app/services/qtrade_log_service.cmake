# ---------------------------------------------------------------------------
# 独立进程：日志分析服务（qtrade_log_service）
# ---------------------------------------------------------------------------

# Build Service Executable
add_executable(qtrade_log_service ${QTRADE_APPS_DIR}/qtrade_log_service/main.cpp)
target_include_directories(qtrade_log_service PRIVATE
    ${QTRADE_SRC_DIR}
)
target_link_libraries(qtrade_log_service PRIVATE qtrade_common)
