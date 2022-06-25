#pragma once

#include <cassert>
#include <concepts>
#include <functional>
#include <list>
#include <string>
#include <unordered_map>

#include <iostream>

namespace cpp_contests {

template <typename Key, typename Value, std::invocable<Key> Load>
class LRUCache final {
  struct ValuePos {
    Value value;
    typename std::list<Key>::const_iterator pos;
  };
  Load load_;
  std::size_t max_size_;
  std::list<Key> keys_;
  std::unordered_map<Key, ValuePos> data_;

public:
  LRUCache(Load const &load, std::size_t max_size)
      : load_(load), max_size_(max_size) {}
  // Forbid all copy and moves, since storing references inside class fields
  // requires special handling, not supported in these methods by default
  LRUCache(LRUCache const &) = delete;
  LRUCache(LRUCache &&) = delete;
  LRUCache &operator=(LRUCache const &) = delete;
  LRUCache &operator=(LRUCache &&) = delete;
  ~LRUCache() = default;

  Value get(Key const &key) {
    if (max_size_ == 0)
      return load_(key);

    if (data_.contains(key)) {
      keys_.splice(data_.at(key).pos, keys_, std::next(data_.at(key).pos),
                   keys_.end());
      return data_.at(key).value;
    }

    Value value = std::invoke(load_, key);
    keys_.push_back(key);
    data_.emplace(key, ValuePos{std::move(value), std::prev(keys_.end())});
    if (keys_.size() > max_size_) {
      assert(keys_.size() == max_size_ + 1);
      data_.erase(keys_.front());
      keys_.pop_front();
    }
    return data_.at(key).value;
  }
};

inline std::size_t lru_hits(std::size_t max_size, std::vector<int> elements) {
  std::size_t misses{0};
  auto load = [&misses](int) -> int {
    ++misses;
    return 0;
  };
  cpp_contests::LRUCache<int, int, decltype(load)> cache{load, max_size};
  for (auto &&e : elements)
    cache.get(e);
  return elements.size() - misses;
}

} // namespace cpp_contests
