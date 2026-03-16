module;

#include <cstddef>
#include <iostream>
#include <vector>

export module perfect_cache.cli;
import perfect_cache;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
extern "C++" auto main() -> int {
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
    std::cout << "Incorrect input! \n";
    std::cout << e.what() << "\n";
  }

  auto lru = cpp_contests::perfect_cache(elements, cache_length * sizeof(int), 0);
  std::cout << lru << "\n";
  return 0;
}
#pragma GCC diagnostic pop
