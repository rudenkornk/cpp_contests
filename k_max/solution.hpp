#include <cassert>
#include <numeric>
#include <set>
#include <vector>

std::vector<int> inline k_max(std::vector<int> const &nums, size_t k) {
  // O(n) memory, O(n * log(k)) time
  std::vector<int> result{};
  result.reserve(nums.size() + 1 - k);
  std::multiset<int> window{};
  for (size_t i = 0; i != k; ++i)
    window.insert(nums[i]);

  result.push_back(*window.rbegin());
  for (size_t i = 1; i != nums.size() + 1 - k; ++i) {
    window.erase(window.find(nums[i - 1]));
    window.insert(nums[k + i - 1]);
    result.push_back(*window.rbegin());
  }
  return result;
}
