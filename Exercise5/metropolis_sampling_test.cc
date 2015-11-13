#include "metropolis_sampling.h"

int main() {
  double L = 1.;
  unsigned Nx = 100;
  unsigned Ny = 115;
  double d0 = 0.875 * L / Nx;
  double alpha = L/Nx - d0;
  unsigned S0 = 100;
  unsigned S = 500;
  MetropolisSampling m (L, Nx, Ny, d0, alpha, S0, S);
  m.Simulate();
  return 0;
}