#ifndef DIFFUSION_H
#define DIFFUSION_H

#include <thread>
#include <deque>

class Diffusion {
 public:
  Diffusion(unsigned steps, double dt, double D);

  void NextStep(int row);

  void Simulate(double time) {
    Simulate_(0, steps_, time);
  }

  void SimulateParallel(double time, int num_thr);
  void clear_grid() { Diffusion(steps_, dt_, D_); }

 private:
  void Simulate_(int begin_row, int end_row, double time);

  unsigned steps_;
  double dt_, D_, dx_;
  std::deque<std::deque<double>> grid_;
};

#endif  // DIFFUSION_H