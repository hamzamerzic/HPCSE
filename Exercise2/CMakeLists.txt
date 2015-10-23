include_directories(.)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_library(diffusion diffusion.cc)
target_link_libraries(diffusion
                      ${CMAKE_THREAD_LIBS_INIT})

add_executable(exercise2-1 exercise2-1.cc)
target_link_libraries(exercise2-1 
                      diffusion
                      ${CMAKE_THREAD_LIBS_INIT})

add_executable(exercise2-2 exercise2-2.cc)
target_link_libraries(exercise2-2 
                      ${CMAKE_THREAD_LIBS_INIT})

add_executable(diffusion-test-compare diffusion-test-compare.cc)
target_link_libraries(diffusion-test-compare
                      diffusion
                      ${CMAKE_THREAD_LIBS_INIT})

