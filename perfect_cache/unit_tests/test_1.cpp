// NOLINTBEGIN(cppcoreguidelines-pro-type-vararg)
// NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)

#define BOOST_TEST_MODULE Test  // NOLINT
#define _CRT_SECURE_NO_WARNINGS // NOLINT

#include <cassert>
#include <utility>
#include <vector>

#include <boost/test/included/unit_test.hpp>

#include "perfect_cache/solution.hpp"

using namespace cpp_contests;

BOOST_AUTO_TEST_CASE(main_test) {
  BOOST_TEST(perfect_cache(0, {}) == std::size_t{0});
  BOOST_TEST(perfect_cache(0, {1}) == std::size_t{0});
  BOOST_TEST(perfect_cache(0, {1, 2, 3, 4}) == std::size_t{0});
  BOOST_TEST(perfect_cache(1, {1, 2, 3, 4}) == std::size_t{0});
  BOOST_TEST(perfect_cache(1, {1, 1, 1, 1}) == std::size_t{3});
  BOOST_TEST(perfect_cache(1, {1, 2, 1, 2}) == std::size_t{1});
  BOOST_TEST(perfect_cache(1, {1, 2, 2, 1}) == std::size_t{1});
  BOOST_TEST(perfect_cache(1, {1, 2, 3, 4, 5, 6, 1}) == std::size_t{1});
  BOOST_TEST(perfect_cache(2, {1, 2, 3, 4, 5, 6, 1}) == std::size_t{1});
  BOOST_TEST(perfect_cache(2, {1, 2, 3, 4, 5, 2, 1}) == std::size_t{2});
  BOOST_TEST(perfect_cache(4, {1, 2, 3, 1, 2, 4, 5, 1, 7, 3, 2, 6, 1, 2}) ==
             std::size_t{7});
}

// NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)
// NOLINTEND(cppcoreguidelines-pro-type-vararg)
