#pragma once

#include <cstddef>
#include <vector>

#include "k_max/k_max_export.h"

namespace cpp_contests {

K_MAX_EXPORT auto k_max(std::vector<int> const &nums,
                        // NOLINTNEXTLINE(readability-identifier-length)
                        std::size_t k) -> std::vector<int>;

} // namespace cpp_contests
