cmake_minimum_required(VERSION 3.29)

enable_testing()

set(VCPKG_ROOT "VCPKG_ROOT" CACHE STRING "VCPKG_ROOT")

if (NOT DEFINED VCPKG_ROOT)
  message(FATAL_ERROR "MISSING ROOT FOR VCPKG setting to C:/vcpkg")
  set(VCPKG_ROOT "C:/vcpkg")
endif()

set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")

project("steganography")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(main Source.cpp)
target_link_libraries(main PUBLIC crypt)
target_include_directories(main PUBLIC ${CMAKE_SOURCE_DIR}/Crypt)

add_subdirectory("Tests")
add_subdirectory("Crypt")
