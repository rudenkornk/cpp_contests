#define BOOST_TEST_MODULE Matrix // NOLINT(cppcoreguidelines-macro-usage)
#define _CRT_SECURE_NO_WARNINGS  // NOLINT(bugprone-reserved-identifier,cert-dcl37-c,cert-dcl51-cpp)

#include <cstddef>
#include <vector>

#include <boost/test/included/unit_test.hpp>

import two_queue_cache.solution;

using cpp_contests::two_queue_hits;

namespace {
// Helper function to compute cache hits using the same parameters as CLI
auto compute_hits(std::vector<int> const &elements, std::size_t cache_size) -> std::size_t {
  constexpr std::size_t virtual_value_size = std::size_t{1024} * 1024 * 1024;
  constexpr std::size_t key_size = sizeof(int);
  return two_queue_hits(elements, cache_size * (virtual_value_size + (key_size * 3) / 2), virtual_value_size);
}
} // namespace

BOOST_AUTO_TEST_CASE(library_function_tests) {
  BOOST_TEST(compute_hits({}, 0) == std::size_t{0});
  BOOST_TEST(compute_hits({1}, 0) == std::size_t{0});
  BOOST_TEST(compute_hits({1, 2, 3, 4, 5}, 0) == std::size_t{0});
  BOOST_TEST(compute_hits({1}, 1) == std::size_t{0});
  BOOST_TEST(compute_hits({1, 1, 1}, 1) == std::size_t{0});
  BOOST_TEST(compute_hits({1, 1, 2}, 1) == std::size_t{0});
  BOOST_TEST(compute_hits({1, 2, 1}, 1) == std::size_t{0});
  BOOST_TEST(compute_hits({1, 2, 3}, 2) == std::size_t{0});
  BOOST_TEST(compute_hits({1, 2, 1}, 2) == std::size_t{0});
  BOOST_TEST(compute_hits({1, 1, 1}, 2) == std::size_t{1});
  BOOST_TEST(compute_hits({1, 1, 2, 2, 3, 3, 1}, 2) == std::size_t{0});
  BOOST_TEST(compute_hits({1, 2, 2, 3, 3, 2, 3}, 2) == std::size_t{2});
  BOOST_TEST(compute_hits({1, 2, 3, 1, 2, 4, 5, 1, 7, 3, 2, 6, 1, 2}, 4) == std::size_t{4});
  BOOST_TEST(compute_hits({1, 2, 3, 4, 1, 2, 5, 1, 2, 4, 3, 4, 2, 1, 4, 4, 2, 1, 5, 1, 2, 5, 2, 5, 1}, 4) ==
             std::size_t{12});
  BOOST_TEST(compute_hits({1, 2, 3, 4, 1, 2, 5, 1, 2, 4, 3, 4}, 4) == std::size_t{0});
  BOOST_TEST(compute_hits({1, 2, 1, 2, 1, 2}, 2) == std::size_t{0});
}
