cmake_minimum_required(VERSION 2.8)
project(HPCSE)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O2")

set(CMAKE_THREAD_PREFER_PTHREAD ON)
find_package(Threads REQUIRED)
if(CMAKE_USE_PTHREADS_INIT)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
endif()
find_package(OpenMP REQUIRED)
find_package(MPI REQUIRED)

include_directories(${MPI_CXX_INCLUDE_PATH})

add_subdirectory(Exercise1)
add_subdirectory(Exercise2)
add_subdirectory(Exercise3)
add_subdirectory(Exercise4)
add_subdirectory(Exercise5)
add_subdirectory(Exercise6)
