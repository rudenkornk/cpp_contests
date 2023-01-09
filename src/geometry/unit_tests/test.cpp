// NOLINTBEGIN(cppcoreguidelines-pro-type-vararg)
// NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)

#define BOOST_TEST_MODULE Matrix // NOLINT
#define _CRT_SECURE_NO_WARNINGS  // NOLINT

#include <boost/test/included/unit_test.hpp>

#include "geometry/matrix.hpp"
#include "geometry/primitives.hpp"

using namespace cpp_contests;

BOOST_AUTO_TEST_CASE(matrix_ctors_test) {
  constexpr std::size_t X = 3;
  constexpr std::size_t Y = 2;
  constexpr std::size_t N = X * Y;
  constexpr std::array<int, N> a_splatted{1, 2, 3, 4, 5, 6};
  constexpr std::array<std::array<int, X>, Y> a{{{1, 2, 3}, {4, 5, 6}}};
  constexpr std::array<std::array<int, Y>, X> at{{{1, 4}, {2, 5}, {3, 6}}};
  constexpr Matrix<X, Y, int> m_default_ctor{};
  constexpr Matrix<X, Y, int> m_from_splatted(a_splatted);
  constexpr Matrix m_from_arr(a);
  constexpr Matrix<X, Y, int> m_from_slist({1, 2, 3, 4, 5, 6});
  constexpr Matrix<X, Y, int> m_from_llist({{1, 2, 3}, {4, 5, 6}});
  constexpr Matrix<X, Y, int> m_from_range(a_splatted.begin(),
                                           a_splatted.end());
  constexpr Matrix m_from_tarr(at);
  constexpr Matrix<X, Y, int> m_from_mat_range(m_from_arr.begin(),
                                               m_from_arr.end());
  constexpr Matrix<Y, X, int> m_from_trange(m_from_arr.tbegin(),
                                            m_from_arr.tend());
  constexpr Matrix<1, 1, int> m_smallest{};
  constexpr int e = m_smallest;
  constexpr double small = 0.00001;

  BOOST_TEST(n(m_from_splatted - m_from_arr) < small);
  BOOST_TEST(n(m_from_mat_range - m_from_arr) < small);
  BOOST_TEST(n(m_from_range - m_from_arr) < small);
  for (std::size_t i = 0; i < X; ++i) {
    for (std::size_t j = 0; j < Y; ++j) {
      auto n = i + j * X;
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
  BOOST_TEST(n(transpose(m_from_trange) - m_from_arr) < small);
  BOOST_TEST(n(m_from_trange - m_from_tarr) < small);
  auto smallest = m_smallest[0, 0];
  BOOST_TEST(e == smallest);
}

BOOST_AUTO_TEST_CASE(matrix_ctors_test_2) {
  constexpr double small = 0.00001;
  constexpr Matrix<3, 3, int> MCheck{0, 1, 2, 3, 4, 5, 6, 7, 8};

  constexpr Matrix<3, 1, int> m1{0, 1, 2};
  constexpr Matrix<3, 1, int> m2{3, 4, 5};
  constexpr Matrix<3, 1, int> m3{6, 7, 8};
  constexpr Matrix<3, 3, int> M(m1, m2, m3);
  constexpr Matrix<3, 3, int> M2(M);

  BOOST_TEST(n(MCheck - M) < small);
  BOOST_TEST(n(MCheck - M2) < small);

  constexpr Matrix<3, 3, int> M3(std::move(M));
  BOOST_TEST(n(MCheck - M3) < small);

  constexpr Matrix<3, 2, int> m4{0, 1, 2, 3, 4, 5};
  constexpr Matrix<3, 1, int> m5{6, 7, 8};
  constexpr Matrix<3, 3, int> M4(m4, m5);

  BOOST_TEST(n(MCheck - M4) < small);
}

BOOST_AUTO_TEST_CASE(matrix_ctors_test_3) {
  constexpr double small = 0.00001;
  constexpr Matrix<3, 3, int> MCheck{0, 3, 6, 1, 4, 7, 2, 5, 8};

  constexpr Matrix<1, 3, int> m1{0, 1, 2};
  constexpr Matrix<1, 3, int> m2{3, 4, 5};
  constexpr Matrix<1, 3, int> m3{6, 7, 8};

  Matrix<3, 3, int> M(m1, m2, m3);
  BOOST_TEST(n(MCheck - M) < small);

  constexpr Matrix<2, 3, int> m4{0, 3, 1, 4, 2, 5};
  constexpr Matrix<1, 3, int> m5{6, 7, 8};
  Matrix<3, 3, int> M4(std::move(m4), m5);
  BOOST_TEST(n(MCheck - M4) < small);
}

BOOST_AUTO_TEST_CASE(matrix_ctors_test_4) {
  constexpr double small = 0.00001;
  constexpr Matrix<0, 3, int> MZeroH{};
  constexpr Matrix<3, 0, int> MZeroV{};
  constexpr Matrix<3, 3, int> M{0, 3, 6, 1, 4, 7, 2, 5, 8};

  Matrix<3, 3, int> MV1(M, MZeroH);
  Matrix<3, 3, int> MV2(MZeroH, M);
  Matrix<3, 3, int> MH1(M, MZeroV);
  Matrix<3, 3, int> MH2(MZeroV, M);

  BOOST_TEST(n(M - MV1) < small);
  BOOST_TEST(n(M - MV2) < small);
  BOOST_TEST(n(M - MH1) < small);
  BOOST_TEST(n(M - MH2) < small);
}

BOOST_AUTO_TEST_CASE(matrix_ops_test) {
  constexpr std::size_t X = 3;
  constexpr std::size_t Y = 2;
  Matrix<X, Y, int> m{{1, 2, 3}, {4, 5, 6}}; // NOLINT
  BOOST_TEST(+m == m);
  BOOST_TEST(-m == (Matrix<X, Y, int>{-1, -2, -3, -4, -5, -6}));
  m += 1;
  BOOST_TEST(m == (Matrix<X, Y, int>{2, 3, 4, 5, 6, 7}));
  m -= 1;
  BOOST_TEST(m == (Matrix<X, Y, int>{1, 2, 3, 4, 5, 6}));
}

BOOST_AUTO_TEST_CASE(matrix_mult_test) {
  constexpr double small = 0.00001;

  // clang-format off
  constexpr Matrix<3, 2, int> m1{
    {1, 2, 3},
    {4, 5, 6}}; // NOLINT
  constexpr Matrix<4, 3, int> m2{
    {1, 2, 0, 1},
    {4, 5, 1, 1},
    {0, 6, 2, 0},
  }; // NOLINT
  constexpr Matrix<4, 2, int> mcheck{
    {9, 30, 8, 3},
    {24, 69, 17, 9},
  };
  constexpr auto prod = m1 * m2;
  BOOST_TEST(n(mcheck - prod) < small);
  // clang-format on
}

BOOST_AUTO_TEST_CASE(matrix_transform_test) {
  // clang-format off
  constexpr Matrix<4, 3, int> m1{
    {1, 2, 0, 1},
    {4, 5, 1, 1},
    {0, 6, 2, 0},
  }; // NOLINT
  constexpr Matrix<3, 2, int> mref{
    {1, 1, 1},
    {5, 5, 2}}; // NOLINT
  // clang-format on
  constexpr auto m3 = t(m1[MInds<2>{3, 1}, MInds<3>{1, 1, 0}]);
  BOOST_TEST(m3 == mref);
}

BOOST_AUTO_TEST_CASE(matrix_det) {
  constexpr Matrix<1, 1, int> m1{9};
  constexpr auto res1 = det(m1);
  // NOLINTNEXTLINE
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
  // NOLINTNEXTLINE
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
  // NOLINTNEXTLINE
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
  // NOLINTNEXTLINE
  static_assert(res6 == 7905);
}

BOOST_AUTO_TEST_CASE(matrix_inv) {
  constexpr double eps = 1e-9;

  constexpr Matrix<1, 1, double> m1{9};
  constexpr auto res1 = inv(m1);
  // NOLINTNEXTLINE
  static_assert(n(m1 * res1 - eye<1, 1, double>()) < eps);

  // clang-format off
  constexpr Matrix<2, 2, double> m2{
    {9, 6},
    {1, 9},
  };
  // clang-format on
  constexpr auto res2 = inv(m2);
  static_assert(n(m2 * res2 - eye<2, 2, double>()) < eps);

  // clang-format off
  constexpr Matrix<3, 3, double> m3{
    {9, 6, 7},
    {0, 9, 8},
    {7, 1, 5},
  };
  // clang-format on
  constexpr auto res3 = inv(m3);
  // NOLINTNEXTLINE
  static_assert(n(m3 * res3 - eye<3, 3, double>()) < eps);

  // clang-format off
  constexpr Matrix<4, 4, double> m4{
    {0, 0, 1, 0},
    {1, 3, 1, 1},
    {1, 2, 1, 3},
    {1, 2, 6, 1},
  };
  // clang-format on
  constexpr auto res4 = inv(m4);
  // NOLINTNEXTLINE
  static_assert(n(m4 * res4 - eye<4, 4, double>()) < eps);

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
  // NOLINTNEXTLINE
  static_assert(n(m6 * res6 - eye<6, 6, double>()) < eps);
}

BOOST_AUTO_TEST_CASE(plane_test) {
  constexpr double eps = 1e-9;
  constexpr Plane p1(1, 0, 0, -1);
  constexpr Plane p2(0, 1, 0, -1);
  constexpr Plane p3(0, 0, 1, -1);
  constexpr auto r2 = cpp_contests::sqrt(2);
  Line ref_inter_12{Point{r2, r2, 0}, Point{0, 0, 1}};
  Line ref_inter_23{Point{0, r2, r2}, Point{1, 0, 0}};
  Line ref_inter_31{Point{r2, 0, r2}, Point{0, 1, 0}};

  constexpr Line inter12 = intersection(p1, p2).value(); // NOLINT
  constexpr Line inter23 = intersection(p2, p3).value(); // NOLINT
  constexpr Line inter31 = intersection(p3, p1).value(); // NOLINT

  auto diff12 = n(inter12.R() - ref_inter_12.R());
  auto diff23 = n(inter23.R() - ref_inter_23.R());
  auto diff31 = n(inter31.R() - ref_inter_31.R());

  BOOST_TEST(diff12 < eps);
  BOOST_TEST(diff23 < eps);
  BOOST_TEST(diff31 < eps);
}

// NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)
// NOLINTEND(cppcoreguidelines-pro-type-vararg)
