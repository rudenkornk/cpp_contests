#define BOOST_TEST_MODULE Matrix // NOLINT(cppcoreguidelines-macro-usage)
#define _CRT_SECURE_NO_WARNINGS  // NOLINT(bugprone-reserved-identifier,cert-dcl37-c,cert-dcl51-cpp)

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <format>
#include <iostream>
#include <iterator>
#include <random>
#include <vector>

#include <boost/test/included/unit_test.hpp>
#include <boost/test/tools/interface.hpp>
#include <boost/test/unit_test_suite.hpp>

import utils.generic;
import lru_cache.solution;
import perfect_cache.solution;
import two_queue_cache.solution;

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers,readability-identifier-length,readability-magic-numbers)
using cpp_contests::perfect_cache;
using cpp_contests::size_to_string;

namespace {
// Assume that sizeof(Value) >> sizeof(Key), then
// how many cache hits would be if you can store at max 'pages' number of Values
auto perfect_cache_wrapper(std::vector<int> const &keys, std::size_t pages) -> std::size_t {
  return perfect_cache(keys, pages * sizeof(int), 0);
}
} // namespace

BOOST_AUTO_TEST_CASE(correctness_test) {
  BOOST_TEST(perfect_cache_wrapper({}, 0) == std::size_t{0});
  BOOST_TEST(perfect_cache_wrapper({1}, 0) == std::size_t{0});
  BOOST_TEST(perfect_cache_wrapper({1, 2, 3, 4}, 0) == std::size_t{0});
  BOOST_TEST(perfect_cache_wrapper({1, 2, 3, 4}, 1) == std::size_t{0});
  BOOST_TEST(perfect_cache_wrapper({1, 1, 1, 1}, 1) == std::size_t{3});
  BOOST_TEST(perfect_cache_wrapper({1, 2, 1, 2}, 1) == std::size_t{1});
  BOOST_TEST(perfect_cache_wrapper({1, 2, 2, 1}, 1) == std::size_t{1});
  BOOST_TEST(perfect_cache_wrapper({1, 2, 3, 4, 5, 6, 1}, 1) == std::size_t{1});
  BOOST_TEST(perfect_cache_wrapper({1, 2, 3, 4, 5, 6, 1}, 2) == std::size_t{1});
  BOOST_TEST(perfect_cache_wrapper({1, 2, 3, 4, 5, 2, 1}, 2) == std::size_t{2});
  BOOST_TEST(perfect_cache_wrapper({1, 2, 3, 1, 2, 4, 5, 1, 7, 3, 2, 6, 1, 2}, 4) == std::size_t{7});
}

using Key = int;
static constexpr std::size_t typical_web_page_size_in_bytes = std::size_t{2} * 1024 * 1024;
static constexpr std::size_t typical_db_key_size_in_bytes = 32;
static constexpr std::size_t actual_key_size_in_benchmark = sizeof(Key);
static constexpr std::size_t virtual_web_page_size_in_benchmark =
    typical_web_page_size_in_bytes / typical_db_key_size_in_bytes * actual_key_size_in_benchmark;

BOOST_AUTO_TEST_CASE(uniform_distribution_cache_test) {
  const double cents = 100.0;
  const std::size_t cache_size_in_bytes = 100 * virtual_web_page_size_in_benchmark;
  const std::size_t n_elements = 10000;
  const int min = 1;
  const int max = 300;
  std::mt19937 gen{0}; // NOLINT(cert-msc32-c,cert-msc51-cpp)
  std::uniform_int_distribution<> d{min, max};
  std::vector<Key> elements{};
  elements.reserve(n_elements);
  std::generate_n(std::back_inserter(elements), n_elements, [&]() -> int { return d(gen); });
  auto &&lru = cpp_contests::lru_hits(elements, cache_size_in_bytes, virtual_web_page_size_in_benchmark);
  auto &&two_queue = cpp_contests::two_queue_hits(elements, cache_size_in_bytes, virtual_web_page_size_in_benchmark);
  auto &&perfect = cpp_contests::perfect_cache(elements, cache_size_in_bytes, virtual_web_page_size_in_benchmark);
  std::cout << "Cache results for uniform_int_distribution:\n";
  std::cout << "Cache size=" << size_to_string(cache_size_in_bytes)
            << ", page size=" << size_to_string(virtual_web_page_size_in_benchmark)
            << ", keys amplitude=" << (max - min + 1) << ", pool size=" << n_elements << "\n";
  std::cout << "LRU cache hit rate: ";
  std::cout << std::format("{:.2f}%\n", cents * static_cast<double>(lru) / n_elements);
  std::cout << "2Q cache hit rate: ";
  std::cout << std::format("{:.2f}%\n", cents * static_cast<double>(two_queue) / n_elements);
  std::cout << "Perfect cache hit rate: ";
  std::cout << std::format("{:.2f}%\n\n", cents * static_cast<double>(perfect) / n_elements);
  BOOST_TEST(lru <= perfect);
}

BOOST_AUTO_TEST_CASE(binomial_distribution_cache_test) {
  const double cents = 100.0;
  const std::size_t cache_size_in_bytes = 100 * virtual_web_page_size_in_benchmark;
  const std::size_t n_elements = 10000;
  const int max = 2999;
  std::mt19937 gen{0}; // NOLINT(cert-msc32-c,cert-msc51-cpp)
  std::binomial_distribution<> d(max);
  std::vector<Key> elements{};
  elements.reserve(n_elements);
  std::generate_n(std::back_inserter(elements), n_elements, [&]() -> int { return d(gen); });
  auto &&lru = cpp_contests::lru_hits(elements, cache_size_in_bytes, virtual_web_page_size_in_benchmark);
  auto &&two_queue = cpp_contests::two_queue_hits(elements, cache_size_in_bytes, virtual_web_page_size_in_benchmark);
  auto &&perfect = cpp_contests::perfect_cache(elements, cache_size_in_bytes, virtual_web_page_size_in_benchmark);
  std::cout << "Cache results for binomial_distribution with perfect coin:\n";
  std::cout << "Cache size=" << size_to_string(cache_size_in_bytes)
            << ", page size=" << size_to_string(virtual_web_page_size_in_benchmark) << ", keys amplitude=" << (max + 1)
            << ", pool size=" << n_elements << "\n";
  std::cout << "LRU cache hit rate: ";
  std::cout << std::format("{:.2f}%\n", cents * static_cast<double>(lru) / n_elements);
  std::cout << "2Q cache hit rate: ";
  std::cout << std::format("{:.2f}%\n", cents * static_cast<double>(two_queue) / n_elements);
  std::cout << "Perfect cache hit rate: ";
  std::cout << std::format("{:.2f}%\n\n", cents * static_cast<double>(perfect) / n_elements);
  BOOST_TEST(lru <= perfect);
}

BOOST_AUTO_TEST_CASE(poisson_distribution_cache_test) {
  const double cents = 100.0;
  const std::size_t cache_size_in_bytes = 100 * virtual_web_page_size_in_benchmark;
  const std::size_t n_elements = 10000;
  const int lambda = 1000;
  std::mt19937 gen{0}; // NOLINT(cert-msc32-c,cert-msc51-cpp)
  std::poisson_distribution<> d(lambda);
  std::vector<Key> elements{};
  elements.reserve(n_elements);
  std::generate_n(std::back_inserter(elements), n_elements, [&]() -> int { return d(gen); });
  auto &&lru = cpp_contests::lru_hits(elements, cache_size_in_bytes, virtual_web_page_size_in_benchmark);
  auto &&two_queue = cpp_contests::two_queue_hits(elements, cache_size_in_bytes, virtual_web_page_size_in_benchmark);
  auto &&perfect = cpp_contests::perfect_cache(elements, cache_size_in_bytes, virtual_web_page_size_in_benchmark);
  std::cout << "Cache results for poisson_distribution:\n";
  std::cout << "Cache size=" << size_to_string(cache_size_in_bytes)
            << ", page size=" << size_to_string(virtual_web_page_size_in_benchmark) << ", pool size=" << n_elements
            << "\n";
  std::cout << "LRU cache hit rate: ";
  std::cout << std::format("{:.2f}%\n", cents * static_cast<double>(lru) / n_elements);
  std::cout << "2Q cache hit rate: ";
  std::cout << std::format("{:.2f}%\n", cents * static_cast<double>(two_queue) / n_elements);
  std::cout << "Perfect cache hit rate: ";
  std::cout << std::format("{:.2f}%\n\n", cents * static_cast<double>(perfect) / n_elements);
  BOOST_TEST(lru <= perfect);
}

// NOLINTEND(cppcoreguidelines-avoid-magic-numbers,readability-identifier-length,readability-magic-numbers)
