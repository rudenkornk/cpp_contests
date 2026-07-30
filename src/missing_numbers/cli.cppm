export module missing_numbers.cli;

import std;
import missing_numbers;

namespace {
auto run(std::span<char *> args) -> int {
  if (args.size() < 2) {
    std::println(std::cerr, "Provide exactly one file input");
    return 1;
  }

  std::string const filename = args[1];
  auto input = std::ifstream{filename};
  if (!input.good()) {
    std::println(std::cerr, "Error opening '{}'", filename);
    return 1;
  }
  std::vector<unsigned> numbers{};
  unsigned value = 0;
  while (input >> value) {
    numbers.push_back(value);
  }
  auto res = cpp_contests::missing_numbers(numbers);
  std::println("{} {}", res.first, res.second);
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
