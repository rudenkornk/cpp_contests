// NOLINTBEGIN(cppcoreguidelines-pro-type-vararg)
// NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)

#define BOOST_TEST_MODULE Test  // NOLINT
#define _CRT_SECURE_NO_WARNINGS // NOLINT

#include <cassert>
#include <utility>
#include <vector>

#include <boost/test/included/unit_test.hpp>

#include "missing_numbers/solution.hpp"

using namespace cpp_contests;

BOOST_AUTO_TEST_CASE(main_test) {
  BOOST_TEST((std::pair(0u, 1u) == missing_numbers({})));
  BOOST_TEST((std::pair(0u, 1u) == missing_numbers({2})));
  BOOST_TEST((std::pair(0u, 1u) == missing_numbers({3, 2})));
  BOOST_TEST((std::pair(0u, 1u) == missing_numbers({4, 3, 2})));
  BOOST_TEST((std::pair(2u, 6u) == missing_numbers({7, 4, 1, 0, 3, 5})));
  BOOST_TEST((std::pair(2u, 5u) == missing_numbers({7, 4, 1, 0, 3, 6})));
  BOOST_TEST(
      (std::pair(2u, 5u) == missing_numbers({7, 4, 1, 0, 3, 6, 9, 11, 10, 8})));
}

// NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)
// NOLINTEND(cppcoreguidelines-pro-type-vararg)
