#include <iostream>
#include "diffusion_omp.h"
#include <chrono>
#include <algorithm>

int main() {
  std::cerr << "Main thread id: " << omp_get_thread_num() << std::endl;
  const double sim_duration = 1;
  int num_threads;
  #pragma omp parallel
  #pragma omp master
  num_threads = omp_get_num_threads();

  std::cout << "Using " << num_threads << " threads." << std::endl;

  Diffusion d (64, 0.0001, 0.5);

  auto start_t = std::chrono::steady_clock::now();
  d.Simulate(sim_duration);
  auto end_t = std::chrono::steady_clock::now();
  auto duration = end_t - start_t;
  double elapsed_t = std::chrono::duration <double, std::milli>
      (duration).count();
  std::cout << "Finished serial: " << elapsed_t << std::endl;
  d.print_result();

  d.clear_grid();
  std::cout << "\n";

  start_t = std::chrono::steady_clock::now();
  d.SimulateParallel(sim_duration);
  end_t = std::chrono::steady_clock::now();
  duration = end_t - start_t;
  elapsed_t = std::chrono::duration <double, std::milli> (duration).count();
  std::cout << "Finished parallel: " << elapsed_t << std::endl;
  d.print_result();

  return 0;
}
