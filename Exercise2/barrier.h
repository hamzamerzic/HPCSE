#ifndef BARRIER_H
#define BARRIER_H

#include <thread>
#include <mutex>

class Barrier {
 public:
  explicit Barrier(int nthreads)
      : nthreads_(nthreads), counter_(0), barrier_reached_(false) {}

  void wait() {
    while (barrier_reached_);  // In case not all threads left

    b_mutex_.lock();
    ++counter_;  // Each thread enters and increments counter

    if (counter_ == nthreads_) {
      barrier_reached_ = true;
    }
    b_mutex_.unlock();

    while (!barrier_reached_);

    b_mutex_.lock();
    --counter_;

    // When last thread leaves, the counter is zero
    if (counter_ == 0) {
      // Resets the barrier_reached -> initial state of the barrier
      barrier_reached_ = false;
    }
    b_mutex_.unlock();
  }

 private:
  const int nthreads_;
  volatile bool barrier_reached_;
  int counter_;
  std::mutex b_mutex_;
};

#endif  // BARRIER_H
