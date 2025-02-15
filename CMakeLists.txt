cmake_minimum_required(VERSION 3.29)

project("steganography")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(main Source.cpp)
target_link_libraries(main PUBLIC crypt)
message(STATUS "main PUBLIC ${CMAKE_SOURCE_DIR}/Crypt")
target_link_directories(main PUBLIC ${CMAKE_SOURCE_DIR}/Crypt)
target_include_directories(main PUBLIC ${CMAKE_SOURCE_DIR}/Crypt)

add_subdirectory("Crypt")