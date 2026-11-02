module;

#include <cassert>
#include <cstddef>
#include <exception>
#include <iostream>
#include <print>
#include <span>
#include <vector>

export module treap_height;

namespace cpp_contests {
namespace {

// NOLINTNEXTLINE(readability-function-size)
auto solve(std::size_t n_len, std::size_t height) -> double {
  if (height >= n_len) {
    return 0.0;
  }
  if (height == 0) {
    return static_cast<double>(n_len <= 1);
  }

  auto zero = 0.0;
  auto one = 1.0;

  auto ps_size = ((n_len - 1) * height) - ((height * (height + 1)) / 2);
  auto ps_raw = std::vector<double>(ps_size);
  auto ps = [&](std::size_t node_count, std::size_t height_count) -> double & {
    if (node_count <= 1 || height_count + 1 >= node_count) {
      return one;
    }
    if (height_count == 0) {
      return zero;
    }

    auto preceding_rows = height_count - 1;
    auto offset = (preceding_rows * (n_len - 2)) - ((preceding_rows * (preceding_rows - 1)) / 2);
    return ps_raw[offset + node_count - height_count - 2];
  };

  for (std::size_t nc = 3; nc <= n_len; ++nc) {
    for (std::size_t hc = 1; hc <= height && hc + 1 < nc; ++hc) {
      auto sum = 0.0;
      for (std::size_t it = 0; it < nc; ++it) {
        sum += ps(nc - 1 - it, hc - 1) * ps(it, hc - 1);
      }
      ps(nc, hc) = sum / static_cast<double>(nc);
    }
  }

  return ps(n_len, height) - ps(n_len, height - 1);
}

auto run([[maybe_unused]] std::span<char *> args) -> void {
  std::size_t height = 0;
  std::size_t n_len = 0;

  std::cin >> n_len >> height;
  auto constexpr precision = 10;
  std::println("{:.{}f}", solve(n_len, height), precision);
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
