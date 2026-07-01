# ---------------------------------------------------------------------------
# cpp-utils：通用 C++ 工具库（含 SOCI 数据库封装）
# ---------------------------------------------------------------------------

set(CPPUTILS_ROOT "${CMAKE_SOURCE_DIR}/../cpputils" CACHE PATH "cpp-utils source root")

if(NOT EXISTS "${CPPUTILS_ROOT}/CMakeLists.txt")
    message(FATAL_ERROR "cpp-utils not found at ${CPPUTILS_ROOT}")
endif()

if(NOT TARGET cpp-utils)
    set(CPPUTILS_ENABLE_SOCI ON CACHE BOOL "Build cpp-utils SOCI wrapper" FORCE)
    add_subdirectory(${CPPUTILS_ROOT} ${CMAKE_BINARY_DIR}/cpputils EXCLUDE_FROM_ALL)
endif()
