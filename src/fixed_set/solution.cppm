module;

#include <cstddef>
#include <cstdint>
#include <exception>
#include <iostream>
#include <optional>
#include <print>
#include <random>
#include <span>
#include <utility>
#include <vector>

export module fixed_set;

namespace cpp_contests {
namespace {

constexpr uint64_t kP = 2'000'000'011;

class HashFunction {
private:
  uint64_t modulus_;
  uint64_t multiplier_;
  uint64_t increment_;

public:
  // NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
  HashFunction(uint64_t modulus, uint64_t multiplier, uint64_t increment)
      : modulus_(modulus), multiplier_(multiplier), increment_(increment) {}

  auto operator()(int item) const -> std::size_t {
    auto const modulus = static_cast<int64_t>(modulus_);
    auto residue = static_cast<int64_t>(item) % modulus;
    if (residue < 0) {
      residue += modulus;
    }
    return ((multiplier_ * static_cast<uint64_t>(residue)) + increment_) % modulus_;
  }
};

template <typename Generator, typename Predicate>
auto find_hash(Generator generate, Predicate accepts) -> HashFunction {
  for (;;) {
    auto hash = generate();
    if (accepts(hash)) {
      return hash;
    }
  }
}

template <typename Keys, typename Insert>
auto distribute_keys(Keys const &keys, HashFunction const &hash, std::size_t bucket_count, Insert insert) -> bool {
  // Insertion changes the table, so this is not a pure predicate for all_of.
  // NOLINTNEXTLINE(readability-use-anyofallof)
  for (int const key : keys) {
    if (!insert(hash(key) % bucket_count, key)) {
      return false;
    }
  }
  return true;
}

class SecondaryTable {
private:
  std::vector<std::optional<int>> items_;
  HashFunction hash_{kP, 1, 0};

public:
  template <typename Generator> auto initialize(std::span<const int> keys, Generator generate) -> void {
    std::vector<std::optional<int>> items;
    auto size = keys.size() * keys.size();
    auto insert = [&](std::size_t idx, int key) -> bool {
      if (items[idx].has_value()) {
        return false;
      }
      items[idx] = key;
      return true;
    };
    hash_ = find_hash(generate, [&](HashFunction const &hash) -> bool {
      items.clear();
      items.resize(size);
      return distribute_keys(keys, hash, items.size(), insert);
    });
    items_ = std::move(items);
  }

  [[nodiscard]] auto contains(int number) const -> bool {
    if (items_.empty()) {
      return false;
    }
    auto const &item = items_[hash_(number) % items_.size()];
    return item.has_value() && item.value() == number;
  }
};

auto sum_squared_bucket_sizes(std::span<const std::vector<int>> buckets) -> std::size_t {
  std::size_t sum = 0;
  for (auto const &bucket : buckets) {
    auto size = bucket.size();
    sum += size * size;
  }
  return sum;
}

class FixedSet {
private:
  std::vector<SecondaryTable> tables_;
  HashFunction hash_{kP, 1, 0};
  std::mt19937_64 engine_;
  std::uniform_int_distribution<uint32_t> a_dist_ = std::uniform_int_distribution<uint32_t>(1, kP - 1);
  std::uniform_int_distribution<uint32_t> b_dist_ = std::uniform_int_distribution<uint32_t>(0, kP - 1);

  auto random_hash() -> HashFunction { return {kP, a_dist_(engine_), b_dist_(engine_)}; }

  static auto try_initialize(std::vector<int> const &numbers, HashFunction const &hash,
                             std::vector<std::vector<int>> &buckets) -> bool {
    auto n_len = numbers.size();
    buckets.clear();
    buckets.resize(n_len);

    distribute_keys(numbers, hash, buckets.size(), [&](std::size_t idx, int key) -> bool {
      buckets[idx].push_back(key);
      return true;
    });

    return sum_squared_bucket_sizes(buckets) <= 4 * n_len;
  }

public:
  // NOLINTNEXTLINE(bugprone-random-generator-seed, cert-msc32-c, cert-msc51-cpp)
  FixedSet() = default;
  auto initialize(std::vector<int> const &numbers) -> void {
    std::vector<std::vector<int>> buckets;
    auto generate = [&] -> HashFunction { return random_hash(); };
    hash_ =
        find_hash(generate, [&](HashFunction const &hash) -> bool { return try_initialize(numbers, hash, buckets); });
    std::vector<SecondaryTable>{}.swap(tables_);
    tables_.resize(buckets.size());
    for (std::size_t idx = 0; idx < buckets.size(); ++idx) {
      tables_[idx].initialize(buckets[idx], generate);
    }
  }
  [[nodiscard]] auto contains(int number) const -> bool {
    if (tables_.empty()) {
      return false;
    }

    return tables_[hash_(number) % tables_.size()].contains(number);
  }
};

auto read_sequence() -> std::vector<int> {
  std::size_t size = 0;
  std::cin >> size;
  std::vector<int> sequence(size);
  for (auto &current : sequence) {
    std::cin >> current;
  }
  return sequence;
}

auto perform_requests(const std::vector<int> &requests, const FixedSet &set) -> std::vector<bool> {
  std::vector<bool> request_answers;
  request_answers.reserve(requests.size());
  for (auto request : requests) {
    request_answers.push_back(set.contains(request));
  }
  return request_answers;
}

auto print_requests_response(const std::vector<bool> &request_answers) -> void {
  for (auto answer : request_answers) {
    std::println("{}", answer ? "Yes" : "No");
  }
}

auto run([[maybe_unused]] std::span<char *> args) -> void {
  auto numbers = read_sequence();
  auto requests = read_sequence();
  FixedSet set;
  set.initialize(numbers);
  print_requests_response(perform_requests(requests, set));
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
