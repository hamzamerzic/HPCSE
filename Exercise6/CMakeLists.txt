# include_directories(.)

add_executable(mpi_playground mpi_playground.cc)
target_link_libraries(mpi_playground
                      ${MPI_CXX_LIBRARIES})

add_library(diffusion_mpi diffusion_mpi.cc)
target_link_libraries(diffusion_mpi
                      ${MPI_CXX_LIBRARIES})

add_executable(diffusion-test diffusion-test.cc)
target_link_libraries(diffusion-test
                      ${MPI_CXX_LIBRARIES}
                      diffusion_mpi)
                      
                    
                      
                     
