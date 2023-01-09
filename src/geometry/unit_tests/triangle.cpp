// NOLINTBEGIN(cppcoreguidelines-pro-type-vararg)
// NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)

#define BOOST_TEST_MODULE Matrix // NOLINT
#define _CRT_SECURE_NO_WARNINGS  // NOLINT

#include <boost/test/included/unit_test.hpp>

#include "geometry/triangle.hpp"

using namespace cpp_contests;

BOOST_AUTO_TEST_CASE(triangle_test) {
  constexpr Triangle t1(Point{0.0, 0.0, 0.0}, Point{1.0, 0.0, 0.0},
                        Point{0.0, 1.0, 0.0});
  constexpr Triangle t2(Point{0.0, 0.0, 0.0}, Point{1.0, 1.0, 1.0},
                        Point{1.0, 1.0, -1.0});
  constexpr Triangle t3(Point{0.0, 0.0, 0.5}, Point{1.0, 0.0, 0.5},
                        Point{0.0, 1.0, 0.5});
  constexpr Triangle t4(Point{0.0, 0.0, 0.0}, Point{1.0, 0.0, 0.0},
                        Point{0.0, 0.0, 1.0});
  constexpr Triangle t5(Point{0.0, 0.0, 0.1}, Point{1.0, 0.0, 0.1},
                        Point{0.0, 0.0, 1.1});

  static_assert(intersects(t1, t1));
  static_assert(intersects(t2, t2));
  static_assert(intersects(t3, t3));

  static_assert(intersects(t1, t2));
  static_assert(intersects(t2, t1));
  static_assert(!intersects(t1, t3));
  static_assert(intersects(t2, t3));
  static_assert(intersects(t1, t4));
  static_assert(!intersects(t1, t5));

  // static_assert(intersect(t1, t1));
}

// NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)
// NOLINTEND(cppcoreguidelines-pro-type-vararg)
