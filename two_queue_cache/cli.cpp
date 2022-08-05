#include <iostream>
#include <vector>

#include "two_queue_cache/solution.hpp"

#include <algorithm>
#include <iterator>

int main() {
  std::size_t cache_size = 0;
  std::size_t n_elements = 0;
  std::vector<int> elements{};

  std::cin.exceptions(std::ios_base::failbit);

  try {
    std::cin >> cache_size;
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

  constexpr std::size_t virtual_value_size_ = std::size_t{1024} * 1024 * 1024;
  constexpr std::size_t key_size = sizeof(int);

  auto two_queue = cpp_contests::two_queue_hits(
      elements, cache_size * (virtual_value_size_ + (key_size * 3) / 2),
      virtual_value_size_);
  std::cout << two_queue << "\n";
  return 0;
}
