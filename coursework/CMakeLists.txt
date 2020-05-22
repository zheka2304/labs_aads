cmake_minimum_required(VERSION 3.12)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wno-deprecated-declarations")

project(coursework)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(coursework main.cpp flow_network.cpp)
add_executable(coursework_tests test.cpp flow_network.cpp)
target_link_libraries(coursework_tests gtest gtest_main)