module;

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <iostream>
#include <numeric>
#include <print>
#include <span>
#include <vector>

export module football_team;

namespace cpp_contests {
namespace {

struct Score {
  int32_t eff;
  std::size_t idx;
};

/// Using exponential search find the first index i such that
/// target <= nums[i] + nums[i + 1];
auto exp_search(std::span<Score> nums, int32_t target) -> std::span<Score>::iterator {
  if (nums.size() <= 2) {
    return nums.begin();
  }

  std::size_t diff = 1;
  auto search_start = nums.begin();
  auto search_end = search_start + 1;
  int32_t const half_tgt = (target / 2) + (target % 2);

  while (search_end->eff < half_tgt) {
    search_start = search_end;
    search_end = std::min(search_end + static_cast<std::ptrdiff_t>(diff), nums.end() - 1);
    diff *= 2;
  }

  auto lower = std::ranges::lower_bound(search_start, search_end + 1, half_tgt, {}, &Score::eff);

  // Check edge case where element < target / 2, but still qualifies.
  if (lower > nums.begin() && ((lower - 1)->eff >= (target - lower->eff))) {
    return lower - 1;
  }

  return lower;
}

auto solve(std::span<Score> nums) -> void {
  std::ranges::sort(nums, {}, &Score::eff);
  auto last_begin = nums.begin();
  auto max_begin = nums.begin();
  auto max_end = nums.end();
  std::uint64_t max_score = 0;
  std::uint64_t curr_score = 0;

  for (auto it = nums.begin(); it < nums.end(); ++it) {
    auto count = (it - last_begin) + 1;
    auto begin = exp_search(nums.subspan(last_begin - nums.begin(), count), it->eff);
    curr_score += it->eff;
    curr_score = std::accumulate(last_begin, begin, curr_score,
                                 [](int64_t score, const auto &element) -> auto { return score - element.eff; });
    if (curr_score >= max_score) {
      max_score = curr_score;
      max_begin = begin;
      max_end = it + 1;
    }

    last_begin = begin;
  }

  auto optimal = nums.subspan(max_begin - nums.begin(), max_end - max_begin);
  std::ranges::sort(optimal, {}, &Score::idx);

  std::println("{}", max_score);
  for (const auto &element : optimal) {
    std::print("{} ", element.idx + 1);
  }
}

auto run([[maybe_unused]] std::span<char *> args) -> void {
  std::size_t n_len = 0;

  std::cin >> n_len;

  std::vector<Score> nums(n_len);

  for (std::size_t i = 0; i < n_len; ++i) {
    std::cin >> nums[i].eff;
    nums[i].idx = i;
  }
  solve(nums);
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
