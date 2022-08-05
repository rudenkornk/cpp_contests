// NOLINTBEGIN(cppcoreguidelines-pro-type-vararg)
// NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)

#define BOOST_TEST_MODULE Test  // NOLINT
#define _CRT_SECURE_NO_WARNINGS // NOLINT

#include <boost/test/included/unit_test.hpp>

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

// NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)
// NOLINTEND(cppcoreguidelines-pro-type-vararg)
