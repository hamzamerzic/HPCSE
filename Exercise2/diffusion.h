#ifndef DIFFUSION_H
#define DIFFUSION_H

#include <thread>
#include <vector>
#include <iostream>
#include "barrier.h"

class Diffusion {
 public:
  Diffusion(unsigned steps, double dt, double D);

  void NextStep(int row);

  void Simulate(double time) {
    Simulate_(0, steps_, time, false, true);
  }

  void SimulateParallel(double time, int num_thr);
  void clear_grid();
  void print_result() {
    for(auto&& row : grid_) {
      for(auto&& item : row)
        std::cout << item << " ";
      std::cout << std::endl;
    }
  }

 private:
  void Simulate_(int begin_row, int end_row, double time,
                 bool par = false, // is the simulation parallel
                 bool swap = false // should the thread swap grid and grid_tmp
                 );

  unsigned steps_;
  double dt_, D_, dx_;
  std::vector<std::vector<double>> grid_, grid_tmp_;
  Barrier* b;
};

#endif  // DIFFUSION_H