# ---------------------------------------------------------------------------
# qtrade_common：日志、进程 bootstrap、错误码（支撑服务最小依赖）
# ---------------------------------------------------------------------------

# Source Files
file(GLOB_RECURSE COMMON_SRC CONFIGURE_DEPENDS ${QTRADE_SRC_DIR}/common/*/*.cpp)
file(GLOB_RECURSE PUBLIC_SRC CONFIGURE_DEPENDS ${QTRADE_SRC_DIR}/public/*.cpp)
file(GLOB_RECURSE COMMON_GRPC_SRC CONFIGURE_DEPENDS ${QTRADE_SRC_DIR}/common/grpc/*.cpp)
file(GLOB_RECURSE SERVICE_CONFIG_SRC CONFIGURE_DEPENDS ${QTRADE_SRC_DIR}/service/config_service/*.cpp)
file(GLOB_RECURSE SERVICE_CONFIG_REPO_SRC CONFIGURE_DEPENDS ${QTRADE_SRC_DIR}/service/config_service/repository/*.cpp)

# Build Common Library
add_library(qtrade_common STATIC ${COMMON_SRC} ${PUBLIC_SRC} ${COMMON_GRPC_SRC} ${SERVICE_CONFIG_SRC}
    ${SERVICE_CONFIG_REPO_SRC})
target_include_directories(qtrade_common PUBLIC
    ${QTRADE_INCLUDE_DIR}
)
target_include_directories(qtrade_common PRIVATE
    ${QTRADE_SRC_DIR}
)
target_link_libraries(qtrade_common PUBLIC
    Threads::Threads
    spdlog::spdlog
    qtrade_proto
    nlohmann_json::nlohmann_json
    cpputils::cpputils
)
