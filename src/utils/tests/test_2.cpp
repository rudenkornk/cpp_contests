#include <algorithm>
#include <chrono>
#include <cstddef>
#include <stdexcept>
#include <string>
#include <thread>
#include <type_traits>
#include <utility>
#include <vector>

#include <boost/test/unit_test.hpp>

import utils;

// CallableTraits and kIsInstanceOf are pure metaprogramming: static_asserts are the whole test.
namespace {

[[maybe_unused]] auto free_function(int value, std::string const &name) -> double {
  return static_cast<double>(value) + static_cast<double>(name.size());
}

using free_function_traits = cpp_contests::CallableTraits<decltype(free_function)>;
static_assert(free_function_traits::kNArguments == 2);
static_assert(std::is_same_v<free_function_traits::return_type, double>);
static_assert(std::is_same_v<free_function_traits::arg_type<0>, int>);
static_assert(std::is_same_v<free_function_traits::arg_type<1>, std::string const &>);
static_assert(free_function_traits::kIsConst<2>);
static_assert(free_function_traits::kIsLValueReference<2>);

[[maybe_unused]] auto const kIncrement = [](int &value) noexcept -> int { return ++value; };
using lambda_traits = cpp_contests::CallableTraits<decltype(kIncrement)>;
static_assert(lambda_traits::kNArguments == 1);
static_assert(lambda_traits::kIsCallableConst);
static_assert(std::is_same_v<lambda_traits::return_type, int>);
static_assert(lambda_traits::kIsLValueReference<1>);

static_assert(cpp_contests::kIsInstanceOf<std::vector, std::vector<int>>);
static_assert(!cpp_contests::kIsInstanceOf<std::vector, int>);

BOOST_AUTO_TEST_CASE(SaveRestoreRestoresOnScopeExit) {
  int value = 1;
  {
    auto const guard = cpp_contests::SaveRestore{value};
    value = 2;
    BOOST_TEST(value == 2);
  }
  BOOST_TEST(value == 1);
}

BOOST_AUTO_TEST_CASE(SaveRestoreMovedFromIsInert) {
  int value = 1;
  {
    auto inner = cpp_contests::SaveRestore{value};
    auto const outer = std::move(inner);
    value = 2;
    BOOST_TEST(value == 2); // The guard moved into outer; nothing has restored yet.
    // Only outer restores; the moved-from inner must not touch value (nor crash).
  }
  BOOST_TEST(value == 1);
}

BOOST_AUTO_TEST_CASE(SaveRestoreRvalueWithExternalTarget) {
  std::string target = "target";
  {
    auto const guard = cpp_contests::SaveRestore<std::string>{std::string{"saved"}, target};
    target = "changed";
  }
  BOOST_TEST(target == "saved");
}

BOOST_AUTO_TEST_CASE(ExceptionSaverCapturesAndRethrows) {
  auto saver = cpp_contests::ExceptionSaver{};
  auto wrapped = saver.wrap([](int value) -> int {
    if (value < 0) {
      throw std::runtime_error{"negative"};
    }
    return 2 * value;
  });
  BOOST_TEST(wrapped(1) == 2);
  BOOST_TEST(saver.ncaptured() == 0);
  BOOST_TEST(wrapped(-1) == 0); // Value-initialized result on a captured exception.
  BOOST_TEST(saver.ncaptured() == 1);
  BOOST_TEST(saver.nsaved() == 1);
  BOOST_CHECK_THROW(saver.rethrow(), std::runtime_error);
  BOOST_TEST(saver.nsaved() == 0);
}

BOOST_AUTO_TEST_CASE(ExceptionSaverWrapperOutlivesTemporaryCallable) {
  auto saver = cpp_contests::ExceptionSaver{};
  // The lambda passed to wrap() is a temporary: the wrapper must own it, not reference it.
  constexpr std::size_t payload_len = 64;
  auto wrapped =
      saver.wrap([captured = std::string(payload_len, 'x')](std::size_t idx) -> char { return captured.at(idx); });
  BOOST_TEST(wrapped(0) == 'x');
  BOOST_TEST(wrapped(payload_len - 1) == 'x');
  BOOST_TEST(saver.ncaptured() == 0);
}

BOOST_AUTO_TEST_CASE(ExceptionSaverOverflowCountsButSavesUpToMax) {
  auto saver = cpp_contests::ExceptionSaver{1};
  auto wrapped = saver.wrap([]() -> void { throw std::runtime_error{"boom"}; });
  wrapped();
  wrapped();
  BOOST_TEST(saver.ncaptured() == 2);
  BOOST_TEST(saver.nsaved() == 1);
  saver.drop();
  BOOST_TEST(saver.nsaved() == 0);
}

BOOST_AUTO_TEST_CASE(ExceptionSaverMultithreadedCapture) {
  constexpr std::size_t n_threads = 8;
  auto saver = cpp_contests::ExceptionSaver{n_threads};
  {
    auto threads = std::vector<std::jthread>{};
    threads.reserve(n_threads);
    auto wrapped = saver.wrap([]() -> void { throw std::runtime_error{"boom"}; });
    for (std::size_t i = 0; i != n_threads; ++i) {
      threads.emplace_back(wrapped);
    }
  }
  BOOST_TEST(saver.ncaptured() == n_threads);
  BOOST_TEST(saver.nsaved() == n_threads);
  saver.drop();
}

BOOST_AUTO_TEST_CASE(SortPermutationRoundtrip) {
  // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
  auto values = std::vector<int>{30, 10, 20, 50, 40};
  auto const expected = std::vector<std::size_t>{1, 2, 0, 4, 3};
  auto permutation = cpp_contests::get_sort_permutation(values);
  BOOST_TEST(permutation == expected);
  cpp_contests::permute(values, permutation);
  BOOST_TEST(std::ranges::is_sorted(values));
}

BOOST_AUTO_TEST_CASE(PermuteIdentityAndReverse) {
  auto values = std::vector<int>{1, 2, 3, 4};
  auto identity = std::vector<std::size_t>{0, 1, 2, 3};
  cpp_contests::permute(values, identity);
  BOOST_TEST(values == (std::vector<int>{1, 2, 3, 4}));

  auto reverse = std::vector<std::size_t>{3, 2, 1, 0};
  cpp_contests::permute(values, reverse);
  BOOST_TEST(values == (std::vector<int>{4, 3, 2, 1}));
}

BOOST_AUTO_TEST_CASE(PermuteWithIndexProjection) {
  auto values = std::vector<std::string>{"c", "a", "b"};
  auto permutation = std::vector<std::pair<std::size_t, char>>{{1, 'x'}, {2, 'y'}, {0, 'z'}};
  cpp_contests::permute(values, permutation, [](auto const &pair) noexcept -> std::size_t { return pair.first; });
  BOOST_TEST(values == (std::vector<std::string>{"a", "b", "c"}));
}

BOOST_AUTO_TEST_CASE(BenchmarkRunsCallableNTimes) {
  std::size_t calls = 0;
  auto const duration = cpp_contests::benchmark<10>(
      [&calls](int increment_by) -> void { calls += static_cast<std::size_t>(increment_by); }, 1);
  BOOST_TEST(calls == 10);
  BOOST_TEST((duration >= std::chrono::nanoseconds{0}));
}

BOOST_AUTO_TEST_CASE(GetIndicesIsIota) {
  auto const indices = cpp_contests::get_indices<4>();
  BOOST_TEST(indices.size() == 4);
  BOOST_TEST(indices.front() == 0);
  BOOST_TEST(indices.back() == 3);
}
} // namespace
