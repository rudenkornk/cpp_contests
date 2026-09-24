module;
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <iostream>
#include <limits>
#include <span>
#include <utility>
#include <vector>

export module longest_alternating_seq;

namespace {

constexpr auto kMax = std::numeric_limits<std::size_t>::max();

struct Info {
  std::size_t len;
  std::size_t prev;
};

auto print(std::span<std::int32_t> nums, std::span<Info> ups, std::span<Info> downs) -> void {
  if (downs[0].len > ups[0].len || (downs[0].len == ups[0].len && downs[0].prev < ups[0].prev)) {
    std::swap(ups, downs);
  }

  std::size_t idx = 0;
  while (idx != kMax) {
    std::cout << nums[idx] << " ";
    idx = ups[idx].prev;
    std::swap(ups, downs);
  };
}

// NOLINTNEXTLINE(readability-function-size)
auto run([[maybe_unused]] std::span<char *> args) -> void {
  std::size_t len = 0;
  std::cin >> len;
  std::vector<std::int32_t> nums(len);
  for (std::size_t idx = 0; idx < len; ++idx) {
    std::cin >> nums[idx];
  }

  std::vector<Info> ups(len);
  std::vector<Info> downs(len);

  for (std::size_t idx = len - 1; idx != kMax; --idx) {
    auto el = nums[idx];
    auto best_up = Info{.len = 1, .prev = kMax};
    auto best_down = Info{.len = 1, .prev = kMax};
    for (std::size_t sub_idx = idx + 1; sub_idx < len; ++sub_idx) {
      auto sub = nums[sub_idx];
      auto &&up = ups[sub_idx];
      auto &&down = downs[sub_idx];
      if (el > sub) {
        if (best_down.len < up.len + 1) {
          best_down.len = up.len + 1;
          best_down.prev = sub_idx;
        }
      } else if (el < sub) {
        if (best_up.len < down.len + 1) {
          best_up.len = down.len + 1;
          best_up.prev = sub_idx;
        }
      }
    }

    ups[idx] = best_up;
    downs[idx] = best_down;
  }
  print(std::span(nums), std::span(ups), std::span(downs));
}
} // namespace

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
extern "C++" auto main(int argc, char **argv) -> int {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  try {
    run(std::span(argv, static_cast<std::size_t>(argc)));
  } catch (std::exception const &e) {
    std::cerr << e.what() << "\n";
    return 1;
  }
}
#pragma GCC diagnostic pop
