#define BOOST_TEST_MODULE Test
#define _CRT_SECURE_NO_WARNINGS // NOLINT

#include <vector>

#include <boost/test/included/unit_test.hpp>

#include "k_max/solution.hpp"

BOOST_AUTO_TEST_CASE(main_test) {
  BOOST_TEST((std::vector{5} == k_max({5}, 1)));
  BOOST_TEST((std::vector{0, 1, 2, 3, 4} == k_max({0, 1, 2, 3, 4}, 1)));
  BOOST_TEST((std::vector{4, 3, 2, 1, 0} == k_max({4, 3, 2, 1, 0}, 1)));
  BOOST_TEST((std::vector{1, 3, 2, 2, 2} == k_max({1, 3, 2, 2, 2}, 1)));
  BOOST_TEST((std::vector{6} == k_max({1, 3, 6, 2, 2}, 5)));
  BOOST_TEST((std::vector{2, 3, 4} == k_max({0, 1, 2, 3, 4}, 3)));
  BOOST_TEST((std::vector{4, 3, 2} == k_max({4, 3, 2, 1, 0}, 3)));
  BOOST_TEST((std::vector{6, 2, 2, 4, 6, 6, 6, 6, 5, 8, 8, 8, 8} ==
              k_max({6, 1, 2, 0, 0, 2, 4, 6, 1, 2, 3, 5, 8, 8, 8, 3}, 4)));
}
