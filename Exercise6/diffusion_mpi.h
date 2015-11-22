#ifndef DIFFUSION_MPI_H
#define DIFFUSION_MPI_H

#include <mpi.h>
#include <vector>
#include <iostream>

class Diffusion {
 public:
  Diffusion(unsigned steps, double dt, double D);

  void Simulate(double time);
  void clear_grid();
  void print_result();

 private:

  unsigned steps_;
  int rank_, size_;
  double dt_, D_, dx_, fac_;
  std::vector<std::vector<double> > grid_, grid_tmp_;
};

#endif  // DIFFUSION_MPI_H