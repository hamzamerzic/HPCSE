#include <cmath>
#include <iostream>
#include <thread>
#include <future>
#include <iomanip>
#include <chrono>
#include <vector>
#include <algorithm>
#include <utility>

double func(double x) {
  return std::log(x) * std::sqrt(x);
}

void RiemannIntegral(double (*f)(double), double a, double b, std::size_t N,
    std::pair<double, std::mutex>& res) {
  double sum = 0;
  double step = (b - a)/N;

  for (unsigned i = 0; i < N; ++i)
    sum += f(a + i * step + step / 2);  // Riemann version, middle-point

  std::lock_guard<std::mutex> lg (res.second);
  res.first += sum * step;
}

void TestRiemannThreads(double (*f)(double), double a, double b, std::size_t N,
    std::size_t num_thr) {
  auto start_t = std::chrono::steady_clock::now();

  std::vector<std::thread> threads;

  std::pair<double, std::mutex> res; res.first = 0;

  for (unsigned i = 0; i < num_thr - 1; ++i)
    threads.emplace_back(RiemannIntegral, f, a + (b - a)/num_thr * i,
        a + (b - a)/num_thr * (i + 1), N / num_thr, std::ref(res));

  RiemannIntegral(f, a + (b - a)/num_thr * (num_thr - 1), b, N / num_thr, res);
  for (auto& thread : threads) thread.join();

  auto end_t = std::chrono::steady_clock::now();
  auto duration = end_t - start_t;
  double elapsed_t = std::chrono::duration <double, std::milli>
      (duration).count();

  std::cout << "--------------------\nResult: " << std::setprecision(10) <<
    res.first << std::endl << "Threads: " << num_thr << std::endl <<
    "Duration: " << elapsed_t << " ms" << std::endl;
}

int main() {
  double a = 1, b = 4;
  std::size_t N = 10000000;

  const std::size_t num_thr = std::max(1u, std::thread::hardware_concurrency());
  std::cout << "Total number of threads: " << num_thr << std::endl;

  for (unsigned i = 1; i <= num_thr; ++i) {
    TestRiemannThreads(func, a, b, N, i);
  }

  return 0;
}