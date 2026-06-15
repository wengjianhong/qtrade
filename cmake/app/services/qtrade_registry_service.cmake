# ---------------------------------------------------------------------------
# 独立进程：服务注册发现（qtrade_registry_service）
# ---------------------------------------------------------------------------

# Build Service Executable
add_executable(qtrade_registry_service ${QTRADE_APPS_DIR}/qtrade_registry_service/main.cpp)
target_include_directories(qtrade_registry_service PRIVATE
    ${QTRADE_SRC_DIR}
)
target_link_libraries(qtrade_registry_service PRIVATE qtrade_common)
