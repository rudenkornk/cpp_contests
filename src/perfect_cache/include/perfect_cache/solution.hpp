#pragma once

#include <vector>

#include "perfect_cache/perfect_cache_export.h"

namespace cpp_contests {

PERFECT_CACHE_EXPORT std::size_t perfect_cache(std::vector<int> const &keys,
                                               std::size_t max_size_in_bytes,
                                               std::size_t value_size_in_bytes);

} // namespace cpp_contests
