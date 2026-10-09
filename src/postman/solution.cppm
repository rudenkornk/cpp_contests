module;

#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <iostream>
#include <print>
#include <span>
#include <utility>
#include <vector>

export module postman;

namespace cpp_contests {
namespace {

using value_type = unsigned int;
using container_type = std::vector<value_type>;

auto sift_up(std::span<value_type> arr, std::size_t idx) -> void {
  using std::swap;
  while (idx > 0) {
    auto parent = (idx - 1) / 2;
    if (arr[parent] >= arr[idx]) {
      break;
    }
    swap(arr[parent], arr[idx]);
    idx = parent;
  }
}

auto sift_down(std::span<value_type> arr, std::size_t idx) -> void {
  using std::swap;
  const auto len = arr.size();

  while (idx < len / 2) {
    auto child1 = (2 * idx) + 1;
    auto child2 = (2 * idx) + 2;
    auto preferred_child = child1;
    if (child2 < len) {
      preferred_child = arr[child1] < arr[child2] ? child2 : child1;
    }

    if (arr[idx] >= arr[preferred_child]) {
      break;
    }
    swap(arr[idx], arr[preferred_child]);
    idx = preferred_child;
  }
}

auto make_heap(std::span<value_type> arr) -> void {
  auto idx = arr.size() / 2;
  while (idx > 0) {
    --idx;
    sift_down(arr, idx);
  }
}

auto push_heap(container_type &arr, value_type value) -> void {
  arr.push_back(value);
  sift_up(arr, arr.size() - 1);
}

auto pop_heap(container_type &arr) -> void {
  assert(!arr.empty());
  using std::swap;
  swap(arr.front(), arr.back());
  arr.pop_back();
  if (!arr.empty()) {
    sift_down(arr, 0);
  }
}

auto insertion_sort(std::span<value_type> arr) -> void {
  auto len = arr.size();
  for (std::size_t i = 0; i < len; ++i) {
    auto el = arr[i];
    auto idx = i;

    while (idx > 0 && arr[idx - 1] > el) {
      arr[idx] = arr[idx - 1];
      --idx;
    }

    arr[idx] = el;
  }
}

auto heap_sort(std::span<value_type> arr) -> void {
  make_heap(arr);
  using std::swap;
  for (auto heap_size = arr.size(); heap_size > 1; --heap_size) {
    swap(arr[0], arr[heap_size - 1]);
    sift_down(arr.first(heap_size - 1), 0);
  }
}

auto pivot(std::span<value_type> arr) -> value_type {
  using std::swap;

  auto left = arr.front();
  auto right = arr.back();
  auto middle = arr[arr.size() / 2];

  if (middle < left) {
    swap(left, middle);
  }
  if (right < middle) {
    swap(middle, right);
  }
  if (middle < left) {
    swap(left, middle);
  }
  return middle;
}

auto partition(std::span<value_type> arr, value_type pivot_value) -> std::size_t {
  using std::swap;
  std::size_t lhs = 0;
  if (arr.empty()) {
    return lhs;
  }
  std::size_t rhs = arr.size() - 1;

  while (true) {
    while (arr[lhs] < pivot_value) {
      ++lhs;
    }
    while (arr[rhs] > pivot_value) {
      --rhs;
    }
    if (lhs >= rhs) {
      return rhs + 1;
    }
    swap(arr[lhs], arr[rhs]);
    ++lhs;
    --rhs;
  }
}

auto constexpr kInsertionSortLimit = 64;
auto quick_sort(std::span<value_type> arr) -> void { // NOLINT(misc-no-recursion)
  using std::swap;
  while (arr.size() > 1) {
    if (arr.size() <= kInsertionSortLimit) {
      insertion_sort(arr);
      return;
    }

    auto pivot_value = pivot(arr);
    auto split = partition(arr, pivot_value);
    auto smaller = arr.first(split);
    arr = arr.subspan(split);
    if (smaller.size() > arr.size()) {
      swap(smaller, arr);
    }
    quick_sort(smaller);
  }
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
auto counting_byte_sort(std::span<value_type> src, std::span<value_type> dst, std::uint8_t byte) -> void {
  auto constexpr bits = 8U;
  auto constexpr buckets = 1U << bits;
  auto counts = std::array<std::size_t, buckets>{};
  for (auto el : src) {
    auto el_byte = static_cast<std::uint8_t>(el >> (byte * bits));
    ++counts.at(el_byte);
  }
  for (std::size_t idx = 1; idx < buckets; ++idx) {
    counts.at(idx) += counts.at(idx - 1);
  }

  for (std::size_t idx = src.size(); idx > 0; --idx) {
    auto el = src[idx - 1];
    auto el_byte = static_cast<std::uint8_t>(el >> (byte * bits));
    --counts.at(el_byte);
    dst[counts.at(el_byte)] = el;
  }
}

auto radix_sort(container_type &arr) -> void {
  auto constexpr bytes = sizeof(value_type);
  auto buf = container_type(arr.size());
  for (std::uint8_t byte = 0; byte < bytes; ++byte) {
    counting_byte_sort(arr, buf, byte);
    std::swap(arr, buf);
  }
}

auto solve(container_type &nums) -> std::uint64_t {
  radix_sort(nums);
  std::uint64_t result = 0;
  auto len = nums.size();
  for (std::size_t i = 0; i < len / 2; ++i) {
    result += (nums[len - i - 1] - nums[i]);
  }

  return result;
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
auto generate_arr(std::size_t n_len, value_type a_param, value_type b_param) -> container_type {
  auto constexpr shift = 8U;
  auto nums = container_type{};
  nums.reserve(n_len);

  value_type cur = 0;
  for (std::size_t i = 0; i < n_len; ++i) {
    cur = (cur * a_param) + b_param;
    auto a_res = cur >> shift;
    cur = (cur * a_param) + b_param;
    auto b_res = cur >> shift;
    nums.push_back((a_res << shift) ^ b_res);
  }
  return nums;
}

auto run([[maybe_unused]] std::span<char *> args) -> void {
  std::size_t n_len = 0;
  value_type a_param = 0;
  value_type b_param = 0;

  std::cin >> n_len >> a_param >> b_param;
  auto arr = generate_arr(n_len, a_param, b_param);
  auto ans = solve(arr);
  std::println("{}", ans);

  auto dummy = container_type{};
  heap_sort(dummy);
  push_heap(dummy, 0);
  pop_heap(dummy);
  quick_sort(dummy);
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
