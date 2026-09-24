#define BOOST_TEST_MODULE Matrix
#define _CRT_SECURE_NO_WARNINGS // NOLINT(bugprone-reserved-identifier,cert-dcl37-c,cert-dcl51-cpp,readability-identifier-naming)

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

import utils;
import lru_cache;
import perfect_cache;
import two_queue_cache;

using cpp_contests::perfect_cache;
using cpp_contests::size_to_string;

// NOLINTBEGIN(readability-identifier-length)

namespace {
// Assume that sizeof(Value) >> sizeof(Key), then
// how many cache hits would be if you can store at max 'pages' number of Values
auto perfect_cache_wrapper(std::vector<int> const &keys, std::size_t pages) -> std::size_t {
  return perfect_cache(keys, pages * sizeof(int), 0);
}
} // namespace

BOOST_AUTO_TEST_CASE(CorrectnessTest) {
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

using key_type = int;
static constexpr std::size_t kTypicalWebPageSizeInBytes = std::size_t{2} * 1024 * 1024;
static constexpr std::size_t kTypicalDbKeySizeInBytes = 32;
static constexpr std::size_t kActualKeySizeInBenchmark = sizeof(key_type);
static constexpr std::size_t kVirtualWebPageSizeInBenchmark =
    kTypicalWebPageSizeInBytes / kTypicalDbKeySizeInBytes * kActualKeySizeInBenchmark;

BOOST_AUTO_TEST_CASE(UniformDistributionCacheTest) {
  const double cents = 100.0;
  const std::size_t cache_size_in_bytes = 100 * kVirtualWebPageSizeInBenchmark;
  const std::size_t n_elements = 10000;
  const int min = 1;
  const int max = 300;
  std::mt19937 gen{0}; // NOLINT(cert-msc32-c,cert-msc51-cpp)
  std::uniform_int_distribution<> d{min, max};
  std::vector<key_type> elements{};
  elements.reserve(n_elements);
  std::generate_n(std::back_inserter(elements), n_elements, [&]() -> int { return d(gen); });
  auto &&lru = cpp_contests::lru_hits(elements, cache_size_in_bytes, kVirtualWebPageSizeInBenchmark);
  auto &&two_queue = cpp_contests::two_queue_hits(elements, cache_size_in_bytes, kVirtualWebPageSizeInBenchmark);
  auto &&perfect = cpp_contests::perfect_cache(elements, cache_size_in_bytes, kVirtualWebPageSizeInBenchmark);
  std::cout << "Cache results for uniform_int_distribution:\n";
  std::cout << "Cache size=" << size_to_string(cache_size_in_bytes)
            << ", page size=" << size_to_string(kVirtualWebPageSizeInBenchmark)
            << ", keys amplitude=" << (max - min + 1) << ", pool size=" << n_elements << "\n";
  std::cout << "LRU cache hit rate: ";
  std::cout << std::format("{:.2f}%\n", cents * static_cast<double>(lru) / n_elements);
  std::cout << "2Q cache hit rate: ";
  std::cout << std::format("{:.2f}%\n", cents * static_cast<double>(two_queue) / n_elements);
  std::cout << "Perfect cache hit rate: ";
  std::cout << std::format("{:.2f}%\n\n", cents * static_cast<double>(perfect) / n_elements);
  BOOST_TEST(lru <= perfect);
}

BOOST_AUTO_TEST_CASE(BinomialDistributionCacheTest) {
  const double cents = 100.0;
  const std::size_t cache_size_in_bytes = 100 * kVirtualWebPageSizeInBenchmark;
  const std::size_t n_elements = 10000;
  const int max = 2999;
  std::mt19937 gen{0}; // NOLINT(cert-msc32-c,cert-msc51-cpp)
  std::binomial_distribution<> d(max);
  std::vector<key_type> elements{};
  elements.reserve(n_elements);
  std::generate_n(std::back_inserter(elements), n_elements, [&]() -> int { return d(gen); });
  auto &&lru = cpp_contests::lru_hits(elements, cache_size_in_bytes, kVirtualWebPageSizeInBenchmark);
  auto &&two_queue = cpp_contests::two_queue_hits(elements, cache_size_in_bytes, kVirtualWebPageSizeInBenchmark);
  auto &&perfect = cpp_contests::perfect_cache(elements, cache_size_in_bytes, kVirtualWebPageSizeInBenchmark);
  std::cout << "Cache results for binomial_distribution with perfect coin:\n";
  std::cout << "Cache size=" << size_to_string(cache_size_in_bytes)
            << ", page size=" << size_to_string(kVirtualWebPageSizeInBenchmark) << ", keys amplitude=" << (max + 1)
            << ", pool size=" << n_elements << "\n";
  std::cout << "LRU cache hit rate: ";
  std::cout << std::format("{:.2f}%\n", cents * static_cast<double>(lru) / n_elements);
  std::cout << "2Q cache hit rate: ";
  std::cout << std::format("{:.2f}%\n", cents * static_cast<double>(two_queue) / n_elements);
  std::cout << "Perfect cache hit rate: ";
  std::cout << std::format("{:.2f}%\n\n", cents * static_cast<double>(perfect) / n_elements);
  BOOST_TEST(lru <= perfect);
}

BOOST_AUTO_TEST_CASE(PoissonDistributionCacheTest) {
  const double cents = 100.0;
  const std::size_t cache_size_in_bytes = 100 * kVirtualWebPageSizeInBenchmark;
  const std::size_t n_elements = 10000;
  const int lambda = 1000;
  std::mt19937 gen{0}; // NOLINT(cert-msc32-c,cert-msc51-cpp)
  std::poisson_distribution<> d(lambda);
  std::vector<key_type> elements{};
  elements.reserve(n_elements);
  std::generate_n(std::back_inserter(elements), n_elements, [&]() -> int { return d(gen); });
  auto &&lru = cpp_contests::lru_hits(elements, cache_size_in_bytes, kVirtualWebPageSizeInBenchmark);
  auto &&two_queue = cpp_contests::two_queue_hits(elements, cache_size_in_bytes, kVirtualWebPageSizeInBenchmark);
  auto &&perfect = cpp_contests::perfect_cache(elements, cache_size_in_bytes, kVirtualWebPageSizeInBenchmark);
  std::cout << "Cache results for poisson_distribution:\n";
  std::cout << "Cache size=" << size_to_string(cache_size_in_bytes)
            << ", page size=" << size_to_string(kVirtualWebPageSizeInBenchmark) << ", pool size=" << n_elements << "\n";
  std::cout << "LRU cache hit rate: ";
  std::cout << std::format("{:.2f}%\n", cents * static_cast<double>(lru) / n_elements);
  std::cout << "2Q cache hit rate: ";
  std::cout << std::format("{:.2f}%\n", cents * static_cast<double>(two_queue) / n_elements);
  std::cout << "Perfect cache hit rate: ";
  std::cout << std::format("{:.2f}%\n\n", cents * static_cast<double>(perfect) / n_elements);
  BOOST_TEST(lru <= perfect);
}

// NOLINTEND(readability-identifier-length)
