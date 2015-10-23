cmake_minimum_required(VERSION 2.8)
project(Exercise1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
find_package(Threads REQUIRED)
find_package(OpenMP REQUIRED)

add_subdirectory(Exercise1)
add_subdirectory(Exercise2)
add_subdirectory(Exercise3)
