#include <iostream>
#include "diffusion.h"
#include <thread>
#include <chrono>
#include <algorithm>

int main(int argc, char *argv[]) {

  std::cerr << "Main thread id: " << std::this_thread::get_id() << std::endl;
  const double sim_duration = 1;
  unsigned num_thr = 1;
  if (argc > 1) {
    num_thr = atoi(argv[1]);
  } else {
    num_thr = std::max(std::thread::hardware_concurrency(), num_thr);
  }
  std::cout << "Using " << num_thr << " threads." << std::endl;

  Diffusion d (128, 0.0001, 1);

  auto start_t = std::chrono::steady_clock::now();
  d.Simulate(sim_duration);
  auto end_t = std::chrono::steady_clock::now();
  auto duration = end_t - start_t;
  double elapsed_t = std::chrono::duration <double, std::milli>
      (duration).count();
  std::cout << "Finished serial: " << elapsed_t << std::endl;
  // d.print_result();

  d.clear_grid();
  std::cout << "\n";

  start_t = std::chrono::steady_clock::now();
  d.SimulateParallel(sim_duration, num_thr);
  end_t = std::chrono::steady_clock::now();
  duration = end_t - start_t;
  elapsed_t = std::chrono::duration <double, std::milli> (duration).count();
  std::cout << "Finished parallel: " << elapsed_t << std::endl;
  // d.print_result();

  return 0;
}
