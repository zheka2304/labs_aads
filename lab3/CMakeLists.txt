cmake_minimum_required(VERSION 3.12)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wno-deprecated-declarations")

project(lab3)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(lab3 main.cpp pathfinding.cpp)
add_executable(lab3_tests test.cpp pathfinding.cpp)
target_link_libraries(lab3_tests gtest gtest_main)