set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")

add_executable(omp_playground omp_playground.cc)

add_library(diffusion_omp diffusion_omp.cc)

add_executable(diffusion-test-compare-omp diffusion-test-compare.cc)
target_link_libraries(diffusion-test-compare-omp
                      diffusion_omp)
