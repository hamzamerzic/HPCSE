#ifndef BARRIER_H
#define BARRIER_H

#include <thread>
#include <mutex>
#include <atomic>

class Barrier {
 public:
  explicit Barrier(int nthreads)
      : nthreads_(nthreads), counter_(0), barrier_reached_(false) {}

  void wait() {
    while (barrier_reached_.load());  // In case not all threads left

    b_mutex_.lock();
    ++counter_;  // Each thread enters and increments counter

    if (counter_ == nthreads_) {
      barrier_reached_.store(true);
    }
    b_mutex_.unlock();

    while (!barrier_reached_.load());

    b_mutex_.lock();
    --counter_;

    // When last thread leaves, the counter is zero
    if (counter_ == 0) {
      // Resets the barrier_reached -> initial state of the barrier
      barrier_reached_.store(false);
    }
    b_mutex_.unlock();
  }

 private:
  const int nthreads_;
  std::atomic<bool> barrier_reached_;
  int counter_;
  std::mutex b_mutex_;
};

#endif  // BARRIER_H
