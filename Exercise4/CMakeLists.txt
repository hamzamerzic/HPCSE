include_directories(.)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")

add_executable(diffusion_steady_test diffusion_steady_test.cc)

