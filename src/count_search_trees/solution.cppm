module;

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <iostream>
#include <print>
#include <span>
#include <unordered_map>
#include <vector>

export module count_search_trees;

namespace cpp_contests {
namespace {

auto constexpr kModulo = 123'456'789;

using counter = uint16_t;
using counters = std::vector<counter>;

struct VectorHasher {
  auto operator()(const counters& vec) const -> std::size_t {
    auto constexpr hash_seed = 0x9e3779b9;
    auto constexpr hash_shift = 6U;
    std::size_t seed = vec.size();
    for (auto x : vec) {
      // Classic hash combining algorithm from Boost.
      seed ^= std::hash<counter>{}(x) + hash_seed + (seed << hash_shift) + (seed >> 2U);
    }
    return seed;
  }
};

using cache_type = std::unordered_map<counters, uint64_t, VectorHasher>;

auto arr_to_counts(std::span<int> const arr) -> counters {
  auto counts = counters{};

  auto last = arr.front();
  std::size_t count = 0;
  for (auto&& num : arr) {
    if (num == last) {
      ++count;
    } else {
      counts.push_back(count);
      last = num;
      count = 1;
    }
  }
  counts.push_back(count);
  return counts;
}

auto split(counters const& counts, std::size_t idx) -> std::pair<counters, counters> {
  auto left = counters(counts.begin(), counts.begin() + static_cast<counters::difference_type>(idx));

  auto right_idx = idx + static_cast<std::size_t>(counts[idx] == 1);
  auto right = counters(counts.begin() + static_cast<counters::difference_type>(right_idx), counts.end());
  if (counts[idx] > 1) {
    right[0] -= 1;
  }

  return {left, right};
}

auto trees(counters const& counts, cache_type& cache) -> uint64_t { // NOLINT(misc-no-recursion)
  if (counts.size() <= 1) {
    return 1;
  }
  if (cache.contains(counts)) {
    return cache[counts];
  }

  auto total = uint64_t{0};
  for (std::size_t idx = 0; idx < counts.size(); ++idx) {
    auto [left, right] = split(counts, idx);
    total += trees(left, cache) * trees(right, cache);
    total %= kModulo;
  }
  cache[counts] = total;
  return total;
}

auto solve(std::span<int> const arr) -> void {
  std::ranges::sort(arr);
  auto counts = arr_to_counts(arr);
  auto cache = cache_type{};
  std::println("{}", trees(counts, cache));
}

auto run([[maybe_unused]] std::span<char*> args) -> void {
  std::size_t n_len = 0;

  std::cin >> n_len;
  auto arr = std::vector<int>(n_len);

  for (std::size_t idx = 0; idx < n_len; ++idx) {
    std::cin >> arr[idx];
  }

  solve(arr);
}

} // namespace
} // namespace cpp_contests

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
extern "C++" auto main(int argc, char** argv) -> int {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  try {
    cpp_contests::run(std::span(argv, static_cast<std::size_t>(argc)));
  } catch (std::exception const& e) {
    std::cerr << e.what() << "\n";
    return 1;
  }
}
#pragma GCC diagnostic pop
