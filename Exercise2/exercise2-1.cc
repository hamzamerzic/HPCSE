#include <iostream>
#include <string>
#include "diffusion.h"
#include <thread>
#include <chrono>
#include <algorithm>

int main(int argc, char *argv[]) {
  std::vector<std::string> args;
  unsigned num_thr = 0, grid_size = 0;
  double time = 0, dt = 0;
  for (int i = 0; i < argc; ++i) {
    args.emplace_back(argv[i]);
  }
  for (int i = 1; i < args.size(); ++i) {
    if (args[i] == "-n") {
      num_thr = std::stoi(args[++i]);
    } else if (args[i] == "-g") {
      grid_size = std::stoi(args[++i]);
    } else if (args[i] == "-t") {
      time = std::stod(args[++i]);
    } else if (args[i] == "-d") {
      dt = std::stod(args[++i]);
    } else {
      std::cout << "Erroneous arguments!" << std::endl;
      return 0;
    }
  }
  if (std::abs(dt * num_thr * grid_size * time) < 1e-10) {
    std::cout << "Missing argument!" << std::endl;
    return 0;
  }
  std::cout << "Grid size " << grid_size << ", time " << time <<
      ", time step " << dt << std::endl;

  Diffusion d (grid_size, dt, 1);
  auto start_t = std::chrono::steady_clock::now();
  if (num_thr == 1) {
    d.Simulate(time);
  }
  else {
    d.SimulateParallel(time, num_thr);
  }
  auto end_t = std::chrono::steady_clock::now();
  auto duration = end_t - start_t;
  double elapsed_t = std::chrono::duration <double, std::milli>
      (duration).count();

  std::cout << "Finished in " << elapsed_t << " ms with " << num_thr <<
      " thr." << std::endl;
}