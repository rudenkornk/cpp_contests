module;

// `<cassert>` stays because `import std;` does not export the `assert` macro.
#include <cassert>

export module geometry:triangle;

import std;

import :matrix;
import :primitives;

export namespace cpp_contests {
// NOLINTBEGIN(readability-identifier-length)

class Triangle final {
private:
  Plane plane_;
  std::array<Point, 3> points_;

public:
  constexpr Triangle(Point p1, Point p2, Point p3) noexcept : plane_(p1, p2, p3), points_({p1, p2, p3}) {
#ifndef NDEBUG
    auto d1 = dist(p1, p2);
    auto d2 = dist(p2, p3);
    auto d3 = dist(p3, p1);
    assert(d1 + d2 > d3 && d2 + d3 > d1 && d3 + d1 > d2);
#endif
  }

  [[nodiscard]] constexpr auto operator[](std::size_t i) const noexcept -> Point const & {
    assert(i < 3);
    // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-constant-array-index)
    return points_[i];
  }
  [[nodiscard]] constexpr auto plane() const noexcept -> Plane const & { return plane_; }
};

constexpr auto complanar_intersects(Triangle const &t1, Triangle const &t2) noexcept -> bool {
  if (std::abs(t1.plane().dist() - t2.plane().dist()) > EPSMIN) {
    return false;
  }
  for (std::size_t i = 0; i < 3; ++i) {
    auto seg1 = Segment(t1[i % 3], t1[(i + 1) % 3]);
    for (std::size_t j = 0; j < 3; ++j) {
      auto seg2 = Segment(t2[j % 3], t2[(j + 1) % 3]);
      if (complanar_intersection(seg1, seg2)) {
        return true;
      }
    }
  }
  return false;
}

constexpr auto complanar_intersection(Triangle const &t, Line const &l) -> std::optional<Segment> {
  Vector<3, Point> result{};
  std::size_t j = 0;
  for (std::size_t i = 0; i < 3; ++i) {
    auto side = Segment(t[i % 3], t[(i + 1) % 3]);
    auto intersection = complanar_intersection(side, l);
    if (!intersection) {
      continue;
    }
    result[j] = intersection.value();
    ++j;
  }
  assert(j == 0 || j == 2 || j == 3);
  if (j == 0) {
    return std::nullopt;
  }
  if (j == 2) {
    return Segment(result[0], result[1]);
  }

  // A corner case where line intersects triangle through one of the vertices
  // and the opposite side
  // We need to pick two most distant points
  // clang-format off
  auto diffs = std::array<double, 3>{
      n(result[1] - result[0]),
      n(result[2] - result[1]),
      n(result[0] - result[2])
  };
  // clang-format on
  auto n = std::ranges::max_element(diffs) - diffs.begin();
  auto inds = (iota<2>() + n) % 3;
  return Segment{.s = result[inds[0]], .e = result[inds[1]]};
}

constexpr auto complanar_intersection(Line const &l, Triangle const &t) -> std::optional<Segment> {
  return complanar_intersection(t, l);
}

constexpr auto intersection(Triangle const &t1, Triangle const &t2) noexcept -> std::optional<Segment> {
  auto intersection_opt = intersection(t1.plane(), t2.plane());
  if (!intersection_opt) {
    // Triangles are complanar
    return std::nullopt;
  }

  auto intersection = intersection_opt.value();
  auto i1 = complanar_intersection(t1, intersection);
  if (!i1) {
    return std::nullopt;
  }
  auto i2 = complanar_intersection(t2, intersection);
  if (!i2) {
    return std::nullopt;
  }

  using std::swap;
  auto s1 = intersection.param(i1.value().s);
  auto e1 = intersection.param(i1.value().e);
  auto s2 = intersection.param(i2.value().s);
  auto e2 = intersection.param(i2.value().e);
  if (s1 > e1) {
    swap(s1, e1);
  }
  if (s2 > e2) {
    swap(s2, e2);
  }
  if (s1 > s2) {
    swap(s1, s2);
    swap(e1, e2);
  }
  if (s2 <= e1) {
    return Segment{.s = intersection.point(s2), .e = intersection.point(e1)};
  }

  return std::nullopt;
}

constexpr auto intersects(Triangle const &t1, Triangle const &t2) noexcept -> bool {
  if (complanar(t1.plane(), t2.plane())) {
    return complanar_intersects(t1, t2);
  }
  return intersection(t1, t2).has_value();
}

// NOLINTEND(readability-identifier-length)
} // namespace cpp_contests
