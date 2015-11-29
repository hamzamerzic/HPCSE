add_executable(simple_vectorization simple_vectorization.cc)
set_target_properties(simple_vectorization PROPERTIES COMPILE_FLAGS "-mavx")
