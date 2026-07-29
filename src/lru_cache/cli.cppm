module;

#include <cstddef>
#include <exception>
#include <iostream>
#include <print>
#include <vector>

export module lru_cache.cli;
import lru_cache;

namespace {
auto run() -> int {
  std::size_t cache_length = 0;
  std::size_t n_elements = 0;
  std::vector<int> elements{};

  std::cin.exceptions(std::ios_base::failbit);

  try {
    std::cin >> cache_length;
    std::cin >> n_elements;
    elements.reserve(n_elements);
    for (std::size_t i = 0; i < n_elements; ++i) {
      int n_el = 0;
      std::cin >> n_el;
      elements.push_back(n_el);
    }
  } catch (std::ios_base::failure const &e) {
    std::println("Incorrect input!");
    std::println("{}", e.what());
  }

  auto lru = cpp_contests::lru_hits(elements, cache_length * sizeof(int), 0);
  std::println("{}", lru);
  return 0;
}
} // namespace

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
extern "C++" auto main() -> int {
  try {
    return run();
  } catch (std::exception const &e) {
    std::cerr << e.what() << "\n";
    return 1;
  }
}
#pragma GCC diagnostic pop
