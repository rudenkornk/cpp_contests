module;

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <iostream>
#include <limits>
#include <print>
#include <span>
#include <vector>

export module ali_baba;

namespace cpp_contests {
namespace {

struct Coin {
  int32_t x;
  int32_t time;
};

struct Time {
  int32_t l;
  int32_t r;
};

auto constexpr kMax = std::numeric_limits<int32_t>::max();
auto constexpr kDebug = false;

// NOLINTBEGIN(readability-function-size,
// readability-function-cognitive-complexity)
auto solve(std::span<Coin> coins) -> int32_t {
  // NOLINTEND(readability-function-size,
  // readability-function-cognitive-complexity)
  auto len = coins.size();

  std::ranges::sort(coins, {}, &Coin::x);
  auto lr_table = std::vector<Time>((len * (len - 1)) / 2);
  auto zero_time = Time{.l = 0, .r = 0};

  auto at = [&lr_table, &zero_time](std::size_t left_idx, std::size_t right_idx) -> Time & {
    if (left_idx == right_idx) {
      return zero_time;
    }
    return lr_table[((right_idx * (right_idx - 1)) / 2) + left_idx];
  };

  for (std::size_t ir = 1; ir < len; ++ir) {
    for (std::size_t ilpp = ir; ilpp != 0; --ilpp) {
      auto il = ilpp - 1;
      auto rdist = coins[ir].x - coins[ir - 1].x;
      auto rtime = at(il, ir - 1);

      auto rrtime = kMax;
      auto rltime = kMax;
      if (rtime.r != kMax) {
        rrtime = rtime.r + rdist;
      }
      if (rtime.l != kMax) {
        auto extra_dist = coins[ir - 1].x - coins[il].x;
        rltime = rtime.l + rdist + extra_dist;
      }
      auto rres = std::min(rrtime, rltime);
      if (rres > coins[ir].time) {
        rres = kMax;
      }

      auto ldist = coins[il + 1].x - coins[il].x;
      auto ltime = at(il + 1, ir);

      auto lrtime = kMax;
      auto lltime = kMax;
      if (ltime.l != kMax) {
        lltime = ltime.l + ldist;
      }
      if (ltime.r != kMax) {
        auto extra_dist = coins[ir].x - coins[il + 1].x;
        lrtime = ltime.r + ldist + extra_dist;
      }
      auto lres = std::min(lrtime, lltime);
      if (lres > coins[il].time) {
        lres = kMax;
      }

      at(il, ir).l = lres;
      at(il, ir).r = rres;
    }
  }

  if constexpr (kDebug) {
    auto constexpr k_width = 18;
    for (std::size_t ir = 0; ir < len; ++ir) {
      std::print("R = {}  ", ir);
      for (std::size_t il = 0; il <= ir; ++il) {
        auto time = at(il, ir);
        auto cell = "(" + std::to_string(time.l) + ", " + std::to_string(time.r) + ")";
        std::print("{:>{}}", cell, k_width);
      }
      std::println();
    }
    std::print("L = ");
    for (std::size_t il = 0; il < len; ++il) {
      std::print("{:>{}}", il, k_width);
    }
    std::println();
  }

  auto res = at(0, len - 1);
  return std::min(res.l, res.r);
}

auto run([[maybe_unused]] std::span<char *> args) -> void {
  std::size_t n_len = 0;

  std::cin >> n_len;

  std::vector<Coin> coins(n_len);

  for (std::size_t i = 0; i < n_len; ++i) {
    std::cin >> coins[i].x >> coins[i].time;
  }
  auto res = solve(coins);
  if (res == kMax) {
    std::println("No solution");
  } else {
    std::println("{}", res);
  }
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
