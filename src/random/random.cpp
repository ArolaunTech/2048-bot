#include <random>
#include <cstdint>
#include <limits>

std::random_device rd;
std::mt19937 generator(rd());
std::uniform_int_distribution<std::uint8_t> distribution(0, 17);
std::uniform_int_distribution<int> big(0, std::numeric_limits<int>::max());
std::uniform_real_distribution<double> uniform(0, 1);