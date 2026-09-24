module;

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <optional>
#include <utility>

export module geometry:primitives;

import :matrix;

export namespace cpp_contests {
// NOLINTBEGIN(readability-identifier-length)

using point = vector<3, double>;

class Line final {
private:
  cpp_contests::point r0_, r_;

public:
  constexpr explicit Line(cpp_contests::point const &r0, cpp_contests::point const &r) noexcept
      : r0_(r0), r_(r / n(r)) {}

  [[nodiscard]] constexpr auto param(cpp_contests::point const &p) const noexcept -> double {
    auto diff = p - r0_;
    auto mi =
        std::ranges::max_element(r_, [](auto left, auto right) -> auto { return std::abs(left) < std::abs(right); }) -
        r_.begin();
    auto l = diff[mi] / r_[mi];
#ifndef NDEBUG
    for (std::size_t i = 0; i < 3; ++i) {
      if (std::cmp_equal(i, mi)) {
        continue;
      }
      if (std::abs(r_[i]) < kEpsMin) {
        continue;
      }
      auto ldebug = diff[i] / r_[i];
      assert(std::abs(l - ldebug) <= kEpsMax * std::abs(l));
    }
#endif
    return l;
  }
  [[nodiscard]] constexpr auto point(double param) const noexcept -> cpp_contests::point { return r0_ + param * r_; }

  [[nodiscard]] constexpr auto r0() const noexcept -> cpp_contests::point const & { return r0_; }
  [[nodiscard]] constexpr auto r() const noexcept -> cpp_contests::point const & { return r_; }
};

struct Segment {
  point s; // NOLINT(misc-non-private-member-variables-in-classes)
  point e; // NOLINT(misc-non-private-member-variables-in-classes)

  [[nodiscard]] constexpr auto line() const noexcept -> Line { return Line{s, e - s}; }
};

class Plane final {
private:
  point normal_;
  double d_;

  static constexpr auto norm_coeff(double a, double b, double c) noexcept {
    auto &&normal = point{a, b, c};
    return n(normal);
  }

public:
  constexpr explicit Plane(point const &normal) noexcept : normal_(normal / n(normal)), d_(n(normal)) {
    assert(n(normal) > kEpsMin);
  }
  constexpr explicit Plane(point const &normal, double d) noexcept : normal_(normal / n(normal)), d_(d / n(normal)) {
    assert(n(normal) > kEpsMin);
  }
  constexpr explicit Plane(point const &p1, point const &p2, point const &p3) noexcept
      : Plane(cross(p1 - p3, p2 - p3), dot(p3, cross(p1 - p3, p2 - p3))) {}
  constexpr explicit Plane(double a, double b, double c, double d) noexcept
      : normal_(point{a, b, c} / norm_coeff(a, b, c)), d_(-d / norm_coeff(a, b, c)) {}

  [[nodiscard]] constexpr auto normal() const noexcept -> point const & { return normal_; }
  [[nodiscard]] constexpr auto dist() const noexcept -> double { return d_; }
};

constexpr auto dist(point const &p1, point const &p2) noexcept -> double { return n(p1 - p2); }

constexpr auto complanar(Plane const &p1, Plane const &p2, double eps = kEpsMin) noexcept -> bool {
  auto diff = n(cross(p1.normal(), p2.normal()));
  // diff is implicitly divided by normal length squared to get relative
  // difference Since normal is a unit vector, division is omitted
  return diff < eps;
}
// Four points and a tolerance form the geometric predicate's natural interface.
// NOLINTNEXTLINE(readability-function-size)
constexpr auto complanar(point const &p1, point const &p2, point const &p3, point const &p4,
                         double eps = kEpsMin) noexcept -> bool {

  auto v1 = p1 - p3;
  auto v2 = p2 - p3;
  if (n(cross(v1, v2)) <= eps * n(v1) * n(v2)) {
    return true;
  }

  auto v3 = p1 - p4;
  auto v4 = p2 - p4;
  if (n(cross(v3, v4)) <= eps * n(v3) * n(v4)) {
    return true;
  }

  auto plane1 = Plane{p1, p2, p3};
  auto plane2 = Plane{p1, p2, p4};
  return complanar(plane1, plane2, eps);
}
constexpr auto complanar(Line const &l1, Line const &l2, double eps = kEpsMin) noexcept -> bool {
  return complanar(l1.r0(), l1.r0() + l1.r(), l2.r0(), l2.r0() + l2.r(), eps);
}

constexpr auto parallel(Line const &l1, Line const &l2, double eps = kEpsMin) noexcept -> bool {
  auto diff = n(cross(l1.r(), l2.r()));
  // diff is implicitly divided by normal length squared to get relative
  // difference Since normal is a unit vector, division is omitted
  return diff < eps;
}

constexpr auto complanar_intersection(Line const &p1, Line const &p2) noexcept -> std::optional<point> {
  assert(complanar(p1, p2, kEpsMax));
  if (parallel(p1, p2)) {
    return std::nullopt;
  }

  auto right = p2.r0() - p1.r0();
  auto left = Matrix<2, 3, double>{p1.r(), -p2.r()};

  auto first = iota<1>();
  auto first2 = iota<2>();
  auto rows = iota<2>();

  std::size_t i = 0;
  for (; i < 3; ++i) {
    auto leftcut = left[first2, rows];
    if (!invertible(leftcut)) {
      // A case where both line projections on the plane are the same line
      rows += 1;
      rows %= 3;
      continue;
    }
    auto rightcut = right[first, rows];
    auto answ = inv(leftcut) * rightcut;
    return (answ[0] * p1.r()) + p1.r0();
  }
  assert(false);
  return point{};
}

constexpr auto intersection(Plane const &p1, Plane const &p2) noexcept -> std::optional<Line> {
  if (complanar(p1, p2)) {
    return std::nullopt;
  }

  auto d1 = p1.dist();
  auto d2 = p2.dist();
  auto nn = dot(p1.normal(), p2.normal());
  auto c1 = (d1 - (d2 * nn)) / (1 - (nn * nn));
  auto c2 = (d2 - (d1 * nn)) / (1 - (nn * nn));
  auto r0 = (c1 * p1.normal()) + (c2 * p2.normal());
  auto r = cross(p1.normal(), p2.normal());
  return Line{r0, r};
}

constexpr auto intersection(Plane const &p1, Plane const &p2, Plane const &p3) noexcept -> std::optional<point> {
  if (complanar(point{}, p1.normal(), p2.normal(), p3.normal())) {
    return std::nullopt;
  }

  Matrix<3, 3, double> const m{t(p1.normal()), t(p2.normal()), t(p3.normal())};
  return inv(m) * point{p1.dist(), p2.dist(), p3.dist()};
}

constexpr auto complanar_intersection(Segment const &s, Line const &l) noexcept -> std::optional<point> {
  auto seg = s.line();
  assert(complanar(seg, l, kEpsMax));
  auto intersection = complanar_intersection(seg, l);
  if (!intersection) {
    // segment and line are parallel
    return std::nullopt;
  }

  auto param = seg.param(*intersection);
  if (0.0 <= param && param <= n(s.e - s.s)) {
    return intersection;
  }
  return std::nullopt;
}

constexpr auto complanar_intersection(Line const &l, Segment const &s) noexcept -> std::optional<point> {
  return complanar_intersection(s, l);
}

constexpr auto complanar_intersection(Segment const &s1, Segment const &s2) noexcept -> std::optional<point> {
  auto l1 = s1.line();
  auto l2 = s2.line();
  auto i1 = complanar_intersection(s1, l2);
  if (!i1) {
    return std::nullopt;
  }
  auto i2 = complanar_intersection(s2, l1);
  if (!i2) {
    return std::nullopt;
  }

  assert(n(*i1 - *i2) <= kEpsMax * n(*i1));
  return i1;
}

// NOLINTEND(readability-identifier-length)
} // namespace cpp_contests
