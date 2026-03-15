module;

#include <cmath>
#include <limits>
#include <type_traits>

export module utils.math;

export namespace cpp_contests {

namespace details_ {
// See https://gist.github.com/alexshtf/eb5128b3e3e143187794
// NOLINTNEXTLINE(misc-no-recursion)
auto constexpr sqrt_newton(double val, double curr, double prev) -> double {
  if (curr == prev) {
    // Converged
    return curr;
  }
  // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
  return sqrt_newton(val, 0.5 * (curr + val / curr), curr);
}
} // namespace details_

auto constexpr sqrt(double val) -> double {
  if (!std::is_constant_evaluated()) {
    // Pretty sure I cannot beat the compiler here.
    return std::sqrt(val);
  }

  if (val < 0) {
    return std::numeric_limits<double>::quiet_NaN();
  }
  if (val > std::numeric_limits<double>::infinity()) {
    return std::numeric_limits<double>::infinity();
  }

  return details_::sqrt_newton(val, val, 0);
}

} // namespace cpp_contests
