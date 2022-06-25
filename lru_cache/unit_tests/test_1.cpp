// NOLINTBEGIN(cppcoreguidelines-pro-type-vararg)
// NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)

#define BOOST_TEST_MODULE Test  // NOLINT
#define _CRT_SECURE_NO_WARNINGS // NOLINT

#include <algorithm>
#include <cassert>
#include <cstdio>
#include <iterator>
#include <random>
#include <vector>

#include <boost/test/included/unit_test.hpp>

#include "lru_cache/solution.hpp"
#include "perfect_cache/solution.hpp"

using namespace cpp_contests;

BOOST_AUTO_TEST_CASE(uniform_distribution_cache_test) {
  const double cents = 100.0;
  const std::size_t cache_size = 100;
  const std::size_t n_elements = 10000;
  const int min = 1;
  const int max = 300;
  std::mt19937 gen{0};
  std::uniform_int_distribution<> d{min, max};
  std::vector<int> elements{};
  elements.reserve(n_elements);
  std::generate_n(std::back_inserter(elements), n_elements,
                  [&]() { return d(gen); });
  auto &&lru = cpp_contests::lru_hits(cache_size, elements);
  auto &&perfect = cpp_contests::perfect_cache(cache_size, elements);
  std::cout << "Cache results for uniform_int_distribution:\n";
  std::cout << "Cache size=" << cache_size
            << ", values amplitude=" << (max - min + 1)
            << ", pool size=" << n_elements << "\n";
  std::cout << "LRU cache hit rate: ";
  std::printf("%.2f%%\n", cents * double(lru) / n_elements); // TODO std::format
  std::cout << "Perfect cache hit rate: ";
  std::printf("%.2f%%\n\n",
              cents * double(perfect) / n_elements); // TODO std::format
  BOOST_TEST(lru <= perfect);
}

BOOST_AUTO_TEST_CASE(binomial_distribution_cache_test) {
  const double cents = 100.0;
  const std::size_t cache_size = 100;
  const std::size_t n_elements = 10000;
  const int max = 299;
  std::mt19937 gen{0};
  std::binomial_distribution<> d(max);
  std::vector<int> elements{};
  elements.reserve(n_elements);
  std::generate_n(std::back_inserter(elements), n_elements,
                  [&]() { return d(gen); });
  auto &&lru = cpp_contests::lru_hits(cache_size, elements);
  auto &&perfect = cpp_contests::perfect_cache(cache_size, elements);
  std::cout << "Cache results for binomial_distribution with perfect coin:\n";
  std::cout << "Cache size=" << cache_size << ", values amplitude=" << (max + 1)
            << ", pool size=" << n_elements << "\n";
  std::cout << "LRU cache hit rate: ";
  std::printf("%.2f%%\n", cents * double(lru) / n_elements); // TODO std::format
  std::cout << "Perfect cache hit rate: ";
  std::printf("%.2f%%\n\n",
              cents * double(perfect) / n_elements); // TODO std::format
  BOOST_TEST(lru <= perfect);
}

BOOST_AUTO_TEST_CASE(poisson_distribution_cache_test) {
  const double cents = 100.0;
  const std::size_t cache_size = 100;
  const std::size_t n_elements = 10000;
  const int lambda = 1000;
  std::mt19937 gen{0};
  std::poisson_distribution<> d(lambda);
  std::vector<int> elements{};
  elements.reserve(n_elements);
  std::generate_n(std::back_inserter(elements), n_elements,
                  [&]() { return d(gen); });
  auto &&lru = cpp_contests::lru_hits(cache_size, elements);
  auto &&perfect = cpp_contests::perfect_cache(cache_size, elements);
  std::cout << "Cache results for poisson_distribution:\n";
  std::cout << "Cache size=" << cache_size << ", lambda=" << lambda
            << ", pool size=" << n_elements << "\n";
  std::cout << "LRU cache hit rate: ";
  std::printf("%.2f%%\n", cents * double(lru) / n_elements); // TODO std::format
  std::cout << "Perfect cache hit rate: ";
  std::printf("%.2f%%\n\n",
              cents * double(perfect) / n_elements); // TODO std::format
  BOOST_TEST(lru <= perfect);
}

// NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)
// NOLINTEND(cppcoreguidelines-pro-type-vararg)
