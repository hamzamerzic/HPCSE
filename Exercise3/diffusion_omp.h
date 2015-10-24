#ifndef DIFFUSION_OMP_H
#define DIFFUSION_OMP_H

#include <omp.h>
#include <vector>
#include <iostream>

class Diffusion {
 public:
  Diffusion(unsigned steps, double dt, double D);

  void Simulate(double time) { Simulate_(time, false); }

  void SimulateParallel(double time) { Simulate_(time, true); }
  void clear_grid();
  void print_result() {
    for(int i = 0; i < steps_; ++i) {
      for(int j = 0; j < steps_; ++j)
        std::cout << grid_[i * steps_ + j] << " ";
      std::cout << std::endl;
    }
  }

 private:
  void Simulate_(double time, bool parallel);

  unsigned steps_;
  double dt_, D_, dx_, fac_;
  std::vector<double> grid_, grid_tmp_;
};

#endif  // DIFFUSION_OMP_H