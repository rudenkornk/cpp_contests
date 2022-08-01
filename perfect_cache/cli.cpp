#include <iostream>
#include <vector>

#include "perfect_cache/solution.hpp"

#include <algorithm>
#include <iterator>

int main() {
  std::size_t cache_length = 0;
  std::size_t n_elements = 0;
  std::cin >> cache_length;
  std::cin >> n_elements;
  std::vector<int> elements{};
  elements.reserve(n_elements);
  for (std::size_t i = 0; i < n_elements; ++i) {
    int n = 0;
    std::cin >> n;
    elements.push_back(n);
  }

  auto lru = cpp_contests::perfect_cache(elements, cache_length * sizeof(int), 0);
  std::cout << lru << "\n";
  return 0;
}
