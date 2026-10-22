module;

#include <sys/types.h>

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <iostream>
#include <print>
#include <span>
#include <vector>

export module bounded_knapsack;

namespace cpp_contests {
namespace {

struct Group {
  uint32_t weight;
  uint32_t kount; // Use such name to match problem description.
  uint32_t cost;
};

struct MaxQEntry {
  int64_t cost;
  std::size_t idx;
};

struct MaxQ {
  std::size_t begin = 0;
  std::size_t size_limit = 0;
  uint32_t stride = 0;
  std::vector<MaxQEntry> entries;
};

auto max_q_add(MaxQ &max_q, MaxQEntry entry) -> MaxQEntry {
  while (max_q.entries.size() > max_q.begin && max_q.entries.back().cost <= entry.cost) {
    max_q.entries.pop_back();
  }
  max_q.entries.push_back(entry);
  if (max_q.entries[max_q.begin].idx + (max_q.stride * max_q.size_limit) < entry.idx) {
    ++max_q.begin;
  }
  return max_q.entries[max_q.begin];
}

// NOLINTNEXTLINE(readability-function-size)
auto solve(uint32_t s_size, std::span<Group> groups) -> uint64_t {
  using std::swap;
  auto current = std::vector<int64_t>(s_size + 1, 0);
  auto next = std::vector<int64_t>(s_size + 1, 0);

  auto max_q = MaxQ{};
  max_q.entries.reserve(s_size + 1);
  for (auto &&group : groups) {
    max_q.stride = group.weight;
    max_q.size_limit = group.kount;
    for (uint32_t residual = 0; residual < group.weight && residual <= s_size; ++residual) {
      max_q.entries.clear();
      max_q.begin = 0;
      for (std::size_t idx = residual; idx <= s_size; idx += group.weight) {
        int64_t const cost = current[idx] - static_cast<int64_t>((idx / group.weight) * group.cost);
        auto max = max_q_add(max_q, MaxQEntry{.cost = cost, .idx = idx});
        next[idx] = current[max.idx] + static_cast<int64_t>((idx - max.idx) / group.weight * group.cost);
      }
    }
    swap(current, next);
  }

  return current[s_size];
}

auto run([[maybe_unused]] std::span<char *> args) -> void {
  std::size_t n_len = 0;
  std::size_t s_size = 0;

  std::cin >> n_len >> s_size;
  std::vector<Group> arr(n_len);
  for (std::size_t i = 0; i < n_len; ++i) {
    std::cin >> arr[i].weight >> arr[i].cost >> arr[i].kount;
  }
  std::println("{}", solve(s_size, arr));
}

} // namespace
} // namespace cpp_contests

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
extern "C++" auto main(int argc, char **argv) -> int {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  try {
    cpp_contests::run(std::span(argv, static_cast<std::size_t>(argc)));
  } catch (std::exception const &e) {
    std::cerr << e.what() << "\n";
    return 1;
  }
}
#pragma GCC diagnostic pop
