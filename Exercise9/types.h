/* File:   types.h */
/* Copyright 2015 ETH Zurich. All Rights Reserved. */
#ifndef TYPES_H_FGO79RJB
#define TYPES_H_FGO79RJB

#include <cstddef>
#include <utility>
#include <vector>


// to ensure 64-Byte alignment for AVX 256-bit registers
#include "aligned_allocator.hpp"

typedef float value_type;
typedef std::size_t size_type;

typedef std::pair<std::vector<value_type, hpc15::aligned_allocator<value_type,64> >, std::vector<value_type, hpc15::aligned_allocator<value_type,64> > > configuration_type;
// first = x, second = y

#endif /* TYPES_H_FGO79RJB */
