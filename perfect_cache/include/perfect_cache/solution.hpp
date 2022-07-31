#pragma once

#include <vector>

namespace cpp_contests {

std::size_t perfect_cache(std::vector<int> const &keys,
                          std::size_t max_size_in_bytes,
                          std::size_t value_size_in_bytes);

} // namespace cpp_contests
