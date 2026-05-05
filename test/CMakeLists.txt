# GTest-based unit tests (aligned with cpp-utils test layout)
find_package(GTest REQUIRED CONFIG)

file(GLOB_RECURSE ALL_TEST_FILES "*.cpp")
set(ALL_TEST_FILES 
    ${ALL_TEST_FILES}
)
add_executable(quant_trading_tests ${ALL_TEST_FILES})

# Project root + parent (for includes like base/common/... when base lives next to this repo)
target_include_directories(quant_trading_tests PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/..
)

target_link_directories(quant_trading_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/release/lib
)

target_link_libraries(quant_trading_tests PRIVATE
    GTest::gtest
    GTest::gtest_main
    qtbase
    atomic
    Threads::Threads
)

include(GoogleTest)
gtest_discover_tests(quant_trading_tests
    PROPERTIES
        LABELS "unit"
        DISCOVERY_TIMEOUT 60
)
