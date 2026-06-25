cmake_minimum_required(VERSION 3.22)
project(qtrade VERSION 0.1.0 LANGUAGES CXX)

# Export compile commands for IDEs
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Default Build Type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)
endif()

# C++ Standard Settings
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Output Directories
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Compiler & Linker Options for Release Build
add_compile_options(-fPIC -Wall -Wextra -Wpedantic -O2)

# Dependencies
find_package(Threads REQUIRED)
find_package(spdlog 1.16.0 REQUIRED)
find_package(nlohmann_json 3.11.0 REQUIRED)

# 工程路径与构建脚本（构建逻辑集中在 cmake/，src/ 仅保留源码）
include(${CMAKE_SOURCE_DIR}/cmake/qtrade_paths.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/lib/qtrade_proto.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/lib/qtrade_common.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/lib/qtrade_core.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/app/qtrade_engine.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/app/qtrade_services.cmake)

# Install
install(TARGETS qtrade_core qtrade_common qtrade_proto ARCHIVE DESTINATION ${CMAKE_SOURCE_DIR}/release/lib)
install(TARGETS
    qtrade_engine
    ${QTRADE_SERVICE_TARGETS}
    RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/release/bin
)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/ DESTINATION ${CMAKE_SOURCE_DIR}/release/include)

# Build Tests
enable_testing()
add_subdirectory(test)
