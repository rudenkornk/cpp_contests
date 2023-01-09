#pragma once

#include <cmath>
#include <type_traits>

namespace cpp_contests {

namespace details_ {
// See https://gist.github.com/alexshtf/eb5128b3e3e143187794
double constexpr sqrt_newton(double x, double curr, double prev) {
  if (curr == prev)
    // Converged
    return curr;
  // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
  return sqrt_newton(x, 0.5 * (curr + x / curr), curr);
}
} // namespace details_

double constexpr sqrt(double x) {
  if (!std::is_constant_evaluated())
    // Pretty sure I cannot beat the compiler here.
    return std::sqrt(x);

  if (x < 0)
    return std::numeric_limits<double>::quiet_NaN();
  if (x > std::numeric_limits<double>::infinity())
    return std::numeric_limits<double>::infinity();

  return details_::sqrt_newton(x, x, 0);
}

} // namespace cpp_contests
