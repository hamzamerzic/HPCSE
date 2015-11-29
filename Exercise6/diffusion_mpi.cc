#include "diffusion_mpi.h"
#include <algorithm>

Diffusion::Diffusion(unsigned steps, double dt, double D)
    : steps_(steps), dt_(dt), D_(D), dx_(1. / (steps - 1)) {
  // Generating the empty grid

  fac_ = dt_ * D_ / (dx_ * dx_);

  MPI_Comm_rank(MPI_COMM_WORLD, &rank_);
  MPI_Comm_size(MPI_COMM_WORLD, &size_);

  int length = steps_ / size_ + (rank_ < (steps_ % size_));
  int start_ind = (steps_ / size_)*rank_ +
    std::min(rank_, int(steps_ % size_)) + 1;

  if (rank_ == 0 || rank_ == size_ - 1) {
    grid_.resize(length + 1, std::vector<double>(steps_));
    grid_tmp_.resize(length + 1, std::vector<double>(steps_));
  } else {
    grid_.resize(length + 2, std::vector<double>(steps_));
    grid_tmp_.resize(length + 2, std::vector<double>(steps_));
  }

  // Filling in the empty fields
  unsigned fourth = (steps + 1) / 4 + 1;

  for (int i = 0; i < grid_.size(); ++i) {
    int global_ind = start_ind - 1 + i + (rank_ == 0 || rank_ == size_ - 1);
    if (global_ind < fourth || global_ind >= steps - fourth)
      continue;
    for (int j = fourth; j < steps - fourth; ++j)
      grid_[i][j] = 1;
  }
}

void Diffusion::Simulate(double time) {
  int length = steps_ / size_ + (rank_ < (steps_ % size_));
  int start_ind = (steps_ / size_) * rank_ +
    std::min(rank_, int(steps_ % size_)) + 1;


  for (double cur_time = 0; cur_time < time; cur_time += dt_) {
    if (rank_ != 0 && (rank_ != size_ - 1 || size_ % 2)) {
      MPI_Status status;
      if (rank_ % 2) {
        MPI_Sendrecv(grid_[grid_.size() - 2].data(), grid_[0].size(), MPI_DOUBLE,
                     rank_ + 1, 1, grid_[grid_.size() - 1].data(), grid_[0].size(),
                     MPI_DOUBLE, rank_ + 1, 2, MPI_COMM_WORLD, &status);
      } else {
        MPI_Sendrecv(grid_[1].data(), grid_[0].size(), MPI_DOUBLE,
                     rank_ - 1, 2, grid_[0].data(), grid_[0].size(), MPI_DOUBLE,
                     rank_ - 1, 1, MPI_COMM_WORLD, &status);
      }
    }
    if (rank_ != size_ - 1 || rank_ % 2) {
      MPI_Status status;
      if (!(rank_ % 2)) {
        MPI_Sendrecv(grid_[grid_.size() - 2].data(), grid_[0].size(), MPI_DOUBLE,
                     rank_ + 1, 3, grid_[grid_.size() - 1].data(), grid_[0].size(),
                     MPI_DOUBLE, rank_ + 1, 4, MPI_COMM_WORLD, &status);
      } else {
        MPI_Sendrecv(grid_[1].data(), grid_[0].size(), MPI_DOUBLE,
                     rank_ - 1, 4, grid_[0].data(), grid_[0].size(), MPI_DOUBLE,
                     rank_ - 1, 3, MPI_COMM_WORLD, &status);
      }
    }
    int max = grid_.size();
    for (unsigned row = 0; row < max; ++row) {
      for (unsigned i = 0; i < steps_; ++i) {
        grid_tmp_[row][i] = grid_[row][i] + fac_ * (
          (i == 0 ? 0. : grid_[row][i - 1]) +
          (i == steps_ - 1 ? 0. : grid_[row][i + 1]) +
          (row == 0 ? 0. : grid_[row - 1][i]) +
          (row == max - 1 ? 0. : grid_[row + 1][i]) -
            4 * grid_[row][i]);
      }
    }
    std::swap(grid_, grid_tmp_);
    MPI_Barrier(MPI_COMM_WORLD);
  }
}

void Diffusion::clear_grid() {
  std::fill(grid_.begin(), grid_.end(), std::vector<double>(steps_));
  std::fill(grid_tmp_.begin(), grid_tmp_.end(), std::vector<double>(steps_));

  int start_ind = (steps_ / size_)*rank_ +
    std::min(rank_, int(steps_ % size_)) + 1;

  unsigned fourth = (steps_ + 1) / 4 + 1;
  for (int i = 0; i < grid_.size(); ++i) {
    int global_ind = start_ind - 1 + i + (rank_ == 0 || rank_ == size_ - 1);
    if (global_ind < fourth || global_ind >= steps_ - fourth)
      continue;
    for (int j = fourth; j < steps_ - fourth; ++j)
      grid_[i][j] = 1;
  }
}

void Diffusion::print_result() {
  int length = steps_ / size_ + (rank_ < (steps_ % size_));
  std::cerr << length << std::endl;
  std::vector<double> buffer (steps_ * length);
  int start_ind = rank_ != 0;

  for (int i = 0; i < length; ++i) {
    for (int j = 0; j < steps_; ++j)
      buffer[i * steps_ + j] = grid_[i + start_ind][j];
  }

  std::vector<double> buffer_r;
  std::vector<int> recvcnts(size_), displs(size_);
  if (rank_ == 0) {
    buffer_r.resize(steps_*steps_ + 1000);
    for (unsigned i = 0; i < size_; ++i) {
      int temp = steps_ / size_ + (i < (steps_ % size_));
      recvcnts[i] = temp * steps_;
      displs[i] = i != 0 ? recvcnts[i - 1] + displs[i - 1]: 0;
    }
  }
  MPI_Gatherv(buffer.data(), buffer.size(), MPI_DOUBLE, buffer_r.data(),
    recvcnts.data(), displs.data(), MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Barrier(MPI_COMM_WORLD);
  if (rank_ == 0) {
    for (int i = 0; i < steps_; ++i) {
      for (int j = 0; j < steps_; ++j)
        std::cout << buffer_r[i * steps_ + j] << " ";
      std::cout << std::endl;
    }
  }
}