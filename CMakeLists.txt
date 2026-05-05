cmake_minimum_required(VERSION 3.22)
project(quant-trading VERSION 0.1.0 LANGUAGES CXX)

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
find_package(spdlog 1.12.0 REQUIRED)
find_package(nlohmann_json 3.11.3 REQUIRED)

# Source Files
file(GLOB_RECURSE ADAPTER_SRC CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/src/adapter/*.cpp)
file(GLOB_RECURSE ENGINE_SRC CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/src/engine/*.cpp)
file(GLOB_RECURSE CLIENT_SRC CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/src/client/*.cpp)
list(APPEND CORE_SRC_FILES ${ADAPTER_SRC} ${ENGINE_SRC} ${CLIENT_SRC})

# Build Core Library
add_library(quant_trading_core STATIC ${CORE_SRC_FILES})
target_include_directories(quant_trading_core PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(quant_trading_core PUBLIC 
    Threads::Threads
    spdlog::spdlog
    nlohmann_json::nlohmann_json
)

# Build Main Executable
add_executable(quant-trading ${CMAKE_SOURCE_DIR}/src/main.cpp)
target_link_libraries(quant-trading PRIVATE quant_trading_core)

# Install
install(TARGETS quant-trading RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/release/bin)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/ DESTINATION ${CMAKE_SOURCE_DIR}/release/include)

# Build Tests
enable_testing()
add_subdirectory(test)
