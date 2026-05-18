find_package(GTest 1.14.0 REQUIRED)

file(GLOB_RECURSE ALL_TEST_FILES CONFIGURE_DEPENDS "*.cpp")

add_executable(qtrade_tests ${ALL_TEST_FILES})

target_link_libraries(qtrade_tests PRIVATE
    qtrade_core
    GTest::gtest
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(qtrade_tests
    PROPERTIES
        LABELS "unit"
        DISCOVERY_TIMEOUT 60
)
