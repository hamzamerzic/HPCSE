#ifndef BARRIER_H
#define BARRIER_H

#include <thread>
#include <atomic>

class Barrier {
 public:
  explicit Barrier(int nthreads)
      : nthreads_(nthreads), counter_(0), barrier_reached_(false) {}

  void wait() {
    // In case not all threads left
    while (barrier_reached_.load(std::memory_order_acquire));

    ++counter_;  // Each thread enters and increments counter

    if (counter_ == nthreads_) {
      barrier_reached_.store(true, std::memory_order_release);
    }

    while (!barrier_reached_.load(std::memory_order_acquire));

    --counter_;

    // When last thread leaves, the counter is zero
    if (counter_ == 0) {
      // Resets the barrier_reached -> initial state of the barrier
      barrier_reached_.store(false, std::memory_order_release);
    }
  }

 private:
  const int nthreads_;
  std::atomic<bool> barrier_reached_;
  std::atomic<int> counter_;
};

#endif  // BARRIER_H
