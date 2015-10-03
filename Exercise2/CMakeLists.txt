include_directories(.)

add_library(diffusion diffusion.cc)

add_executable(exercise2-1 exercise2-1.cc)
target_link_libraries(exercise2-1 
											diffusion)

add_executable(exercise2-2 exercise2-2.cc)

