#include "diffusion_omp.h"

Diffusion::Diffusion(unsigned steps, double dt, double D)
    : steps_(steps), dt_(dt), D_(D), dx_(1. / (steps - 1)) {
  // Generating the empty grid
  grid_.resize(steps_ * steps_, 0);
  grid_tmp_.resize(steps_ * steps_, 0);

  fac_ = dt_ * D_ / (dx_ * dx_);

  // Filling in the empty fields
  unsigned fourth = (steps + 1) / 4 + 1;
  #pragma omp parallel for collapse(2)
  for (int i = fourth; i < steps - fourth; ++i) {
    for (int j = fourth; j < steps - fourth; ++j)
      grid_[i * steps_ + j] = 1;
  }
}

void Diffusion::Simulate_(double time, bool parallel) {
  #pragma omp parallel if (parallel)
  {
    for (double cur_time = 0; cur_time < time; cur_time += dt_) {
      #pragma omp for collapse(2)
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

      #pragma omp single
      std::swap(grid_, grid_tmp_);
    }
  }
}

void Diffusion::clear_grid() {
  std::fill(grid_.begin(), grid_.end(), 0.);
  std::fill(grid_tmp_.begin(), grid_tmp_.end(), 0.);

  unsigned fourth = (steps_ + 1) / 4 + 1;
  #pragma omp parallel for collapse(2)
  for (int i = fourth; i < steps_ - fourth; ++i) {
    for (int j = fourth; j < steps_ - fourth; ++j)
      grid_[i * steps_ + j] = 1;
  }
}