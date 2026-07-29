module;

#include <cstddef>
#include <exception>
#include <fstream>
#include <iostream>
#include <span>
#include <string>
#include <vector>

export module missing_numbers.cli;
import missing_numbers;

namespace {
auto run(std::span<char *> args) -> int {
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
} // namespace

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
extern "C++" auto main(int argc, char **argv) -> int {
  try {
    return run(std::span(argv, static_cast<std::size_t>(argc)));
  } catch (std::exception const &e) {
    std::cerr << e.what() << "\n";
    return 1;
  }
}
#pragma GCC diagnostic pop
