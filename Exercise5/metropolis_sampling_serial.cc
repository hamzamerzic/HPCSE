#include "metropolis_sampling.h"
#include <omp.h>
#include <iostream>

int main() {
  double L = 1.;
  unsigned Nx = 100;
  unsigned Ny = 115;
  double d0 = 0.5 * L / Nx;
  double alpha = L/Nx - d0;
  unsigned S0 = 100;
  unsigned S = 500;

  int num_threads;
  #pragma omp parallel
  #pragma omp master
  num_threads = omp_get_num_threads();

  std::vector<std::vector<int>> histograms (num_threads, std::vector<int>(512));

  #pragma omp parallel
  {
    MetropolisSampling m (L, Nx, Ny, d0, alpha, omp_get_thread_num());
    int thread_num = omp_get_thread_num();

    while (!m.Step());

    for (unsigned i = 0; i < S0; ++i) {
      m.Step();
    }

    #pragma omp master
    m.Plot("start");

    #pragma omp for
    for (unsigned i = 0; i < S; ++i) {
      std::cerr << i << " ";
      m.Step();
      std::vector<int> bins = std::move(m.DistanceProfile());
      for (unsigned j = 0; j < histograms[thread_num].size(); ++j) {
        histograms.at(thread_num).at(j) += bins.at(j);
      }
    }

    #pragma omp master
    m.Plot("end");
  }
  return 0;
}