# ---------------------------------------------------------------------------
# 独立进程：历史成交服务（qtrade_history_order_service）
# ---------------------------------------------------------------------------

# Build Service Executable
add_executable(qtrade_history_order_service ${QTRADE_APPS_DIR}/qtrade_history_order_service/main.cpp)
target_include_directories(qtrade_history_order_service PRIVATE
    ${QTRADE_SRC_DIR}
)
target_link_libraries(qtrade_history_order_service PRIVATE qtrade_common)
