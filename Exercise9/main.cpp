/* File:   main.cpp */
/* Copyright 2015 ETH Zurich. All Rights Reserved. */
// STL
#include <string>
#include <iostream>
#include <fstream>
#include <chrono>
#include <numeric>

// Our stuff
#include "types.h"
#include "LennardJones.h"

void load_particles(const std::string& fname, configuration_type& c, const size_type Np=1000);


int main(int argc, char* argv[])
{
  configuration_type config;
  LennardJones LJ(0.1, 5.0);

  load_particles("particles_SP.dat", config);

  const size_type N = config.first.size();

  std::chrono::time_point<std::chrono::high_resolution_clock> start, end;

  std::pair<value_type,value_type> new_pos;
  new_pos.first  = config.first[N-1]  + 0.01;
  new_pos.second = config.second[N-1] + 0.01;

  start = std::chrono::high_resolution_clock::now();

  value_type energy_diff = 0.0;

  for (int i = 0; i < 100; ++i)
  {
#ifndef _AVX256_
    energy_diff += LJ.diff(config, new_pos);
#else
    energy_diff += LJ.diff_avx(config, new_pos);
#endif /* _AVX256_ */
  }


  end = std::chrono::high_resolution_clock::now();

  std::cout << "## Results ##"  << std::endl;
  std::cout << "Result: " << energy_diff  << std::endl;

  const double elapsed_seconds = std::chrono::duration<double>(end-start).count();
  std::cout << std::endl << "Job done in "<< elapsed_seconds << " seconds." << std::endl;

  return 0;
}


/* void load_particles(const std::string& fname, configuration_type& c, const size_type Np=1000) */
void load_particles(const std::string& fname, configuration_type& c, const size_type Np)
{
    const size_type N = Np*Np + (Np-1)*(Np-1);
    float * buf = new float[2*N];
    std::ifstream ifs(fname);
    ifs.read((char*)buf, 2*N*sizeof(float));
    ifs.close();
    for (size_type i = 0; i < 2*N; i+=2)
    {
        c.first.push_back(buf[i]);
        c.second.push_back(buf[i+1]);
    }
    delete[] buf;
    std::cout << "Loaded " << N << " particles from " << fname << "." << std::endl;
}
