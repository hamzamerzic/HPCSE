#ifndef DIFFUSION_H
#define DIFFUSION_H

#include <thread>
#include <vector>
#include <iostream>
#include "barrier.h"

class Diffusion {
 public:
  Diffusion(unsigned steps, double dt, double D);

  void Simulate(double time) {
    double cur_time = 0;
    while (cur_time < time) {
      for (unsigned row = 0; row < steps_; ++row) {
        // df/dt (t0) = (f(t0 + dt) - f(t0))/dt
        for (unsigned i = 0; i < steps_; ++i) {
          grid_tmp_[row * steps_ + i] = grid_[row * steps_ + i] + fac_ * (
            (i == 0 ? 0. : grid_[row * steps_ + i - 1]) +
            (i == steps_ - 1 ? 0. : grid_[row * steps_ + i + 1]) +
            (row == 0 ? 0. : grid_[(row - 1) * steps_ + i]) +
            (row == steps_ - 1 ? 0. : grid_[(row + 1) * steps_ + i]) -
              4 * grid_[row * steps_ + i]);
        }
      }
      std::swap(grid_, grid_tmp_);
      cur_time += dt_;
    }
  }

  void SimulateParallel(double time, int num_thr);
  void clear_grid();
  void print_result() {
    for(int i = 0; i < steps_; ++i) {
      for(int j = 0; j < steps_; ++j)
        std::cout << grid_[i * steps_ + j] << " ";
      std::cout << std::endl;
    }
  }

 private:
  void Simulate_(int begin_row, int end_row, double time,
                 bool par = false, // is the simulation parallel
                 bool swap = false // should the thread swap grid and grid_tmp
                 );

  unsigned steps_;
  double dt_, D_, dx_, fac_;
  std::vector<double> grid_, grid_tmp_;
  Barrier* b;
};

#endif  // DIFFUSION_H