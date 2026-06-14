# ---------------------------------------------------------------------------
# 独立进程：配置中心服务（qtrade_config_service）
# ---------------------------------------------------------------------------

# Build Service Executable
add_executable(qtrade_config_service ${QTRADE_APPS_DIR}/qtrade_config_service/main.cpp)
target_include_directories(qtrade_config_service PRIVATE
    ${QTRADE_SRC_DIR}
)
target_link_libraries(qtrade_config_service PRIVATE qtrade_common)
