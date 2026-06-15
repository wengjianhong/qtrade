# ---------------------------------------------------------------------------
# 独立进程：策略管理服务（qtrade_strategy_service）
# ---------------------------------------------------------------------------

# Build Service Executable
add_executable(qtrade_strategy_service ${QTRADE_APPS_DIR}/qtrade_strategy_service/main.cpp)
target_include_directories(qtrade_strategy_service PRIVATE
    ${QTRADE_SRC_DIR}
)
target_link_libraries(qtrade_strategy_service PRIVATE qtrade_common)
