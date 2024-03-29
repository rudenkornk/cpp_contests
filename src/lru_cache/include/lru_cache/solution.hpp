#pragma once

#include <cassert>
#include <functional>
#include <list>
#include <string>
#include <unordered_map>

namespace cpp_contests {

template <typename Key, typename Value, std::invocable<Key> Load>
class LRUCache final {
  using Position = typename std::list<Key>::const_iterator;
  Load load_;
  std::size_t max_length_;
  std::list<Key> keys_;
  std::unordered_map<Key, Value> data_;
  std::unordered_map<Key, Position> key_positions_;

public:
  // NOLINTNEXTLINE
  LRUCache(Load const &load, std::size_t max_size_in_bytes,
           std::size_t override_value_size = sizeof(Value))
      : load_(load),
        max_length_(max_size_in_bytes / (override_value_size + sizeof(Key))) {}

  // Forbid all copy and moves, since storing references inside class fields
  // requires special handling, not supported in these methods by default
  LRUCache(LRUCache const &) = delete;
  LRUCache(LRUCache &&) = delete;
  LRUCache &operator=(LRUCache const &) = delete;
  LRUCache &operator=(LRUCache &&) = delete;
  ~LRUCache() = default;

  Value lookup_update(Key const &key) {
    if (key_positions_.contains(key)) {
      keys_.splice(key_positions_.at(key), keys_,
                   std::next(key_positions_.at(key)), keys_.end());
      return data_.at(key);
    }

    Value value = std::invoke(load_, key);
    keys_.push_back(key);
    data_.emplace(key, value);
    key_positions_.emplace(key, std::prev(keys_.end()));
    if (keys_.size() > max_length_) {
      assert(keys_.size() == max_length_ + 1);
      data_.erase(keys_.front());
      key_positions_.erase(keys_.front());
      keys_.pop_front();
    }
    return value;
  }
};

inline std::size_t lru_hits(std::vector<int> const &elements,
                            std::size_t max_size_in_bytes,
                            std::size_t value_size_in_bytes) {
  std::size_t misses{0};
  auto load = [&misses](int) -> int {
    ++misses;
    return 0;
  };
  cpp_contests::LRUCache<int, int, decltype(load)> cache{
      load, max_size_in_bytes, value_size_in_bytes};
  for (auto &&e : elements)
    cache.lookup_update(e);
  return elements.size() - misses;
}

} // namespace cpp_contests
