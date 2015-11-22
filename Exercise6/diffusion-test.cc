#include <iostream>
#include "diffusion_mpi.h"
#include <chrono>
#include <algorithm>
#include <mpi.h>

int main(int argc, char** argv) {
  const double sim_duration = 1;
  MPI_Init(&argc, &argv);
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0)
    std::cout << "Using " << size << " processes." << std::endl;

  Diffusion d (64, 0.0001, 0.5);

  auto start_t = std::chrono::steady_clock::now();
  d.Simulate(sim_duration);
  auto end_t = std::chrono::steady_clock::now();
  auto duration = end_t - start_t;
  MPI_Barrier(MPI_COMM_WORLD);
  double elapsed_t = std::chrono::duration <double, std::milli>
      (duration).count();


  if (rank == 0)
    std::cerr << "Finished: " << elapsed_t << std::endl;
  d.print_result();

  d.clear_grid();

  MPI_Finalize();
  return 0;
}
