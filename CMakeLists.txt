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
find_package(spdlog 1.17.0 REQUIRED)
find_package(nlohmann_json 3.11.0 REQUIRED)

# Source Files
file(GLOB_RECURSE ADAPTER_SRC CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/src/adapter/*.cpp)
file(GLOB_RECURSE ENGINE_SRC CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/src/engine/*.cpp)
file(GLOB_RECURSE CLIENT_SRC CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/src/client/*.cpp)
file(GLOB_RECURSE COMMON_SRC CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/src/common/*.cpp)
file(GLOB_RECURSE QTRADE_SRC CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/src/qtrade/*.cpp)
file(GLOB_RECURSE DEMO_SRC CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/demo/strategy/*.cpp)
list(APPEND CORE_SRC_FILES 
    ${ADAPTER_SRC} 
    ${ENGINE_SRC} 
    ${CLIENT_SRC} 
    ${COMMON_SRC} 
    ${QTRADE_SRC}
    ${DEMO_SRC}
)

# Build Core Library
add_library(qtrade_core STATIC ${CORE_SRC_FILES})
target_include_directories(qtrade_core PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)
target_include_directories(qtrade_core PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/demo
)
target_link_libraries(qtrade_core PUBLIC 
    Threads::Threads
    spdlog::spdlog
    nlohmann_json::nlohmann_json
)

# Build Main Executable
add_executable(qtrade ${CMAKE_SOURCE_DIR}/src/main.cpp)
target_include_directories(qtrade PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/demo
)
target_link_libraries(qtrade PRIVATE qtrade_core)

# Install
install(TARGETS qtrade RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/release/bin)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/ DESTINATION ${CMAKE_SOURCE_DIR}/release/include)

# Build Tests
enable_testing()
add_subdirectory(test)