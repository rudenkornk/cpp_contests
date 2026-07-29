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
