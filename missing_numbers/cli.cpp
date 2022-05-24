#include <fstream>
#include <iostream>
#include <vector>

#include "solution.hpp"

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cerr << "Provide exactly one file input\n";
    return 1;
  }
  std::string filename = argv[1];
  auto input = std::ifstream{filename};
  if (!input.good()) {
    std::cerr << "Error opening '" << filename << "'\n";
    return 1;
  }
  std::vector<unsigned> numbers{};
  unsigned value = 0;
  while (input >> value)
    numbers.push_back(value);
  auto res = missing_numbers(numbers);
  std::cout << res.first << " " << res.second;
  return 0;
}
