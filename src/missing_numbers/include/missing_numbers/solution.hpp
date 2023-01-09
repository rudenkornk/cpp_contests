#pragma once

#include <cassert>
#include <climits>
#include <numeric>
#include <vector>

namespace cpp_contests {

std::pair<unsigned, unsigned> inline missing_numbers(
    std::vector<unsigned> const &nums) {
  assert(nums.size() + 2 < (1u << sizeof(unsigned) * CHAR_BIT / 2));
  unsigned n = static_cast<unsigned>(nums.size() + 1);
  unsigned total_sum = ((n + 1) * n) / 2;
  unsigned missing_sum = total_sum - std::reduce(nums.begin(), nums.end());
  unsigned half_missing_sum = (missing_sum - 1) / 2;
  unsigned half_sum = std::accumulate(
      nums.begin(), nums.end(), 0,
      [half_missing_sum](unsigned left, unsigned right) noexcept {
        right = (right <= half_missing_sum) ? right : 0;
        return left + right;
      });
  unsigned total_half_sum = ((half_missing_sum + 1) * half_missing_sum) / 2;
  unsigned x = total_half_sum - half_sum;
  unsigned y = missing_sum - x;
  return {x, y};
}

} // namespace cpp_contests
