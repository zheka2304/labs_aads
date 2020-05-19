cmake_minimum_required(VERSION 3.12)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wno-deprecated-declarations")

project(lab1)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(lab1 main.cpp)
add_executable(lab1_tests test.cpp)
target_link_libraries(lab1_tests gtest gtest_main)