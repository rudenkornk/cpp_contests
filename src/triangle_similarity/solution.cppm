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

export module triangle_similarity;

namespace cpp_contests {
namespace {

using side = uint32_t;

auto sort(side side_x, side side_y, side side_z) -> std::tuple<side, side, side> {
  if (side_x > side_y) {
    std::swap(side_x, side_y);
  }
  if (side_y > side_z) {
    std::swap(side_y, side_z);
  }
  if (side_x > side_y) {
    std::swap(side_x, side_y);
  }
  return {side_x, side_y, side_z};
}

// NOLINTBEGIN(misc-non-private-member-variables-in-classes)
struct Triangle {
  side x = 1;
  side y = 1;
  side z = 1;

  Triangle(side side_x, side side_y, side side_z) {
    std::tie(x, y, z) = sort(side_x, side_y, side_z);
    auto gcd = std::gcd(std::gcd(x, y), z);
    x /= gcd;
    y /= gcd;
    z /= gcd;
  }

  auto operator==(Triangle const &other) const -> bool = default;
};
// NOLINTEND(misc-non-private-member-variables-in-classes)

struct TriangleHash {
  auto operator()(Triangle const &triangle) const -> std::size_t {
    auto constexpr prime = 0x9e3779b9;
    auto constexpr shift = 6U;
    auto seed = std::hash<side>{}(triangle.x);
    for (auto value : {triangle.y, triangle.z}) {
      seed ^= std::hash<side>{}(value) + std::size_t{prime} + (seed << shift) + (seed >> 2U);
    }
    return seed;
  }
};

auto solve(std::vector<Triangle> const &triangles) -> void {
  auto set_capacity = 4 * triangles.size();
  auto set = std::vector<std::vector<Triangle>>(set_capacity);
  auto set_size = std::size_t{0};
  auto hash = TriangleHash{};
  for (auto const &triangle : triangles) {
    auto idx = hash(triangle) % set_capacity;
    auto &bucket = set[idx];
    if (std::ranges::find(bucket, triangle) == bucket.end()) {
      bucket.push_back(triangle);
      ++set_size;
    }
  }
  std::println("{}", set_size);
}

auto run([[maybe_unused]] std::span<char *> args) -> void {
  std::size_t n_len = 0;

  std::cin >> n_len;
  auto triangles = std::vector<Triangle>();
  triangles.reserve(n_len);

  for (std::size_t idx = 0; idx < n_len; ++idx) {
    auto x = side{};
    auto y = side{};
    auto z = side{};
    std::cin >> x >> y >> z;
    triangles.emplace_back(x, y, z);
  }

  solve(triangles);
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
