#include <thread>
#include <vector>
#include <iostream>
#include "barrier.h"

using namespace std;

Barrier b (4);

void CalculateParallel(int rand_) {
  srand(rand_);
  double t = 0.;
  while (t < 1.) {
    double d = 0;
    for (unsigned i = 0; i < rand() % 2000000; ++i) {
      d += 0.1;
      d *= 2.3;
    }
    b.wait();
    t += 1e-3;
    b.wait();
    std::cerr << t << std::endl;
  }
}

int main() {
  vector<thread> threads (3);
  for (unsigned i = 0; i < 3; ++i) {
    threads[i] = thread(CalculateParallel, rand() * (i + 1));
  }
  CalculateParallel(rand());
  for(auto& i : threads) {
    i.join();
  }
  return 0;
}