#pragma once

#include <cassert>
#include <functional>
#include <list>
#include <queue>
#include <unordered_map>
#include <unordered_set>

namespace cpp_contests {

template <typename Key, typename Value, std::invocable<Key> Load>
class TwoQueueCache final {
  using ColdList = std::queue<Key>;
  using HotList = std::list<Key>;
  using HotPosition = typename HotList::const_iterator;

  static constexpr double cold_in_ratio_ = 0.25;
  static constexpr double cold_out_ratio_ = 0.5;
  static constexpr double hot_ratio_ = 1 - cold_in_ratio_;

  Load load_;
  std::size_t max_size_;
  static constexpr std::size_t key_size_ = sizeof(Key);
  std::size_t value_size_;
  std::size_t max_length_;
  std::size_t cold_in_max_length_;
  std::size_t cold_out_max_length_;
  std::size_t hot_max_length_;

  std::unordered_map<Key, Value> data_;
  std::unordered_map<Key, HotPosition> hot_positions_;
  std::unordered_set<Key> cold_out_set_;
  ColdList cold_in_;
  ColdList cold_out_;
  HotList hot_;

public:
  // NOLINTNEXTLINE
  TwoQueueCache(Load const &load, std::size_t max_size_in_bytes,
                std::size_t override_value_size = sizeof(Value))
      : load_(load), max_size_(max_size_in_bytes),
        value_size_(override_value_size),
        max_length_(static_cast<std::size_t>(
            max_size_ / (value_size_ + key_size_ * (1 + cold_out_ratio_)))),
        cold_in_max_length_(
            static_cast<std::size_t>(max_length_ * cold_in_ratio_)),
        cold_out_max_length_(
            static_cast<std::size_t>(max_length_ * cold_out_ratio_)),
        hot_max_length_(max_length_ - cold_in_max_length_) {}

  // Forbid all copies and moves, since storing references inside class fields
  // requires special handling, not supported in these methods by default
  TwoQueueCache(TwoQueueCache const &) = delete;
  TwoQueueCache(TwoQueueCache &&) = delete;
  TwoQueueCache &operator=(TwoQueueCache const &) = delete;
  TwoQueueCache &operator=(TwoQueueCache &&) = delete;
  ~TwoQueueCache() = default;

  Value get(Key const &key) {
    if (hot_positions_.contains(key)) {
      touch_hot(key);
      return data_.at(key);
    }

    // key in cold_in_ bucket
    if (data_.contains(key))
      return data_.at(key);

    Value value = load_and_allocate(key);
    if (cold_out_set_.contains(key))
      push_hot(key);
    else
      push_cold(key);

    verify();
    return value;
  }

private:
  void touch_hot(Key const &key) {
    auto &&pos = hot_positions_.at(key);
    hot_.splice(pos, hot_, std::next(pos), hot_.end());
  }
  Value load_and_allocate(Key const &key) {
    assert(!data_.contains(key));
    Value value = std::invoke(load_, key);
    data_.emplace(key, value);
    return value;
  }
  void push_hot(Key const &key) {
    hot_.push_back(key);
    if (hot_.size() > hot_max_length_) {
      assert(hot_.size() == hot_max_length_ + 1);
      data_.erase(hot_.front());
      hot_positions_.erase(hot_.front());
      hot_.pop_front();
    }
  }
  void push_cold(Key const &key) {
    cold_in_.push(key);
    if (cold_in_.size() > cold_in_max_length_) {
      assert(cold_in_.size() == cold_in_max_length_ + 1);
      data_.erase(cold_in_.front());
      push_cold_out(cold_in_.front());
      cold_in_.pop();
    }
  }
  void push_cold_out(Key const &key) {
    cold_out_.push(key);
    cold_out_set_.insert(key);
    if (cold_out_.size() > cold_out_max_length_) {
      assert(cold_out_.size() == cold_out_max_length_ + 1);
      cold_out_set_.erase(cold_out_.front());
      cold_out_.pop();
    }
  }
  void verify() const noexcept {
    assert(data_.size() <= max_length_);
    assert(hot_.size() <= hot_max_length_);
    assert(cold_in_.size() <= cold_in_max_length_);
    assert(cold_out_.size() <= cold_out_max_length_);
    assert(hot_.size() + cold_in_.size() == data_.size());
  }
};

inline std::size_t two_queue_hits(std::vector<int> const &elements,
                                  std::size_t max_size_in_bytes,
                                  std::size_t value_size) {
  std::size_t misses{0};
  auto load = [&misses](int) -> int {
    ++misses;
    return 0;
  };
  cpp_contests::TwoQueueCache<int, int, decltype(load)> cache{
      load, max_size_in_bytes, value_size};
  for (auto &&e : elements)
    cache.get(e);
  return elements.size() - misses;
}

} // namespace cpp_contests
