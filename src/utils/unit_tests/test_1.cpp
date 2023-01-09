// NOLINTBEGIN(cppcoreguidelines-pro-type-vararg)
// NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)

#include <cstddef>
#define BOOST_TEST_MODULE Test  // NOLINT
#define _CRT_SECURE_NO_WARNINGS // NOLINT

#include <boost/test/included/unit_test.hpp>

#include "utils/math.hpp"
#include "utils/utils.hpp"

using namespace cpp_contests;

BOOST_AUTO_TEST_CASE(size_to_string_test) {
  BOOST_TEST(size_to_string(std::size_t{0}) == "0 B");
  BOOST_TEST(size_to_string(std::size_t{1}) == "1 B");
  BOOST_TEST(size_to_string(std::size_t{1024}) == "1 KiB");
  BOOST_TEST(size_to_string(std::size_t{1024} * 1024) == "1 MiB");
  BOOST_TEST(size_to_string(std::size_t{1024} * 1024 * 1024) == "1 GiB");
  BOOST_TEST(size_to_string(std::size_t{1024} * 1024 * 1024 * 1024) == "1 TiB");
  BOOST_TEST(size_to_string(std::size_t{1024} * 1024 * 1024 * 1024 * 1024) ==
             "1 PiB");
  BOOST_TEST(size_to_string(std::size_t{1024} * 1024 * 1024 * 1024 * 1024 *
                            1024) == "1 EiB");
  BOOST_TEST(size_to_string(std::size_t{17}) == "17 B");
  BOOST_TEST(size_to_string(std::size_t{1025}) == "1.0 KiB");
  BOOST_TEST(size_to_string(std::size_t{1129}) == "1.1 KiB");
}

// NOLINTNEXTLINE
constexpr double pow2(double x, std::size_t n) {
  double res = 1;
  for (std::size_t i = 0; i < n; ++i)
    res *= x;
  return res;
}

constexpr bool sqrt_check() {
  const double start = 1e-6;
  const double mult = 1.1;
  const double eps = 1e-7;
  const std::size_t N = 300;
  bool result = true;
  for (std::size_t i = 0; i < N; ++i) {
    double curr = pow2(mult, i) * start;
    double res = cpp_contests::sqrt(curr * curr);
    double diff = res - curr;
    result &= std::abs(diff) < eps;
  }
  return result;
}

BOOST_AUTO_TEST_CASE(sqrt_test) { static_assert(sqrt_check()); }

// NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)
// NOLINTEND(cppcoreguidelines-pro-type-vararg)
