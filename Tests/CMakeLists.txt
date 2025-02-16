
add_executable(tests
    Tests.cpp
)

find_package(GTest CONFIG REQUIRED)
target_link_libraries(tests PRIVATE crypt GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)

target_link_directories(tests PRIVATE ${CMAKE_SOURCE_DIR}/Crypt)

include(GoogleTest)
gtest_discover_tests(tests)