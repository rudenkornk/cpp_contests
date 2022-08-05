#pragma once

#include <array>
#include <cassert>
#include <climits>
#include <fmt/core.h>
#include <numeric>
#include <string>
#include <string_view>

namespace cpp_contests {

std::string size_to_string(size_t size) {
  // NOLINTNEXTLINE
  constexpr std::array<std::string_view, 8> units = {
      "B", "KiB", "MiB", "GiB", "TiB", "PiB", "EiB", "ZiB"};
  constexpr std::size_t base = 1024;
  if (size == 0) {
    return "0 B";
  }

  std::size_t tier = 0;
  double result = 0;
  bool is_exact = true;
  while (size != 0) {
    std::size_t residual = size % base;
    result = static_cast<double>(residual) + result / base;
    size /= base;
    if (residual > 0 && size > 0)
      is_exact = false;
    ++tier;
  }
  --tier;
  if (is_exact)
    return fmt::format("{} {}", result, units[tier]); // TODO std::format
  else
    return fmt::format("{:.1f} {}", result, units[tier]); // TODO std::format
}

} // namespace cpp_contests
