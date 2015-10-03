cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

find_package(Threads REQUIRED)

project(Exercise1)
add_subdirectory(Exercise1)
add_subdirectory(Exercise2)
