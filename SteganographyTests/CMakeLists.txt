
add_executable(SteganographyTests
    Tests.cpp
)

find_package(GTest CONFIG REQUIRED)
target_link_libraries(SteganographyTests PRIVATE crypt imagecrypthandler GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)

target_link_directories(SteganographyTests PRIVATE ${CMAKE_SOURCE_DIR}/Crypt)

include(GoogleTest)
gtest_discover_tests(SteganographyTests)