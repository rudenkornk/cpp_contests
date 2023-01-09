#include <iostream>
#include <vector>

#include "lru_cache/solution.hpp"

#include <algorithm>
#include <iterator>

int main() {
  std::size_t cache_length = 0;
  std::size_t n_elements = 0;
  std::vector<int> elements{};

  std::cin.exceptions(std::ios_base::failbit);

  try {
    std::cin >> cache_length;
    std::cin >> n_elements;
    elements.reserve(n_elements);
    for (std::size_t i = 0; i < n_elements; ++i) {
      int n = 0;
      std::cin >> n;
      elements.push_back(n);
    }
  } catch (std::ios_base::failure const &e) {
    std::cout << "Incorrect input! \n";
    std::cout << e.what() << "\n";
  }

  auto lru = cpp_contests::lru_hits(elements, cache_length * sizeof(int), 0);
  std::cout << lru << "\n";
  return 0;
}
