#include "diffusion_steady.h"
#include <iostream>
#include <chrono>
#include <vector>

double g(double x, double y) {
  return x;
}

int main() {
  const int reps = 18;
  DiffusionSteady d (0.01);
  unsigned N = 2;
  // double rho, var;

  std::vector<unsigned> cores {1, 2, 4};
  for (int i = 0; i < reps; ++i, N *= 2) {
    std::cout << "**********" << std::endl << "N = " << N << std::endl;
    for (auto& num_thr : cores) {
      double elapsed = 0.;
      for (int j = 0; j < 20; ++j) {
        auto start_t = std::chrono::steady_clock::now();
        // std::tie(rho, var) = d.RandomWalk(0.3, 0.4, N, g);
        d.RandomWalk(0.3, 0.4, N, g, num_thr);
        auto end_t = std::chrono::steady_clock::now();
        auto duration = end_t - start_t;
        elapsed += std::chrono::duration <double, std::milli>
          (duration).count();
      }
      std::cout << num_thr << " " << elapsed / 20 << std::endl;
    }
  }
  return 0;
}