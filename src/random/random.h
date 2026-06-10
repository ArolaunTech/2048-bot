#include <random>
#include <cstdint>

#ifndef RANDOM_H
#define RANDOM_H

extern std::random_device rd;
extern std::mt19937 generator;
extern std::uniform_int_distribution<std::uint8_t> distribution;
extern std::uniform_int_distribution<int> big;
extern std::uniform_real_distribution<double> uniform;

#endif