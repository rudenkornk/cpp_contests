#pragma once

#include <boost/algorithm/cxx17/reduce.hpp>
#include <boost/range/numeric.hpp>
#include <cassert>
#include <climits>
#include <utility>
#include <vector>

namespace cpp_contests {

auto inline missing_numbers(std::vector<unsigned> const &nums) -> std::pair<unsigned, unsigned> {
  assert(nums.size() + 2 < (1U << sizeof(unsigned) * CHAR_BIT / 2));
  auto const len = static_cast<unsigned>(nums.size() + 1);
  unsigned const total_sum = ((len + 1) * len) / 2;
  unsigned const missing_sum = total_sum - boost::algorithm::reduce(nums);
  unsigned const half_missing_sum = (missing_sum - 1) / 2;
  unsigned const half_sum =
      boost::accumulate(nums, 0, [half_missing_sum](unsigned left, unsigned right) noexcept -> unsigned int {
        right = (right <= half_missing_sum) ? right : 0;
        return left + right;
      });
  unsigned const total_half_sum = ((half_missing_sum + 1) * half_missing_sum) / 2;
  unsigned const miss_1 = total_half_sum - half_sum;
  unsigned const miss_2 = missing_sum - miss_1;
  return {miss_1, miss_2};
}

} // namespace cpp_contests
