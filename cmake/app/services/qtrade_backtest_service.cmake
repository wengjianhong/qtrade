# ---------------------------------------------------------------------------
# 独立进程：回测引擎服务（qtrade_backtest_service）
# ---------------------------------------------------------------------------

# Build Service Executable
add_executable(qtrade_backtest_service ${QTRADE_APPS_DIR}/qtrade_backtest_service/main.cpp)
target_include_directories(qtrade_backtest_service PRIVATE
    ${QTRADE_SRC_DIR}
)
target_link_libraries(qtrade_backtest_service PRIVATE qtrade_common)
