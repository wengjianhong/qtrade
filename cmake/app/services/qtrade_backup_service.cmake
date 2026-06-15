# ---------------------------------------------------------------------------
# 独立进程：容灾备份服务（qtrade_backup_service）
# ---------------------------------------------------------------------------

# Build Service Executable
add_executable(qtrade_backup_service ${QTRADE_APPS_DIR}/qtrade_backup_service/main.cpp)
target_include_directories(qtrade_backup_service PRIVATE
    ${QTRADE_SRC_DIR}
)
target_link_libraries(qtrade_backup_service PRIVATE qtrade_common)
