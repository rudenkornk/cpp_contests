#include "perfect_cache/solution.hpp"

#include <cassert>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace cpp_contests {

using Key = int;

struct KeyNextpos {
  Key key;
  std::size_t nextpos;
};
struct KeyNextposCompare {
  constexpr bool operator()(KeyNextpos const &lhs,
                            KeyNextpos const &rhs) const noexcept {
    return std::tie(lhs.nextpos, lhs.key) > std::tie(rhs.nextpos, rhs.key);
  }
};

std::size_t perfect_cache(std::vector<Key> const &keys,
                          std::size_t max_size_in_bytes,
                          std::size_t value_size_in_bytes) {
  std::size_t const max_length =
      max_size_in_bytes / (value_size_in_bytes + sizeof(Key));
  if (max_length == 0)
    return 0;

  std::unordered_map<Key, std::vector<std::size_t>> positions{};
  for (std::size_t i = 0, e = keys.size(); i != e; ++i) {
    auto index = keys.size() - i - 1;
    auto &&key = keys[index];
    if (positions.contains(key))
      positions.at(key).push_back(index);
    else
      positions.emplace(key, std::vector<std::size_t>{index});
  }

  std::set<KeyNextpos, KeyNextposCompare> fartherst_keys{};

  std::unordered_map<Key, decltype(fartherst_keys)::const_iterator> cache{};
  cache.reserve(max_length);
  std::size_t hits{0};
  for (auto &&key : keys) {
    positions.at(key).pop_back();
    if (!cache.contains(key)) {
      std::size_t nextpos = keys.size();
      if (!positions.at(key).empty())
        nextpos = positions.at(key).back();
      auto &&res = fartherst_keys.insert({key, nextpos});
      assert(res.second);
      cache.emplace(key, res.first);
      if (cache.size() > max_length) {
        assert(cache.size() == max_length + 1);
        auto erase_key = (*fartherst_keys.begin()).key;
        fartherst_keys.erase(fartherst_keys.begin());
        cache.erase(erase_key);
      }
    } else {
      ++hits;
      fartherst_keys.erase(cache.at(key));
      std::size_t nextpos = keys.size();
      if (!positions.at(key).empty())
        nextpos = positions.at(key).back();
      auto &&res = fartherst_keys.insert({key, nextpos});
      assert(res.second);
      cache.at(key) = res.first;
    }
  }
  return hits;
}

} // namespace cpp_contests
