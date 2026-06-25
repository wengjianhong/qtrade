# ---------------------------------------------------------------------------
# qtrade_core：交易引擎核心库（adapter/mock、adapter/emt / engine / client / demo）
# ---------------------------------------------------------------------------

# Source Files
file(GLOB_RECURSE ADAPTER_SRC CONFIGURE_DEPENDS ${QTRADE_SRC_DIR}/adapter/*.cpp)
file(GLOB_RECURSE ENGINE_SRC CONFIGURE_DEPENDS ${QTRADE_SRC_DIR}/engine/*.cpp)
file(GLOB_RECURSE CLIENT_SRC CONFIGURE_DEPENDS ${QTRADE_SRC_DIR}/client/*.cpp)
file(GLOB_RECURSE DEMO_SRC CONFIGURE_DEPENDS ${QTRADE_DEMO_DIR}/strategy/*.cpp)
list(APPEND CORE_SRC_FILES
    ${ADAPTER_SRC}
    ${ENGINE_SRC}
    ${CLIENT_SRC}
    ${DEMO_SRC}
)

# Build Core Library
add_library(qtrade_core STATIC ${CORE_SRC_FILES})
target_include_directories(qtrade_core PUBLIC
    ${QTRADE_INCLUDE_DIR}
)
target_include_directories(qtrade_core PRIVATE
    ${QTRADE_SRC_DIR}
    ${QTRADE_DEMO_DIR}
)
target_link_libraries(qtrade_core PUBLIC
    qtrade_common
    qtrade_proto
    nlohmann_json::nlohmann_json
)
