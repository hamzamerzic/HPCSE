#ifndef DIFFUSION_STEADY_H
#define DIFFUSION_STEADY_H

#include <random>
#include <cmath>
#include <functional>
#include <tuple>
#include <omp.h>
#include <iostream>
#include <vector>

class DiffusionSteady {
 public:
  DiffusionSteady(double d) : d_(d), K_PI(4 * atan(1.)) {
    std::random_device rd;
    random_engine_.seed(rd());
  }

  std::tuple<double, double> RandomWalk(const double x0, const double y0,
        unsigned N, const std::function<double(double, double)>& g,
        unsigned num_threads) {
    if (x0 < 0 || y0 < 0 || x0 > 1 || y0 > 1) throw "Incorrect input.";

    double sum_g = 0., sum_g2 = 0.;

    #pragma omp parallel num_threads(num_threads) reduction(+:sum_g, sum_g2)
    {
      std::mt19937 gen;
      std::uniform_real_distribution<double> distribution(0., 1.);
      #pragma omp critical
      {
        gen.seed(random_engine_());
      }

      double temp = 0., temp2 = 0.;
      #pragma omp for schedule(static)
      for (unsigned i = 0; i < N; ++i) {
        double x = x0, y = y0;
        while (x > 0 && x < 1 && y > 0 && y < 1) {
          double a = distribution(gen);
          x += d_ * cos(2 * K_PI * a);
          y += d_ * sin(2 * K_PI * a);
        }

        double res = g(x, y);
        temp += res;
        temp2 += res * res;
      }
      sum_g = temp;
      sum_g2 = temp2;
    }

    sum_g /= N;
    sum_g2 /= N;
    return std::make_tuple(sum_g, N / (N-1) * (sum_g2 - sum_g * sum_g));
  }

 private:
  double d_;
  const double K_PI;
  std::mt19937 random_engine_;
};

#endif  // DIFFUSION_STEADY_H
