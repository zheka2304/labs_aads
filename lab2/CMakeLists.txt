cmake_minimum_required(VERSION 3.12)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wno-deprecated-declarations")

project(lab2)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(lab2 main.cpp buffer.cpp huffman.cpp)
add_executable(lab2_tests test.cpp buffer.cpp huffman.cpp)
target_link_libraries(lab2_tests gtest gtest_main)