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

// CallableTraits and isInstanceOf are pure metaprogramming: static_asserts are the whole test.
namespace {

[[maybe_unused]] auto free_function(int value, std::string const &name) -> double {
  return static_cast<double>(value) + static_cast<double>(name.size());
}

using FreeFunctionTraits = cpp_contests::CallableTraits<decltype(free_function)>;
static_assert(FreeFunctionTraits::nArguments == 2);
static_assert(std::is_same_v<FreeFunctionTraits::ReturnType, double>);
static_assert(std::is_same_v<FreeFunctionTraits::ArgType<0>, int>);
static_assert(std::is_same_v<FreeFunctionTraits::ArgType<1>, std::string const &>);
static_assert(FreeFunctionTraits::isConst<2>);
static_assert(FreeFunctionTraits::isLValueReference<2>);

[[maybe_unused]] auto const increment = [](int &value) noexcept -> int { return ++value; };
using LambdaTraits = cpp_contests::CallableTraits<decltype(increment)>;
static_assert(LambdaTraits::nArguments == 1);
static_assert(LambdaTraits::isCallableConst);
static_assert(std::is_same_v<LambdaTraits::ReturnType, int>);
static_assert(LambdaTraits::isLValueReference<1>);

static_assert(cpp_contests::isInstanceOf<std::vector, std::vector<int>>);
static_assert(!cpp_contests::isInstanceOf<std::vector, int>);

} // namespace

BOOST_AUTO_TEST_CASE(save_restore_restores_on_scope_exit) {
  int value = 1;
  {
    auto const guard = cpp_contests::SaveRestore{value};
    value = 2;
    BOOST_TEST(value == 2);
  }
  BOOST_TEST(value == 1);
}

BOOST_AUTO_TEST_CASE(save_restore_moved_from_is_inert) {
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

BOOST_AUTO_TEST_CASE(save_restore_rvalue_with_external_target) {
  std::string target = "target";
  {
    auto const guard = cpp_contests::SaveRestore<std::string>{std::string{"saved"}, target};
    target = "changed";
  }
  BOOST_TEST(target == "saved");
}

BOOST_AUTO_TEST_CASE(exception_saver_captures_and_rethrows) {
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

BOOST_AUTO_TEST_CASE(exception_saver_wrapper_outlives_temporary_callable) {
  auto saver = cpp_contests::ExceptionSaver{};
  // The lambda passed to wrap() is a temporary: the wrapper must own it, not reference it.
  constexpr std::size_t payload_len = 64;
  auto wrapped =
      saver.wrap([captured = std::string(payload_len, 'x')](std::size_t idx) -> char { return captured.at(idx); });
  BOOST_TEST(wrapped(0) == 'x');
  BOOST_TEST(wrapped(payload_len - 1) == 'x');
  BOOST_TEST(saver.ncaptured() == 0);
}

BOOST_AUTO_TEST_CASE(exception_saver_overflow_counts_but_saves_up_to_max) {
  auto saver = cpp_contests::ExceptionSaver{1};
  auto wrapped = saver.wrap([]() -> void { throw std::runtime_error{"boom"}; });
  wrapped();
  wrapped();
  BOOST_TEST(saver.ncaptured() == 2);
  BOOST_TEST(saver.nsaved() == 1);
  saver.drop();
  BOOST_TEST(saver.nsaved() == 0);
}

BOOST_AUTO_TEST_CASE(exception_saver_multithreaded_capture) {
  constexpr std::size_t n_threads = 8;
  auto saver = cpp_contests::ExceptionSaver{n_threads};
  {
    auto threads = std::vector<std::jthread>{};
    threads.reserve(n_threads);
    for (std::size_t i = 0; i != n_threads; ++i) {
      threads.emplace_back(saver.wrap([]() -> void { throw std::runtime_error{"boom"}; }));
    }
  }
  BOOST_TEST(saver.ncaptured() == n_threads);
  BOOST_TEST(saver.nsaved() == n_threads);
  saver.drop();
}
