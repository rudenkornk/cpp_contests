export module two_queue_cache.cli;

import std;
import two_queue_cache;

namespace {
auto run() -> int {
  std::size_t cache_size = 0;
  std::size_t n_elements = 0;
  std::vector<int> elements{};

  std::cin.exceptions(std::ios_base::failbit);

  try {
    std::cin >> cache_size;
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

  constexpr std::size_t virtual_value_size_ = std::size_t{1024} * 1024 * 1024;
  constexpr std::size_t key_size = sizeof(int);

  auto two_queue = cpp_contests::two_queue_hits(elements, cache_size * (virtual_value_size_ + (key_size * 3) / 2),
                                                virtual_value_size_);
  std::println("{}", two_queue);
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
