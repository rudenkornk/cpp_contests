module;
#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <iostream>
#include <print>
#include <ranges>
#include <set>
#include <utility>
#include <vector>

export module artifacts;

namespace {
// Keep the sliding-window state and its advance operation in one place.
// NOLINTNEXTLINE(readability-function-size)
auto solution() -> void {
  std::size_t n_epochs = 0;
  std::size_t m_arts = 0;

  std::cin >> n_epochs >> m_arts;

  std::vector<std::uint64_t> arts(n_epochs * m_arts);
  for (std::size_t i = 0; i < n_epochs * m_arts; ++i) {
    std::cin >> arts[i];
  }

  // a[EPOCH * num_arts + i_art]

  std::vector<std::pair<std::uint64_t, std::size_t>> tagged(m_arts * n_epochs);
  auto indexed = std::ranges::views::zip(arts, std::views::iota(std::size_t{0}));

  std::ranges::transform(indexed, tagged.begin(), [m_arts](auto &&pair) -> std::pair<std::uint64_t, std::size_t> {
    auto [art, idx] = pair;
    return {art, idx / m_arts};
  });

  std::ranges::sort(tagged);

  std::vector<std::size_t> epoch_counters(n_epochs);

  // ------------  Initialization block. -------------------
  std::set<std::size_t> seen_epochs;

  std::size_t begin_ptr = 0;
  std::size_t end_ptr = -1;
  while (seen_epochs.size() < n_epochs) {
    ++end_ptr;
    const auto &[art, epoch] = tagged[end_ptr];
    ++epoch_counters[epoch];
    seen_epochs.insert(epoch);
  }

  std::size_t optimal_begin = begin_ptr;
  std::size_t optimal_end = end_ptr;
  std::uint64_t optimal_score = -1;
  std::size_t last_being_art = -1;
  // ----------- Compute block. -------------------

  auto advance = [&]() -> bool {
    while (last_being_art == tagged[begin_ptr].first) {
      const auto &[_art, epoch] = tagged[begin_ptr];
      if (epoch_counters[epoch] > 0) {
        --epoch_counters[epoch];
        if (epoch_counters[epoch] == 0) {
          seen_epochs.erase(epoch);
        }
      }
      ++begin_ptr;
      if (begin_ptr == tagged.size()) {
        return false;
      }
    }
    last_being_art = tagged[begin_ptr].first;

    end_ptr = std::max(begin_ptr, end_ptr);

    while (seen_epochs.size() < n_epochs) {
      ++end_ptr;
      if (end_ptr == tagged.size()) {
        return false;
      }

      const auto &[art, epoch] = tagged[end_ptr];
      ++epoch_counters[epoch];
      seen_epochs.insert(epoch);
    }
    return true;
  };

  while (advance()) {
    const std::uint64_t score = tagged[end_ptr].first - tagged[begin_ptr].first;
    if (score < optimal_score) {
      optimal_score = score;
      optimal_begin = begin_ptr;
      optimal_end = end_ptr;
    }
  }

  seen_epochs.clear();
  for (std::size_t i = optimal_begin; i <= optimal_end; ++i) {
    const auto &[art, epoch] = tagged[i];

    if (seen_epochs.contains(epoch)) {
      continue;
    }
    std::print("{} ", art);
    seen_epochs.insert(epoch);
  }
}
} // namespace

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
extern "C++" auto main() -> int {
  try {
    solution();
  } catch (std::exception const &e) {
    std::cerr << e.what() << "\n";
    return 1;
  }
  return 0;
}
#pragma GCC diagnostic pop
