// NOLINTBEGIN(cppcoreguidelines-pro-type-vararg)
// NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)

#define BOOST_TEST_MODULE Test  // NOLINT
#define _CRT_SECURE_NO_WARNINGS // NOLINT

#include <algorithm>
#include <cassert>
#include <iterator>
#include <random>
#include <vector>

#include <boost/test/included/unit_test.hpp>
#include <fmt/core.h>

#include "lru_cache/solution.hpp"
#include "perfect_cache/solution.hpp"
#include "utils/utils.hpp"

using namespace cpp_contests;

using Key = int;
static constexpr std::size_t typical_web_page_size_in_bytes =
    std::size_t{2} * 1024 * 1024;
static constexpr std::size_t typical_db_key_size_in_bytes = 32;
static constexpr std::size_t actual_key_size_in_benchmark = sizeof(Key);
static constexpr std::size_t virtual_web_page_size_in_benchmark =
    typical_web_page_size_in_bytes / typical_db_key_size_in_bytes *
    actual_key_size_in_benchmark;

BOOST_AUTO_TEST_CASE(uniform_distribution_cache_test) {
  const double cents = 100.0;
  const std::size_t cache_size_in_bytes =
      100 * virtual_web_page_size_in_benchmark;
  const std::size_t n_elements = 10000;
  const int min = 1;
  const int max = 300;
  std::mt19937 gen{0};
  std::uniform_int_distribution<> d{min, max};
  std::vector<Key> elements{};
  elements.reserve(n_elements);
  std::generate_n(std::back_inserter(elements), n_elements,
                  [&]() { return d(gen); });
  auto &&lru = cpp_contests::lru_hits(elements, cache_size_in_bytes,
                                      virtual_web_page_size_in_benchmark);
  auto &&perfect = cpp_contests::perfect_cache(
      elements, cache_size_in_bytes, virtual_web_page_size_in_benchmark);
  std::cout << "Cache results for uniform_int_distribution:\n";
  std::cout << "Cache size=" << size_to_string(cache_size_in_bytes)
            << ", page size="
            << size_to_string(virtual_web_page_size_in_benchmark)
            << ", keys amplitude=" << (max - min + 1)
            << ", pool size=" << n_elements << "\n";
  std::cout << "LRU cache hit rate: ";
  fmt::print("{:.2f}%\n", cents * double(lru) / n_elements); // TODO std::format
  std::cout << "Perfect cache hit rate: ";
  fmt::print("{:.2f}%\n\n",
             cents * double(perfect) / n_elements); // TODO std::format
  BOOST_TEST(lru <= perfect);
}

BOOST_AUTO_TEST_CASE(binomial_distribution_cache_test) {
  const double cents = 100.0;
  const std::size_t cache_size_in_bytes =
      100 * virtual_web_page_size_in_benchmark;
  const std::size_t n_elements = 10000;
  const int max = 299;
  std::mt19937 gen{0};
  std::binomial_distribution<> d(max);
  std::vector<Key> elements{};
  elements.reserve(n_elements);
  std::generate_n(std::back_inserter(elements), n_elements,
                  [&]() { return d(gen); });
  auto &&lru = cpp_contests::lru_hits(elements, cache_size_in_bytes,
                                      virtual_web_page_size_in_benchmark);
  auto &&perfect = cpp_contests::perfect_cache(
      elements, cache_size_in_bytes, virtual_web_page_size_in_benchmark);
  std::cout << "Cache results for binomial_distribution with perfect coin:\n";
  std::cout << "Cache size=" << size_to_string(cache_size_in_bytes)
            << ", page size="
            << size_to_string(virtual_web_page_size_in_benchmark)
            << ", keys amplitude=" << (max + 1)
            << ", pool size=" << n_elements << "\n";
  std::cout << "LRU cache hit rate: ";
  fmt::print("{:.2f}%\n", cents * double(lru) / n_elements); // TODO std::format
  std::cout << "Perfect cache hit rate: ";
  fmt::print("{:.2f}%\n\n",
             cents * double(perfect) / n_elements); // TODO std::format
  BOOST_TEST(lru <= perfect);
}

BOOST_AUTO_TEST_CASE(poisson_distribution_cache_test) {
  const double cents = 100.0;
  const std::size_t cache_size_in_bytes =
      100 * virtual_web_page_size_in_benchmark;
  const std::size_t n_elements = 10000;
  const int lambda = 1000;
  std::mt19937 gen{0};
  std::poisson_distribution<> d(lambda);
  std::vector<Key> elements{};
  elements.reserve(n_elements);
  std::generate_n(std::back_inserter(elements), n_elements,
                  [&]() { return d(gen); });
  auto &&lru = cpp_contests::lru_hits(elements, cache_size_in_bytes,
                                      virtual_web_page_size_in_benchmark);
  auto &&perfect = cpp_contests::perfect_cache(
      elements, cache_size_in_bytes, virtual_web_page_size_in_benchmark);
  std::cout << "Cache results for poisson_distribution:\n";
  std::cout << "Cache size=" << size_to_string(cache_size_in_bytes)
            << ", page size="
            << size_to_string(virtual_web_page_size_in_benchmark)
            << ", pool size=" << n_elements << "\n";
  std::cout << "LRU cache hit rate: ";
  fmt::print("{:.2f}%\n", cents * double(lru) / n_elements); // TODO std::format
  std::cout << "Perfect cache hit rate: ";
  fmt::print("{:.2f}%\n\n",
             cents * double(perfect) / n_elements); // TODO std::format
  BOOST_TEST(lru <= perfect);
}

// NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)
// NOLINTEND(cppcoreguidelines-pro-type-vararg)
