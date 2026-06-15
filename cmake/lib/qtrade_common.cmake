# ---------------------------------------------------------------------------
# qtrade_common：日志、进程 bootstrap、错误码（支撑服务最小依赖）
# ---------------------------------------------------------------------------

# Source Files
file(GLOB_RECURSE COMMON_SRC CONFIGURE_DEPENDS ${QTRADE_SRC_DIR}/common/*/*.cpp)
file(GLOB_RECURSE QTRADE_SRC CONFIGURE_DEPENDS ${QTRADE_SRC_DIR}/qtrade/*.cpp)

# Build Common Library
add_library(qtrade_common STATIC ${COMMON_SRC} ${QTRADE_SRC})
target_include_directories(qtrade_common PUBLIC
    ${QTRADE_INCLUDE_DIR}
)
target_include_directories(qtrade_common PRIVATE
    ${QTRADE_SRC_DIR}
)
target_link_libraries(qtrade_common PUBLIC
    Threads::Threads
    spdlog::spdlog
)
