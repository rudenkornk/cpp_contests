#define BOOST_TEST_MODULE Matrix
#define _CRT_SECURE_NO_WARNINGS // NOLINT(bugprone-reserved-identifier,cert-dcl37-c,cert-dcl51-cpp,readability-identifier-naming)

#include <boost/test/included/unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>

import geometry;

using cpp_contests::point;
using cpp_contests::Triangle;

// NOLINTBEGIN(readability-identifier-length)

BOOST_AUTO_TEST_CASE(TriangleTest) {
  constexpr Triangle t1(point{0.0, 0.0, 0.0}, point{1.0, 0.0, 0.0}, point{0.0, 1.0, 0.0});
  constexpr Triangle t2(point{0.0, 0.0, 0.0}, point{1.0, 1.0, 1.0}, point{1.0, 1.0, -1.0});
  constexpr Triangle t3(point{0.0, 0.0, 0.5}, point{1.0, 0.0, 0.5}, point{0.0, 1.0, 0.5});
  constexpr Triangle t4(point{0.0, 0.0, 0.0}, point{1.0, 0.0, 0.0}, point{0.0, 0.0, 1.0});
  constexpr Triangle t5(point{0.0, 0.0, 0.1}, point{1.0, 0.0, 0.1}, point{0.0, 0.0, 1.1});

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

// NOLINTEND(readability-identifier-length)
