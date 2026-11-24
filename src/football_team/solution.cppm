module;

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <iostream>
#include <iterator>
#include <numeric>
#include <random>
#include <span>
#include <type_traits>
#include <utility>
#include <vector>

export module football_team;

namespace cpp_contests {
namespace {

struct Player {
  int32_t eff;
  std::size_t idx;
};

struct ByEfficiency {
  auto operator()(const Player &lhs, const Player &rhs) const noexcept -> bool { return lhs.eff < rhs.eff; }
};

struct ByOriginalIndex {
  auto operator()(const Player &lhs, const Player &rhs) const noexcept -> bool { return lhs.idx < rhs.idx; }
};

template <typename RandomIt, typename Compare>
auto insertion_sort(RandomIt first, RandomIt last, const Compare &compare) -> void {
  for (auto it = first; it != last; ++it) {
    const std::iter_value_t<RandomIt> element = *it;
    auto position = it;

    while (position != first && compare(element, *(position - 1))) {
      *position = *(position - 1);
      --position;
    }

    *position = element;
  }
}

template <typename RandomIt, std::uniform_random_bit_generator Rng>
auto select_pivot(RandomIt first, RandomIt last, Rng &rng) -> std::iter_value_t<RandomIt> {
  using difference = std::iter_difference_t<RandomIt>;
  auto offset = std::uniform_int_distribution<difference>(0, (last - first) - 1);
  return *(first + offset(rng));
}

/// Partition the range [first, last) into two parts:
/// [first, split) contains elements less than pivot_value.
/// [split, last) contains elements greater than or equal to pivot_value.
/// Caller must guarantee that pivot_value is in the range [first, last).
template <typename RandomIt, typename Compare>
auto partition(RandomIt first, RandomIt last, const std::iter_value_t<RandomIt> &pivot_value, const Compare &compare)
    -> std::pair<RandomIt, RandomIt> {
  // first      eq_first      cur         eq_last      last
  // |            |          |             |           |
  // | < pivot    | = pivot  | unexamined  | > pivot   |

  auto eq_first = first;
  auto cur = first;
  auto eq_last = last;

  while (cur != eq_last) {
    if (compare(*cur, pivot_value)) {
      std::ranges::iter_swap(eq_first, cur);
      ++eq_first;
      ++cur;
    } else if (compare(pivot_value, *cur)) {
      --eq_last;
      std::ranges::iter_swap(cur, eq_last);
    } else {
      ++cur;
    }
  }

  return {eq_first, eq_last};
}

template <typename RandomIt, typename Compare, typename Rng = std::mt19937>
  requires std::uniform_random_bit_generator<std::remove_reference_t<Rng>>
// NOLINTNEXTLINE(misc-no-recursion, bugprone-random-generator-seed, cert-msc32-c, cert-msc51-cpp)
auto quick_sort(RandomIt first, RandomIt last, Compare compare, Rng &&rng = Rng{}) -> void {
  auto constexpr k_insertion_sort_limit = 64;
  while (first != last && last - first > 1) {
    if (last - first <= k_insertion_sort_limit) {
      insertion_sort(first, last, compare);
      return;
    }

    auto pivot_value = select_pivot(first, last, rng);
    auto [eq_first, eq_last] = partition(first, last, pivot_value, compare);
    if (eq_first - first <= last - eq_last) {
      quick_sort(first, eq_first, compare, std::forward<Rng>(rng));
      first = eq_last;
    } else {
      quick_sort(eq_last, last, compare, std::forward<Rng>(rng));
      last = eq_first;
    }
  }
}

/// Using exponential search find the first index i such that
/// target <= nums[i] + nums[i + 1];
auto exp_search(std::span<Player> nums, int32_t target) -> std::span<Player>::iterator {
  if (nums.size() <= 2) {
    return nums.begin();
  }

  std::ptrdiff_t diff = 1;
  auto search_start = nums.begin();
  auto search_end = search_start + 1;
  int32_t const half_tgt = (target / 2) + (target % 2);

  while (search_end->eff < half_tgt) {
    search_start = search_end;
    search_end += std::min(diff, (nums.end() - 1) - search_end);
    diff *= 2;
  }

  auto lower = std::ranges::lower_bound(search_start, search_end + 1, half_tgt, {}, &Player::eff);

  // Check edge case where element < target / 2, but still qualifies.
  if (lower > nums.begin() && ((lower - 1)->eff >= (target - lower->eff))) {
    return lower - 1;
  }

  return lower;
}

auto best_football_team(std::vector<Player> players) -> std::vector<Player> {
  auto players_span = std::span{players};

  quick_sort(players_span.begin(), players_span.end(), ByEfficiency{});
  auto last_begin = players_span.begin();
  auto max_begin = players_span.begin();
  auto max_end = players_span.end();
  std::uint64_t max_score = 0;
  std::uint64_t curr_score = 0;

  for (auto it = players_span.begin(); it < players_span.end(); ++it) {
    auto begin = exp_search(std::span{last_begin, it + 1}, it->eff);
    curr_score += static_cast<std::uint64_t>(it->eff);
    curr_score =
        std::accumulate(last_begin, begin, curr_score, [](std::uint64_t score, const Player &player) -> std::uint64_t {
          return score - static_cast<std::uint64_t>(player.eff);
        });
    if (curr_score >= max_score) {
      max_score = curr_score;
      max_begin = begin;
      max_end = it + 1;
    }

    last_begin = begin;
  }

  return {max_begin, max_end};
}

auto sort_by_original_index(std::vector<Player> &team) -> void {
  quick_sort(team.begin(), team.end(), ByOriginalIndex{});
}

auto sum_efficiency(std::span<const Player> team) -> std::uint64_t {
  std::uint64_t score = 0;
  for (const auto &player : team) {
    score += static_cast<std::uint64_t>(player.eff);
  }
  return score;
}

auto write_team(std::ostream &out, std::span<const Player> team) -> void {
  out << sum_efficiency(team) << '\n';
  for (const auto &player : team) {
    out << player.idx + 1 << ' ';
  }
  out << '\n';
}

auto read_players(std::istream &input) -> std::vector<Player> {
  std::size_t n_len = 0;

  input >> n_len;

  std::vector<Player> players(n_len);

  for (std::size_t i = 0; i < n_len; ++i) {
    input >> players[i].eff;
    players[i].idx = i;
  }
  return players;
}

auto run([[maybe_unused]] std::span<char *> args) -> void {
  auto team = best_football_team(read_players(std::cin));
  sort_by_original_index(team);
  write_team(std::cout, team);
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
