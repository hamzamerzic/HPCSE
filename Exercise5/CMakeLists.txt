set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")

add_executable(metropolis_sampling_parallel metropolis_sampling_parallel.cc)

