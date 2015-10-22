#include "diffusion.h"

Diffusion::Diffusion(unsigned steps, double dt, double D)
    : steps_(steps), dt_(dt), D_(D), dx_(1. / (steps - 1)) {
  // Generating the empty grid
  grid_.resize(steps_ * steps_, 0);
  grid_tmp_.resize(steps_ * steps_, 0);

  fac_ = dt_ * D_ / (dx_ * dx_);

  // Filling in the empty fields
  unsigned fourth = (steps + 1) / 4 + 1;
  for (int i = fourth; i < steps - fourth; ++i) {
    for (int j = fourth; j < steps - fourth; ++j)
      grid_[i * steps_ + j] = 1;
  }
}

void Diffusion::SimulateParallel(double time, int num_thr) {
  std::vector<std::thread> threads (num_thr - 1);
  int rows_per_thread = steps_ / num_thr;
  int rest = steps_ % num_thr;
  b = new Barrier(num_thr);

  int current_row = 0;
  for (unsigned i = 0; i < num_thr - 1; ++i) {
    if (rest > 0) {
      threads[i] = std::thread(&Diffusion::Simulate_, this, current_row,
                               current_row + rows_per_thread + 1, time, true,
                               false);
      current_row += rows_per_thread + 1;
      --rest;
    }
    else {
      threads[i] = std::thread(&Diffusion::Simulate_, this, current_row,
                               current_row + rows_per_thread, time, true,
                               false);
      current_row += rows_per_thread;
    }
  }

  Simulate_(current_row, current_row + rows_per_thread, time, true, true);
  for(auto& thread : threads)
    thread.join();
}

void Diffusion::clear_grid() {
  std::fill(grid_.begin(), grid_.end(), 0.);
  std::fill(grid_tmp_.begin(), grid_tmp_.end(), 0.);

  unsigned fourth = (steps_ + 1) / 4 + 1;
  for (int i = fourth; i < steps_ - fourth; ++i) {
    for (int j = fourth; j < steps_ - fourth; ++j)
      grid_[i * steps_ + j] = 1;
  }
}

void Diffusion::Simulate_(int begin_row, int end_row, double time, bool par,
    bool swap) {
  double cur_time = 0;
  while (cur_time < time) {
    for (unsigned row = begin_row; row < end_row; ++row) {
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
    if (par) b->wait();
    if (swap) std::swap(grid_, grid_tmp_);
    if (par) b->wait();
    cur_time += dt_;
  }
  return;
}
