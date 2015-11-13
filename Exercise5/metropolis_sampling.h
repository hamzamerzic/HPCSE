#ifndef METROPOLIS_SAMPLING_H
#define METROPOLIS_SAMPLING_H

#include <random>
#include <cmath>
#include <utility>
#include <algorithm>

#include "plotter.h"

const unsigned K_M = 512;

class MetropolisSampling {
 public:
  MetropolisSampling(double L,
                     unsigned width,  // Nx
                     unsigned height,  // Ny
                     double d0,
                     double alpha,
                     unsigned seed = 0)
    : L_(L), width_(width), height_(height), d0_(d0), alpha_(alpha),
      dist_update_(-alpha_, alpha_),
      dist_particle_(0, width_ * height_ - 1), histogram_(K_M) {
    std::random_device rd;
    mt_.seed(rd() + seed);

    dx_ = L / width_;
    dy_ = sqrt(3.) * dx_ / 2;
    dA_ = (pow(sqrt(2) * L_ / 2, 2) - d0_ * d0_) / K_M;

    for (unsigned i = 0; i < width_; ++i) {
      for (unsigned j = 0; j < height_; ++j) {
        double x = dx_ * (i + 1./2 * (j % 2 + 0.9)), y = dy_ * (j + 1./2);
        particles_.emplace_back(x, y);
      }
    }
  }

  bool Step() {
    int idx = dist_particle_(mt_);
    double upd_x = particles_[idx].first + dist_update_(mt_),
           upd_y = particles_[idx].second + dist_update_(mt_);
    if (upd_x < 0) upd_x += L_;
    if (upd_x >= L_) upd_x -= L_;
    if (upd_y < 0) upd_y += L_;
    if (upd_y >= L_) upd_y -= L_;

    std::pair<double, double> upd(upd_x, upd_y);

    for (int i = 0; i < width_ * height_; ++i) {
      if (i == idx) continue;
      if (MinDist(upd, particles_[i]) < d0_ * d0_) {
        return false;
      }
    }
    particles_[idx] = upd;
    return true;
  }

  std::vector<int> DistanceProfile() {
    std::vector<int> bins(K_M);

    for (int i = 0; i < width_ * height_ - 1; ++i) {
      for (unsigned j = i + 1; j < width_ * height_; ++j) {
        double dist = MinDist(particles_[i], particles_[j]);

        int idx = (dist - d0_*d0_) / dA_;
        ++bins[idx];
      }
    }
    return bins;
  }

  void Plot(std::string filename) {
    Plotter p (1024, 1024, int(d0_ * 1024 / 2));

    std::vector<std::pair<int, int>> points;
    for (unsigned i = 0; i < particles_.size(); ++i) {
      points.emplace_back(int(particles_[i].first * 1024 / L_),
                          int(particles_[i].second * 1024 / L_));
    }
    p.Plot(points, filename + ".pbm");
  }

  double MinDist(std::pair<double, double> p1, std::pair<double, double> p2) {
    std::vector<double> dist_help {0., -L_, L_};

    double dist = L_;
    for(auto& elx : dist_help) {
      for(auto& ely : dist_help) {
        double x = p1.first + elx - p2.first;
        double y = p1.second + ely - p2.second;
        dist = std::min(dist, x * x + y * y);
      }
    }
    return dist;
  }

 private:
  double L_;
  unsigned width_, height_;
  double d0_, alpha_;
  double dx_, dy_, dA_;
  std::uniform_real_distribution<double> dist_update_;
  std::uniform_int_distribution<int> dist_particle_;
  std::mt19937 mt_;
  std::vector<std::pair<double, double>> particles_;
  std::vector<double> histogram_;
};

#endif  // METROPOLIS_SAMPLING_H
