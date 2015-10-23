cmake_minimum_required(VERSION 2.8)
project(HPCSE)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(CMAKE_THREAD_PREFER_PTHREAD ON)
find_package(Threads REQUIRED)
if(CMAKE_USE_PTHREADS_INIT)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
endif()
find_package(OpenMP REQUIRED)

add_subdirectory(Exercise1)
add_subdirectory(Exercise2)
add_subdirectory(Exercise3)
