#include <thread>
#include <iostream>
#include <vector>
#include <barrier.h>
#include <cstdlib> // For rand
#include <ctime>
#include <chrono>

Barrier b (std::max(1u, std::thread::hardware_concurrency()));

void ThreadWait() {
  int seconds = rand() % 11;
  std::chrono::seconds timespan(seconds );
  std::cout << "Sleep for " << seconds << " seconds." << std::endl;
  std::this_thread::sleep_for(timespan);

  std::cout << "Finished. Waiting..." << std::endl;
  b.wait();
  std::cout << "Continuing..." << std::endl;
}

int main() {
  const std::size_t num_thr = std::max(1u, std::thread::hardware_concurrency());
  srand(time(NULL));

  std::vector<std::thread> threads;
  for (unsigned i = 0; i < num_thr - 1; ++i) {
    threads.emplace_back(ThreadWait);
  }

  ThreadWait();
  for (auto& thread : threads) thread.join();

  return 0;
}