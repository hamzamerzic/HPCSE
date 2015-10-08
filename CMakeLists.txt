cmake_minimum_required(VERSION 2.8)
project(Exercise1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
find_package(Threads REQUIRED)

add_subdirectory(Exercise1)
add_subdirectory(Exercise2)
