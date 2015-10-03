#include <thread>
#include <utility>
#include <mutex>

class Barrier {
 public:
  explicit Barrier(int nthreads) : nthreads_(nthreads) {
																  counter_.first = 0;
																	barrier_reached_.first = false;
																	}

  void wait() {
    counter_.second.lock();
    ++counter_.first;
    counter_.second.unlock();
    while(!CheckExitStatus());

    std::lock_guard<std::mutex> lg (counter_.second);
    --counter_.first;
    if(counter_.first == 0)
      barrier_reached_.first = false;
  }

 private:
  bool CheckExitStatus() {
    std::lock_guard<std::mutex> lg (barrier_reached_.second);
    if(barrier_reached_.first || counter_.first == nthreads_) {
      barrier_reached_.first = true;
      return true;
    }
    return false;
  }
  int nthreads_;
  std::pair<bool, std::mutex> barrier_reached_;
  std::pair<int, std::mutex> counter_;

};
