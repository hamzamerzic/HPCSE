#include <iostream>
#include "diffusion.h"
#include <thread>
#include <chrono>
#include <algorithm>

int main(int argc, char *argv[]) {
  unsigned num_thr = 1;
  if (argc > 1) {
    num_thr = atoi(argv[1]);
  } else {
    num_thr = std::max(std::thread::hardware_concurrency(), num_thr);
  }
  std::cout << "Using " << num_thr << " threads." << std::endl;

  Diffusion d (128, 0.0005, 1);

  auto start_t = std::chrono::steady_clock::now();
  d.Simulate(1);
  auto end_t = std::chrono::steady_clock::now();
  auto duration = end_t - start_t;
  double elapsed_t = std::chrono::duration <double, std::milli>
      (duration).count();
  std::cout << "Finished " << elapsed_t << std::endl;

  d.clear_grid();

  start_t = std::chrono::steady_clock::now();
  d.SimulateParallel(1, num_thr);
  end_t = std::chrono::steady_clock::now();
  duration = end_t - start_t;
  elapsed_t = std::chrono::duration <double, std::milli> (duration).count();
  std::cout << "Finished " << elapsed_t << std::endl;

  return 0;
}
