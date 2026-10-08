module;

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <functional>
#include <iostream>
#include <print>
#include <span>
#include <vector>

export module two_pointers_max;

namespace cpp_contests {
namespace {

// NOLINTNEXTLINE(readability-function-size)
auto solve(std::span<int32_t const> nums, std::vector<bool> const &right) -> void {
  std::size_t l_ptr = 0;
  std::size_t r_ptr = 0;

  // maxs_begin is used to track head of maxs instead of erasing head element,
  // which causes O(n) element shift.
  std::size_t maxs_begin = 0;
  std::vector<std::size_t> maxs{maxs_begin};
  for (auto next_right : right) {
    if (next_right) {
      ++r_ptr;
      auto el = nums[r_ptr];
      auto lower =
          std::ranges::lower_bound(maxs.begin() + static_cast<int64_t>(maxs_begin), maxs.end(), el,
                                   std::greater<int32_t>{}, [&nums](std::size_t idx) -> int { return nums[idx]; });
      maxs.erase(lower, maxs.end());
      maxs.push_back(r_ptr);
    } else {
      if (maxs[maxs_begin] == l_ptr) {
        ++maxs_begin;
      }
      ++l_ptr;
    }
    std::print("{} ", nums[maxs[maxs_begin]]);
  }
}

auto run([[maybe_unused]] std::span<char *> args) -> void {
  std::size_t n_len = 0;
  std::size_t m_len = 0;

  std::cin >> n_len;

  std::vector<int32_t> nums(n_len);

  for (std::size_t i = 0; i < n_len; ++i) {
    std::cin >> nums[i];
  }
  std::cin >> m_len;
  std::vector<bool> right(m_len);
  for (std::size_t i = 0; i < m_len; ++i) {
    char c_ptr = ' ';
    std::cin >> c_ptr;
    right[i] = c_ptr == 'R';
  }

  solve(nums, right);
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
