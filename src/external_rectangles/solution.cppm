module;

#include <algorithm>
#include <bit>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <iostream>
#include <optional>
#include <print>
#include <span>
#include <vector>

export module external_rectangles;

namespace cpp_contests {
namespace {

// NOLINTBEGIN(misc-non-private-member-variables-in-classes)
struct Rect {
  int32_t x1;
  int32_t x2;
  int32_t y1;
  int32_t y2;

  auto normalize() noexcept -> void {
    if (x1 > x2) {
      std::swap(x1, x2);
    }
    if (y1 > y2) {
      std::swap(y1, y2);
    }
  }
};
// NOLINTEND(misc-non-private-member-variables-in-classes)

struct LowerCmp {
  auto operator()(Rect const &lhs, Rect const &rhs) const -> bool { return lhs.y1 < rhs.y1; }
};

struct Boundary {
  Rect rect;
  std::size_t y_idx;
  int32_t boundary;
  bool opening;
};

// std::set<Rect> is much more straightforward, but it is banned for this task.
// NOLINTBEGIN(misc-non-private-member-variables-in-classes)
struct SegmentTree {
  std::vector<bool> tree;
  std::size_t size;

  auto set_active(std::size_t idx, bool active = true) -> void {
    assert(idx < size);
    auto node = (tree.size() / 2) + idx;
    tree[node] = active;
    while (node > 1) {
      node /= 2;
      tree[node] = tree[2 * node] || tree[(2 * node) + 1];
    }
  }

  [[nodiscard, maybe_unused]] auto is_active(std::size_t idx) const -> bool {
    assert(idx < size);
    return tree[(tree.size() / 2) + idx];
  }

  [[nodiscard]] auto last_active(std::size_t end) const -> std::optional<std::size_t> {
    // This is magical method I do not understand, sorry.
    assert(end <= size);
    if (end == 0) {
      return std::nullopt;
    }

    auto capacity = tree.size() / 2;
    auto node = capacity + end - 1;

    while (!tree[node]) {
      while (node > 1 && node % 2 == 0) {
        node /= 2;
      }

      if (node == 1) {
        return std::nullopt;
      }

      --node;
    }

    while (node < capacity) {
      auto right = (2 * node) + 1;
      node = tree[right] ? right : right - 1;
    }

    return node - capacity;
  }
};
// NOLINTEND(misc-non-private-member-variables-in-classes)

auto solve(std::vector<Rect> const &rects) -> std::size_t {
  auto verts = rects;
  std::ranges::sort(verts, LowerCmp{});

  auto pow2 = std::bit_ceil(verts.size());
  auto tree = SegmentTree{.tree = std::vector<bool>(pow2 * 2, false), .size = verts.size()};

  auto boundaries = std::vector<Boundary>();
  boundaries.reserve(verts.size() * 2);
  for (std::size_t idx = 0; idx < verts.size(); ++idx) {
    boundaries.push_back({.rect = verts[idx], .y_idx = idx, .boundary = verts[idx].x1, .opening = true});
    boundaries.push_back({.rect = verts[idx], .y_idx = idx, .boundary = verts[idx].x2, .opening = false});
  }
  std::ranges::sort(boundaries, {}, [](auto const &boundary) -> auto { return boundary.boundary; });

  std::size_t outers = 0;
  for (auto &&boundary : boundaries) {
    if (!boundary.opening) {
      // Remove active flag from segment tree if any.
      tree.set_active(boundary.y_idx, false);
      continue;
    }
    auto max_candidate_it = std::ranges::lower_bound(verts, boundary.rect, LowerCmp{});
    auto last_active_idx = tree.last_active(max_candidate_it - verts.begin());

    if (!last_active_idx.has_value() || verts[last_active_idx.value()].y2 < boundary.rect.y2) {
      ++outers;
      tree.set_active(boundary.y_idx, true);
    }
  }

  return outers;
}

auto run([[maybe_unused]] std::span<char *> args) -> void {
  std::size_t n_len = 0;

  std::cin >> n_len;
  auto arr = std::vector<Rect>(n_len);

  for (std::size_t idx = 0; idx < n_len; ++idx) {
    std::cin >> arr[idx].x1;
    std::cin >> arr[idx].y1;
    std::cin >> arr[idx].x2;
    std::cin >> arr[idx].y2;
    arr[idx].normalize();
  }

  std::println("{}", solve(arr));
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
