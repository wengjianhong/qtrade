# 工程路径（集中维护，避免与 src/ 混放 CMakeLists）
# 命名约定：cmake/*.cmake 统一使用 qtrade_ 前缀 + snake_case
set(QTRADE_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(QTRADE_SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(QTRADE_DEMO_DIR ${CMAKE_SOURCE_DIR}/demo)
set(QTRADE_APPS_DIR ${QTRADE_SRC_DIR}/apps)
