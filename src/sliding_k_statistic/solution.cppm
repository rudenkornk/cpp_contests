module;

#include <cassert>
#include <cstddef>
#include <exception>
#include <iostream>
#include <print>
#include <queue>
#include <span>
#include <vector>

export module sliding_k_statistic;

namespace cpp_contests {
namespace {

auto constexpr kDebug = false;

// NOLINTBEGIN(misc-non-private-member-variables-in-classes)
struct Pos {
  int value;
  std::size_t index;

  auto operator<(Pos const &other) const -> bool { return std::tie(value, other.index) < std::tie(other.value, index); }
  auto operator>(Pos const &other) const -> bool { return !(operator<(other)); }
};
// NOLINTEND(misc-non-private-member-variables-in-classes)

using min_heap = std::priority_queue<Pos, std::vector<Pos>, std::greater<>>;
using max_heap = std::priority_queue<Pos>;

[[maybe_unused]] auto print_heap(max_heap const &heap) -> void {
  std::print("MaxHeap: ");
  auto copy = heap;
  while (!copy.empty()) {
    std::print("{} ", copy.top().value);
    copy.pop();
  }
  std::println();
}
[[maybe_unused]] auto print_heap(min_heap const &heap) -> void {
  std::print("MinHeap: ");
  auto copy = heap;
  while (!copy.empty()) {
    std::print("{} ", copy.top().value);
    copy.pop();
  }
  std::println();
}

auto keep_max(max_heap &max_heap, std::size_t left) {
  while (!max_heap.empty() && max_heap.top().index < left) {
    max_heap.pop();
  }
}

auto keep_min(min_heap &min_heap, std::size_t left) {
  while (!min_heap.empty() && min_heap.top().index < left) {
    min_heap.pop();
  }
}

auto transfuse(max_heap &max_heap, min_heap &min_heap, std::size_t &real_size, std::size_t left) {
  auto top = max_heap.top();
  max_heap.pop();
  --real_size;
  min_heap.push(top);
  keep_max(max_heap, left);
}

auto transfuse_back(max_heap &max_heap, min_heap &min_heap, std::size_t &real_size, std::size_t left) -> bool {
  if (min_heap.empty()) {
    return false;
  }
  auto top = min_heap.top();
  min_heap.pop();
  max_heap.push(top);
  ++real_size;
  keep_min(min_heap, left);
  return true;
}

// NOLINTNEXTLINE(readability-function-size)
auto add(max_heap &max_heap, min_heap &min_heap, std::size_t &real_size, std::size_t k_len, Pos element,
         std::size_t left) -> void {
  if (real_size < k_len) {
    max_heap.push(element);
    ++real_size;
    return;
  }
  if (element.value >= max_heap.top().value) {
    min_heap.push(element);
    return;
  }
  transfuse(max_heap, min_heap, real_size, left);
  max_heap.push(element);
  ++real_size;
}

// NOLINTNEXTLINE(readability-function-size)
auto remove(max_heap &max_heap, min_heap &min_heap, std::size_t &real_size, Pos element, std::size_t left) -> void {
  if (element.value > max_heap.top().value) {
    keep_min(min_heap, left);
    return;
  }
  if (element.value < max_heap.top().value) {
    --real_size;
    transfuse_back(max_heap, min_heap, real_size, left);
    return;
  }

  // Top element is equal to removed element.value.
  // By construction top element of max_heap and min_heap is always valid.
  // Thus, if we find its index invalid, it must the exact element we are
  // removing.
  if (max_heap.top().index < left) {
    max_heap.pop();
    --real_size;
    keep_max(max_heap, left);
    transfuse_back(max_heap, min_heap, real_size, left);
    return;
  }
  // Top element of max_heap is valid.
  // It is possible that top element of min_heap became invalid then.
  keep_min(min_heap, left);
}

auto solve(std::vector<int> const &nums, std::vector<bool> const &is_forward, std::size_t k_len) -> void {
  auto max_heap = cpp_contests::max_heap();
  auto min_heap = cpp_contests::min_heap();
  std::size_t left = 0;
  std::size_t right = 0;
  std::size_t real_size = 1;
  max_heap.push(Pos{.value = nums[0], .index = 0});

  for (auto &&is_right : is_forward) {
    if (is_right) {
      ++right;
      auto add_element = Pos{.value = nums[right], .index = right};
      add(max_heap, min_heap, real_size, k_len, add_element, left);
    } else {
      auto remove_element = Pos{.value = nums[left], .index = left};
      ++left;
      remove(max_heap, min_heap, real_size, remove_element, left);
    }
    if (kDebug) {
      std::println("real_size: {}", real_size);
      print_heap(max_heap);
      print_heap(min_heap);
    }

    if (real_size == k_len) {
      std::println("{}", max_heap.top().value);
    } else {
      std::println("-1");
    }
  }
}

auto run([[maybe_unused]] std::span<char *> args) -> void {
  std::size_t n_len = 0;
  std::size_t m_len = 0;
  std::size_t k_len = 0;

  std::cin >> n_len >> m_len >> k_len;

  auto nums = std::vector<int>(n_len);
  auto is_forward = std::vector<bool>(m_len);
  for (std::size_t idx = 0; idx < n_len; ++idx) {
    std::cin >> nums[idx];
  }
  for (std::size_t idx = 0; idx < m_len; ++idx) {
    char x = 0;
    std::cin >> x;
    is_forward[idx] = (x == 'R');
  }

  solve(nums, is_forward, k_len);
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
