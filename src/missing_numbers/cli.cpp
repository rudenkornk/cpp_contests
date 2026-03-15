#include <cstddef>
#include <fstream>
#include <iostream>
#include <span>
#include <string>
#include <vector>

#include "missing_numbers/solution.hpp"

auto main(int argc, char **argv) -> int {
  auto const args = std::span(argv, static_cast<std::size_t>(argc));

  if (args.size() < 2) {
    std::cerr << "Provide exactly one file input\n";
    return 1;
  }

  std::string const filename = args[1];
  auto input = std::ifstream{filename};
  if (!input.good()) {
    std::cerr << "Error opening '" << filename << "'\n";
    return 1;
  }
  std::vector<unsigned> numbers{};
  unsigned value = 0;
  while (input >> value) {
    numbers.push_back(value);
  }
  auto res = cpp_contests::missing_numbers(numbers);
  std::cout << res.first << " " << res.second;
  return 0;
}
