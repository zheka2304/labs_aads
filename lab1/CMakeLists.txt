cmake_minimum_required(VERSION 3.12)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wno-deprecated-declarations")

add_subdirectory(google_tests)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

project(lab1)
add_executable(lab1 main.cpp)

add_executable(lab1_tests test.cpp)
target_link_libraries(lab1_tests gtest gtest_main)