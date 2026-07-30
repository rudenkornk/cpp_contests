export module k_max;

import std;

export namespace cpp_contests {

auto k_max(std::vector<int> const &nums,
           // NOLINTNEXTLINE(readability-identifier-length)
           std::size_t k) -> std::vector<int> {
  // O(n) memory, O(n * log(k)) time
  std::vector<int> result{};
  result.reserve(nums.size() + 1 - k);
  std::multiset<int> window{};
  for (std::size_t i = 0; i != k; ++i) {
    window.insert(nums[i]);
  }

  result.push_back(*window.rbegin());
  for (std::size_t i = 1; i != nums.size() + 1 - k; ++i) {
    window.erase(window.find(nums[i - 1]));
    window.insert(nums[k + i - 1]);
    result.push_back(*window.rbegin());
  }
  return result;
}

} // namespace cpp_contests
