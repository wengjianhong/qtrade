cmake_minimum_required(VERSION 3.22)
project(quant-trading VERSION 1.0.0 LANGUAGES CXX)

# C++ Standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compiler & Linker Options
add_compile_options(-fPIC -Wall -Wextra -O2)

# Debug mode flags
add_compile_options($<$<CONFIG:Debug>:-g>)
add_link_options($<$<CONFIG:Debug>:-fsanitize=address>)

# Dependencies
find_package(Threads REQUIRED)
# find_package(spdlog REQUIRED)
# find_package(nlohmann_json REQUIRED)

# Build submodules
#add_subdirectory(core)
#add_subdirectory(plugins)
#add_subdirectory(service)
file(GLOB_RECURSE ALL_SRC_FILES "*.cpp")
set(ALL_SRC_FILES 
    ${ALL_SRC_FILES}
)

# Install quant-trading executable
add_executable(quant-trading ${ALL_SRC_FILES})
target_link_libraries(quant-trading PUBLIC Threads::Threads)
# target_link_libraries(quant-trading PUBLIC spdlog::spdlog)
# target_link_libraries(quant-trading PUBLIC nlohmann_json::nlohmann_json)

# Output Directories
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Install
install(DIRECTORY include/ DESTINATION ${CMAKE_SOURCE_DIR}/release/include)

# Tests
add_subdirectory(test)
