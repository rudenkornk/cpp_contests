#pragma once

#include <algorithm>
#include <cassert>
#include <optional>

#include "geometry/matrix.hpp"

namespace cpp_contests {

using Point = Vector<3, double>;

class Line final {
private:
  Point R0_, R_;

public:
  constexpr explicit Line(Point const &R0, Point const &R) noexcept
      : R0_(R0), R_(R / n(R)) {}

  [[nodiscard]] constexpr double param(Point const &p) const noexcept {
    auto diff = p - R0_;
    auto mi = std::max_element(R_.begin(), R_.end(),
                               [](auto left, auto right) {
                                 return std::abs(left) < std::abs(right);
                               }) -
              R_.begin();
    auto l = diff[mi] / R_[mi];
#ifndef NDEBUG
    for (std::size_t i = 0; i < 3; ++i) {
      if (i == static_cast<std::size_t>(mi))
        continue;
      if (std::abs(R_[i]) < EPSMIN)
        continue;
      auto ldebug = diff[i] / R_[i];
      assert(std::abs(l - ldebug) <= EPSMAX * std::abs(l));
    }
#endif
    return l;
  }
  [[nodiscard]] constexpr Point point(double param) const noexcept {
    return R0_ + param * R_;
  }

  [[nodiscard]] constexpr Point const &R0() const noexcept { return R0_; }
  [[nodiscard]] constexpr Point const &R() const noexcept { return R_; }
};

struct Segment {
  Point s;
  Point e;

  [[nodiscard]] constexpr Line line() const noexcept { return Line{s, e - s}; }
};

class Plane final {
private:
  Point normal_;
  double d_;

  static constexpr auto norm_coeff_(double A, double B, double C) noexcept {
    auto &&normal = Point{A, B, C};
    return n(normal);
  }

public:
  constexpr explicit Plane(Point const &normal) noexcept
      : normal_(normal / n(normal)), d_(n(normal)) {
    assert(n(normal) > EPSMIN);
  }
  // NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
  constexpr explicit Plane(Point const &normal, double d) noexcept
      : normal_(normal / n(normal)), d_(d / n(normal)) {
    assert(n(normal) > EPSMIN);
  }
  constexpr explicit Plane(Point const &p1, Point const &p2,
                           Point const &p3) noexcept
      : Plane(cross(p1 - p3, p2 - p3), dot(p3, cross(p1 - p3, p2 - p3))) {}
  constexpr explicit Plane(double A, double B, double C, double D) noexcept
      : normal_(Point{A, B, C} / norm_coeff_(A, B, C)),
        d_(-D / norm_coeff_(A, B, C)) {}

  [[nodiscard]] constexpr Point const &normal() const noexcept {
    return normal_;
  }
  [[nodiscard]] constexpr double dist() const noexcept { return d_; }
};

constexpr double dist(Point const &p1, Point const &p2) noexcept {
  return n(p1 - p2);
}

constexpr bool complanar(Plane const &p1, Plane const &p2,
                         double eps = EPSMIN) noexcept {
  auto diff = n(cross(p1.normal(), p2.normal()));
  // diff is implicitly divided by normal length squared to get relative
  // difference Since normal is a unit vector, division is omitted
  return diff < eps;
}
constexpr bool complanar(Point const &p1, Point const &p2, Point const &p3,
                         Point const &p4, double eps = EPSMIN) noexcept {

  auto v1 = p1 - p3;
  auto v2 = p2 - p3;
  if (n(cross(v1, v2)) <= eps * n(v1) * n(v2))
    return true;

  auto v3 = p1 - p4;
  auto v4 = p2 - p4;
  if (n(cross(v3, v4)) <= eps * n(v3) * n(v4))
    return true;

  auto plane1 = Plane{p1, p2, p3};
  auto plane2 = Plane{p1, p2, p4};
  return complanar(plane1, plane2, eps);
}
constexpr bool complanar(Line const &l1, Line const &l2,
                         double eps = EPSMIN) noexcept {
  return complanar(l1.R0(), l1.R0() + l1.R(), l2.R0(), l2.R0() + l2.R(), eps);
}

constexpr bool parallel(Line const &l1, Line const &l2,
                        double eps = EPSMIN) noexcept {
  auto diff = n(cross(l1.R(), l2.R()));
  // diff is implicitly divided by normal length squared to get relative
  // difference Since normal is a unit vector, division is omitted
  return diff < eps;
}

constexpr std::optional<Point> complanar_intersection(Line const &p1,
                                                      Line const &p2) noexcept {
  assert(complanar(p1, p2, EPSMAX));
  if (parallel(p1, p2))
    return std::nullopt;

  auto right = p2.R0() - p1.R0();
  auto left = Matrix<2, 3, double>{p1.R(), -p2.R()};

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
    return answ[0] * p1.R() + p1.R0();
  }
  assert(false);
  return Point{};
}

constexpr std::optional<Line> intersection(Plane const &p1,
                                           Plane const &p2) noexcept {
  if (complanar(p1, p2))
    return std::nullopt;

  auto d1 = p1.dist();
  auto d2 = p2.dist();
  auto nn = dot(p1.normal(), p2.normal());
  auto c1 = (d1 - d2 * nn) / (1 - nn * nn);
  auto c2 = (d2 - d1 * nn) / (1 - nn * nn);
  auto R0 = c1 * p1.normal() + c2 * p2.normal();
  auto R = cross(p1.normal(), p2.normal());
  return Line{R0, R};
}

constexpr std::optional<Point> intersection(Plane const &p1, Plane const &p2,
                                            Plane const &p3) noexcept {
  if (complanar(Point{}, p1.normal(), p2.normal(), p3.normal()))
    return std::nullopt;

  Matrix<3, 3, double> M{t(p1.normal()), t(p2.normal()), t(p3.normal())};
  return inv(M) * Point{p1.dist(), p2.dist(), p3.dist()};
}

constexpr std::optional<Point> complanar_intersection(Segment const &s,
                                                      Line const &l) noexcept {
  auto seg = s.line();
  assert(complanar(seg, l, EPSMAX));
  auto intersection = complanar_intersection(seg, l);
  if (!intersection)
    // segment and line are parallel
    return std::nullopt;

  auto param = seg.param(intersection.value());
  if (0.0 <= param && param <= n(s.e - s.s))
    return intersection;
  return std::nullopt;
}

constexpr std::optional<Point>
complanar_intersection(Line const &l, Segment const &s) noexcept {
  return complanar_intersection(s, l);
}

constexpr std::optional<Point>
// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
complanar_intersection(Segment const &s1, Segment const &s2) noexcept {
  auto l1 = s1.line();
  auto l2 = s2.line();
  auto i1 = complanar_intersection(s1, l2);
  if (!i1)
    return std::nullopt;
  auto i2 = complanar_intersection(s2, l1);
  if (!i2)
    return std::nullopt;

  assert(n(i1.value() - i2.value()) <= EPSMAX * n(i1.value()));
  return i1;
}

} // namespace cpp_contests
