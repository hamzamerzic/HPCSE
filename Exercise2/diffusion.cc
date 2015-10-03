#include "diffusion.h"

Diffusion::Diffusion(unsigned steps, double dt, double D)
    : steps_(steps), dt_(dt), D_(D), dx_(1. / steps) {

  // Generating the empty grid
  for (unsigned i = 0; i < steps; ++i)
    grid_.emplace_back(std::deque<double> (steps, 0));

  // Filling in the empty fields
  unsigned fourth = (steps + 1) / 4 + 1;
  for (int i = fourth; i < steps - fourth; ++i) {
    for (int j = fourth; j < steps - fourth; ++j)
      grid_[i][j] = 1;
  }
}

void Diffusion::NextStep(int row) {
  // df/dt (t0) = (f(t0 + dt) - f(t0))/dt
  grid_[row][0] += dt_ * D_ * (grid_[row][1] - 2 * grid_[row][0]) /
      (dx_ * dx_);
  for (unsigned i = 1; i < steps_ - 1; ++i) {
    grid_[row][i] += dt_ * D_ * (grid_[row][i - 1] + grid_[row][i + 1] -
        2 * grid_[row][i]) / (dx_ * dx_);
  }
  grid_[row][steps_ - 1] += dt_ * D_ * (grid_[row][steps_ - 2] -
      2 * grid_[row][steps_ - 1]) / (dx_ * dx_);
}

void Diffusion::SimulateParallel(double time, int num_thr) {
  std::deque<std::thread> threads (num_thr - 1);
  int rows_per_thread = steps_ / num_thr;
  int rest = steps_ % num_thr;

  int current_row = 0;
  for (unsigned i = 0; i < num_thr - 1; ++i) {
    if (rest > 0) {
      threads[i] = std::thread(&Diffusion::Simulate_, this, current_row,
                               current_row + rows_per_thread + 1, time);
      current_row += rows_per_thread + 1;
      --rest;
    }
    else {
      threads[i] = std::thread(&Diffusion::Simulate_, this, current_row,
                               current_row + rows_per_thread, time);
      current_row += rows_per_thread;
    }
  }

  Simulate_(current_row, current_row + rows_per_thread, time);

  for(auto& thread : threads)
    thread.join();
}

void Diffusion::Simulate_(int begin_row, int end_row, double time) {
  double cur_time = 0;
  while (cur_time < time) {
    for (unsigned row = begin_row; row < end_row; ++row) {
      NextStep(row);
    }
    cur_time += dt_;
  }
}
