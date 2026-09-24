module;

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <functional>
#include <iostream>
#include <ranges>
#include <span>
#include <vector>

export module min_max_arrays;

namespace cpp_contests {
namespace {

auto solve(std::span<int32_t const> a_inc, std::span<int32_t const> b_dec) -> std::size_t {
  auto diff = std::views::zip_transform(std::minus<int>{}, a_inc, b_dec);
  std::size_t idx = std::ranges::lower_bound(diff, 0) - diff.begin();
  if (idx == 0) {
    return 0;
  }
  if (idx == a_inc.size()) {
    return idx - 1;
  }
  if (a_inc[idx] >= b_dec[idx - 1]) {
    return idx - 1;
  }
  return idx;
}

auto run([[maybe_unused]] std::span<char *> args) -> void {
  std::size_t n_len = 0;
  std::size_t m_len = 0;
  std::size_t len = 0;
  std::size_t q_queries = 0;

  std::cin >> n_len >> m_len >> len;

  std::vector<int32_t> a_inc(n_len * len);
  std::vector<int32_t> b_dec(m_len * len);

  for (size_t i = 0; i < n_len * len; ++i) {
    std::cin >> a_inc[i];
  }
  for (size_t i = 0; i < m_len * len; ++i) {
    std::cin >> b_dec[i];
  }
  std::cin >> q_queries;
  std::vector<size_t> its(q_queries);
  std::vector<size_t> jts(q_queries);

  for (std::size_t qt = 0; qt < q_queries; ++qt) {
    std::cin >> its[qt] >> jts[qt];
  }
  std::vector<size_t> results(q_queries);

  for (std::size_t qt = 0; qt < q_queries; ++qt) {
    auto res = solve(std::span{&a_inc[(its[qt] - 1) * len], len}, std::span{&b_dec[(jts[qt] - 1) * len], len}) + 1;
    results[qt] = res;
  }
  for (std::size_t qt = 0; qt < q_queries; ++qt) {
    std::cout << results[qt] << " ";
  }
}

} // namespace
} // namespace cpp_contests

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
extern "C++" auto main(int argc, char **argv) -> int {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  try {
    cpp_contests::run(std::span(argv, static_cast<std::size_t>(argc)));
  } catch (std::exception const &e) {
    std::cerr << e.what() << "\n";
    return 1;
  }
}
#pragma GCC diagnostic pop
