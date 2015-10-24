#include <iostream>
#include <omp.h>

int main() {
  #pragma omp parallel
  {
    // now we execute this block in multiple threads
    #pragma omp critical (output)
    {
      std::cout << "I am thread " << omp_get_thread_num()
        << " of " << omp_get_num_threads() << " threads." << std::endl;
    }
  }

  #pragma omp parallel
  {
    for(int i = 0; i < 10; ++i) {
      #pragma omp for
      for (int j = 0; j < 10; ++j)
        std::cout << 10 * i + j << std::endl;
      #pragma omp single
      std::cout << "One down" << std::endl;
    }
  }
}