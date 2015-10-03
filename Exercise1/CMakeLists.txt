include_directories(.)
add_executable(exercise1 exercise1.cc)
target_link_libraries(exercise1 ${CMAKE_THREAD_LIBS_INIT})
