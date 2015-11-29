#include <x86intrin.h>
#include <iostream>
#include <chrono>

int main () {
  int N = 1024000/5;
  int M = 10000;
  alignas(64) double x[N];
  alignas(64) double y[N];
  alignas(64) double z[N];
  for (int i = 0; i < N; ++i) {
    x[i] = i;
    y[i] = i/2.;
    z[i] = 2000;
  }
  std::cerr << "Vectorized..." << std::endl;
  for (int k = 0; k < M; ++k)
    for (int i = 0; i < N; i += 4) {
      auto a = _mm256_load_pd(&x[i]);
      auto b = _mm256_load_pd(&y[i]);
      auto s = _mm256_add_pd(a, b);
      _mm256_stream_pd(&z[i], _mm256_mul_pd(s, s));
    }
  std::cerr << "Done!" << std::endl;
  std::cerr << "Serial..." << std::endl;
  for (int k = 0; k < M; ++k)
    for (int i = 0; i < N; ++i) {
      z[i] = x[i] * x[i] + y[i] * y[i] + 2 * x[i] * y[i];
    }
  std::cerr << "Done!" << std::endl;
  return 0;
}
