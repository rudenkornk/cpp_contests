module;
#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <format>
#include <iostream>
#include <span>
#include <vector>

export module minimal_covering_circle;

namespace {

auto constexpr kEps = 0.00001;

struct Point {
  int16_t x;
  int16_t y;
};

struct Bracket {
  double pos;
  bool starting;
};

auto max_r(std::span<Point const> coords) -> double {
  auto rads = std::vector<double>(coords.size());
  std::ranges::transform(coords, rads.begin(), [](Point const &point) -> double {
    return static_cast<double>((point.x * point.x) + (point.y * point.y));
  });

  return std::sqrt(std::ranges::max(rads));
}

auto get_brackets(std::span<Point const> coords, double rad) -> std::vector<Bracket> {
  auto brackets = std::vector<Bracket>();
  for (auto point : coords) {
    if (point.y > rad) {
      continue;
    }

    double x_delta = std::sqrt((rad * rad) - (point.y * point.y));
    brackets.push_back({.pos = static_cast<double>(point.x) - x_delta, .starting = true});
    brackets.push_back({.pos = static_cast<double>(point.x) + x_delta, .starting = false});
  }
  return brackets;
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
auto fit(std::span<Point const> coords, std::size_t k_len, double rad) -> bool {
  auto brackets = get_brackets(coords, rad);
  std::ranges::sort(brackets, [](Bracket left, Bracket right) -> bool {
    return (left.pos < right.pos) || (left.pos == right.pos && left.starting && !right.starting);
  });
  std::size_t current = 0;
  for (auto bracket : brackets) {
    current += (bracket.starting ? 1 : -1);
    if (current >= k_len) {
      return true;
    }
  }

  return false;
}

auto solve(std::span<Point const> coords, std::size_t k_len) -> double {
  auto left = 0.0;
  auto right = max_r(coords);
  auto constexpr k_max_iter = 100;
  auto iter = 0;

  while ((right - left > kEps) && iter < k_max_iter) {
    auto mid = (left + right) / 2;
    if (fit(coords, k_len, mid)) {
      right = mid;
    } else {
      left = mid;
    }
    ++iter;
  }

  return right;
}

auto run([[maybe_unused]] std::span<char *> args) -> void {
  std::size_t n_len = 0;
  std::size_t k_len = 0;

  std::cin >> n_len >> k_len;

  std::vector<Point> coords(n_len);

  for (size_t i = 0; i < n_len; ++i) {
    std::cin >> coords[i].x >> coords[i].y;
    coords[i].y = static_cast<int16_t>(std::abs(coords[i].y));
  }

  auto result = solve(coords, k_len);
  std::cout << std::format("{:.6f}", result) << "\n";
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
