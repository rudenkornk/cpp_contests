#define BOOST_TEST_MODULE Matrix
#define _CRT_SECURE_NO_WARNINGS // NOLINT(bugprone-reserved-identifier,cert-dcl37-c,cert-dcl51-cpp,readability-identifier-naming)

#include <array>
#include <cstddef>
#include <numbers>

#include <boost/test/included/unit_test.hpp>
#include <boost/test/tools/interface.hpp>
#include <boost/test/unit_test_suite.hpp>

import geometry;

using cpp_contests::eye;
using cpp_contests::Line;
using cpp_contests::m_inds;
using cpp_contests::Matrix;
using cpp_contests::Plane;
using cpp_contests::point;

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers,readability-identifier-length,readability-magic-numbers)

// NOLINTBEGIN(readability-function-cognitive-complexity)
namespace {
BOOST_AUTO_TEST_CASE(MatrixCtorsTest) {
  constexpr std::size_t x = 3;
  constexpr std::size_t y = 2;
  constexpr std::size_t count = x * y;
  constexpr std::array<int, count> a_splatted{1, 2, 3, 4, 5, 6};
  constexpr std::array<std::array<int, x>, y> a{{{1, 2, 3}, {4, 5, 6}}};
  constexpr std::array<std::array<int, y>, x> at{{{1, 4}, {2, 5}, {3, 6}}};
  constexpr Matrix<x, y, int> m_default_ctor{};
  constexpr Matrix<x, y, int> m_from_splatted(a_splatted);
  constexpr Matrix m_from_arr(a);
  constexpr Matrix<x, y, int> m_from_slist({1, 2, 3, 4, 5, 6});
  constexpr Matrix<x, y, int> m_from_llist({{1, 2, 3}, {4, 5, 6}});
  constexpr Matrix<x, y, int> m_from_range(a_splatted.begin(), a_splatted.end());
  constexpr Matrix m_from_tarr(at);
  constexpr Matrix<x, y, int> m_from_mat_range(m_from_arr.begin(), m_from_arr.end());
  constexpr Matrix<y, x, int> m_from_trange(m_from_arr.tbegin(), m_from_arr.tend());
  constexpr Matrix<1, 1, int> m_smallest{};
  constexpr int e = m_smallest;
  constexpr double small = 0.00001;

  BOOST_TEST(n(m_from_splatted - m_from_arr) < small);
  BOOST_TEST(n(m_from_mat_range - m_from_arr) < small);
  BOOST_TEST(n(m_from_range - m_from_arr) < small);
  // NOLINTBEGIN(cppcoreguidelines-pro-bounds-constant-array-index)
  for (std::size_t i = 0; i < x; ++i) {
    for (std::size_t j = 0; j < y; ++j) {
      auto n = i + (j * x);
      auto ijres = m_from_arr[i, j];
      auto ijres_splat = m_from_splatted[i, j];
      BOOST_TEST(ijres == a[j][i]);
      BOOST_TEST(m_from_arr[n] == a_splatted[n]);
      BOOST_TEST(ijres_splat == a[j][i]);
      BOOST_TEST(m_from_splatted[n] == a_splatted[n]);
      BOOST_TEST(m_from_slist[n] == a_splatted[n]);
      BOOST_TEST(m_from_llist[n] == a_splatted[n]);
      BOOST_TEST(m_default_ctor[n] == 0);
    }
  }
  // NOLINTEND(cppcoreguidelines-pro-bounds-constant-array-index)
  BOOST_TEST(n(transpose(m_from_trange) - m_from_arr) < small);
  BOOST_TEST(n(m_from_trange - m_from_tarr) < small);
  auto smallest = m_smallest[0, 0];
  BOOST_TEST(e == smallest);
}
// NOLINTEND(readability-function-cognitive-complexity)

BOOST_AUTO_TEST_CASE(MatrixCtorsTest2) {
  constexpr double small = 0.00001;
  constexpr Matrix<3, 3, int> m_check{0, 1, 2, 3, 4, 5, 6, 7, 8};

  constexpr Matrix<3, 1, int> m1{0, 1, 2};
  constexpr Matrix<3, 1, int> m2{3, 4, 5};
  constexpr Matrix<3, 1, int> m3{6, 7, 8};
  constexpr Matrix<3, 3, int> m(m1, m2, m3);
  constexpr Matrix<3, 3, int> matrix2(m);

  BOOST_TEST(n(m_check - m) < small);
  BOOST_TEST(n(m_check - matrix2) < small);

  constexpr Matrix<3, 3, int> matrix3(m);
  BOOST_TEST(n(m_check - matrix3) < small);

  constexpr Matrix<3, 2, int> m4{0, 1, 2, 3, 4, 5};
  constexpr Matrix<3, 1, int> m5{6, 7, 8};
  constexpr Matrix<3, 3, int> matrix4(m4, m5);

  BOOST_TEST(n(m_check - matrix4) < small);
}

BOOST_AUTO_TEST_CASE(MatrixCtorsTest3) {
  constexpr double small = 0.00001;
  constexpr Matrix<3, 3, int> m_check{0, 3, 6, 1, 4, 7, 2, 5, 8};

  constexpr Matrix<1, 3, int> m1{0, 1, 2};
  constexpr Matrix<1, 3, int> m2{3, 4, 5};
  constexpr Matrix<1, 3, int> m3{6, 7, 8};

  Matrix<3, 3, int> const m(m1, m2, m3);
  BOOST_TEST(n(m_check - m) < small);

  constexpr Matrix<2, 3, int> m4{0, 3, 1, 4, 2, 5};
  constexpr Matrix<1, 3, int> m5{6, 7, 8};
  Matrix<3, 3, int> const matrix4(m4, m5);
  BOOST_TEST(n(m_check - matrix4) < small);
}

BOOST_AUTO_TEST_CASE(MatrixCtorsTest4) {
  constexpr double small = 0.00001;
  constexpr Matrix<0, 3, int> m_zero_h{};
  constexpr Matrix<3, 0, int> m_zero_v{};
  constexpr Matrix<3, 3, int> m{0, 3, 6, 1, 4, 7, 2, 5, 8};

  Matrix<3, 3, int> const mv1(m, m_zero_h);
  Matrix<3, 3, int> const mv2(m_zero_h, m);
  Matrix<3, 3, int> const mh1(m, m_zero_v);
  Matrix<3, 3, int> const mh2(m_zero_v, m);

  BOOST_TEST(n(m - mv1) < small);
  BOOST_TEST(n(m - mv2) < small);
  BOOST_TEST(n(m - mh1) < small);
  BOOST_TEST(n(m - mh2) < small);
}

BOOST_AUTO_TEST_CASE(MatrixOpsTest) {
  constexpr std::size_t x = 3;
  constexpr std::size_t y = 2;
  Matrix<x, y, int> m{{1, 2, 3}, {4, 5, 6}};
  BOOST_TEST(+m == m);
  BOOST_TEST(-m == (Matrix<x, y, int>{-1, -2, -3, -4, -5, -6}));
  m += 1;
  BOOST_TEST(m == (Matrix<x, y, int>{2, 3, 4, 5, 6, 7}));
  m -= 1;
  BOOST_TEST(m == (Matrix<x, y, int>{1, 2, 3, 4, 5, 6}));
}

BOOST_AUTO_TEST_CASE(MatrixMultTest) {
  constexpr double small = 0.00001;

  // clang-format off
  constexpr Matrix<3, 2, int> m1{
    {1, 2, 3},
    {4, 5, 6}};
  constexpr Matrix<4, 3, int> m2{
    {1, 2, 0, 1},
    {4, 5, 1, 1},
    {0, 6, 2, 0},
  }; 
  constexpr Matrix<4, 2, int> mcheck{
    {9, 30, 8, 3},
    {24, 69, 17, 9},
  };
  constexpr auto prod = m1 * m2;
  BOOST_TEST(n(mcheck - prod) < small);
  // clang-format on
}

BOOST_AUTO_TEST_CASE(MatrixTransformTest) {
  // clang-format off
  constexpr Matrix<4, 3, int> m1{
    {1, 2, 0, 1},
    {4, 5, 1, 1},
    {0, 6, 2, 0},
  };
  constexpr Matrix<3, 2, int> mref{
    {1, 1, 1},
    {5, 5, 2}};
  // clang-format on
  constexpr auto m3 = t(m1[m_inds<2>{3, 1}, m_inds<3>{1, 1, 0}]);
  BOOST_TEST(m3 == mref);
}

BOOST_AUTO_TEST_CASE(MatrixDet) {
  constexpr Matrix<1, 1, int> m1{9};
  constexpr auto res1 = det(m1);
  static_assert(res1 == 9);

  // clang-format off
  constexpr Matrix<2, 2, int> m2{
    {9, 6},
    {1, 9},
  };
  // clang-format on
  constexpr auto res2 = det(m2);
  BOOST_TEST(res2 == 75);

  // clang-format off
  constexpr Matrix<3, 3, int> m3{
    {9, 6, 7},
    {0, 9, 8},
    {7, 1, 5},
  };
  // clang-format on
  constexpr auto res3 = det(m3);
  static_assert(res3 == 228);

  // clang-format off
  constexpr Matrix<4, 4, int> m4{
    {0, 0, 1, 0},
    {1, 3, 1, 1},
    {1, 2, 1, 3},
    {1, 2, 6, 1},
  };
  // clang-format on
  constexpr auto res4 = det(m4);
  static_assert(res4 == 2);

  // clang-format off
  constexpr Matrix<6, 6, int> m6{
    {9, 6, 7, 0, 7, 0},
    {0, 9, 8, 2, 1, 4},
    {7, 1, 5, 4, 7, 5},
    {0, 0, 2, 6, 8, 1},
    {1, 1, 8, 8, 3, 1},
    {4, 1, 8, 7, 4, 5},
  };
  // clang-format on
  constexpr auto res6 = det(m6);
  static_assert(res6 == 7905);
}

BOOST_AUTO_TEST_CASE(MatrixInv) {
  constexpr double eps = 1e-9;

  constexpr Matrix<1, 1, double> m1{9};
  constexpr auto res1 = inv(m1);
  static_assert(n((m1 * res1) - eye<1, 1, double>()) < eps);

  // clang-format off
  constexpr Matrix<2, 2, double> m2{
    {9, 6},
    {1, 9},
  };
  // clang-format on
  constexpr auto res2 = inv(m2);
  static_assert(n((m2 * res2) - eye<2, 2, double>()) < eps);

  // clang-format off
  constexpr Matrix<3, 3, double> m3{
    {9, 6, 7},
    {0, 9, 8},
    {7, 1, 5},
  };
  // clang-format on
  constexpr auto res3 = inv(m3);
  static_assert(n((m3 * res3) - eye<3, 3, double>()) < eps);

  // clang-format off
  constexpr Matrix<4, 4, double> m4{
    {0, 0, 1, 0},
    {1, 3, 1, 1},
    {1, 2, 1, 3},
    {1, 2, 6, 1},
  };
  // clang-format on
  constexpr auto res4 = inv(m4);
  static_assert(n((m4 * res4) - eye<4, 4, double>()) < eps);

  // clang-format off
  constexpr Matrix<6, 6, double> m6{
    {9, 6, 7, 0, 7, 0},
    {0, 9, 8, 2, 1, 4},
    {7, 1, 5, 4, 7, 5},
    {0, 0, 2, 6, 8, 1},
    {1, 1, 8, 8, 3, 1},
    {4, 1, 8, 7, 4, 5},
  };
  // clang-format on
  constexpr auto res6 = inv(m6);
  static_assert(n((m6 * res6) - eye<6, 6, double>()) < eps);
}

BOOST_AUTO_TEST_CASE(PlaneTest) {
  constexpr double eps = 1e-9;
  constexpr Plane p1(1, 0, 0, -1);
  constexpr Plane p2(0, 1, 0, -1);
  constexpr Plane p3(0, 0, 1, -1);
  constexpr auto r2 = std::numbers::sqrt2;
  Line const ref_inter_12{point{r2, r2, 0}, point{0, 0, 1}};
  Line const ref_inter_23{point{0, r2, r2}, point{1, 0, 0}};
  Line const ref_inter_31{point{r2, 0, r2}, point{0, 1, 0}};

  // NOLINTBEGIN(bugprone-unchecked-optional-access)
  constexpr Line inter12 = intersection(p1, p2).value();
  constexpr Line inter23 = intersection(p2, p3).value();
  constexpr Line inter31 = intersection(p3, p1).value();
  // NOLINTEND(bugprone-unchecked-optional-access)

  auto diff12 = n(inter12.r() - ref_inter_12.r());
  auto diff23 = n(inter23.r() - ref_inter_23.r());
  auto diff31 = n(inter31.r() - ref_inter_31.r());

  BOOST_TEST(diff12 < eps);
  BOOST_TEST(diff23 < eps);
  BOOST_TEST(diff31 < eps);
}

} // namespace

// NOLINTEND(cppcoreguidelines-avoid-magic-numbers,readability-identifier-length,readability-magic-numbers)
