module;

#include <cassert>
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
public:
  // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
  uint64_t kp = kP;
  uint64_t a = 1;
  uint64_t b = 0;
  uint64_t n = 0;
  // NOLINTEND(misc-non-private-member-variables-in-classes)

  HashFunction() = default;
  auto operator()(int item) const -> std::size_t {
    constexpr int64_t offset = 1'000'000'000;
    return ((a * (static_cast<int64_t>(item) + offset)) + b) % kp % n;
  }
};

struct Bucket {
  std::vector<std::optional<int>> items;
  HashFunction hash;
};

class FixedSet {
private:
  std::vector<Bucket> buckets_;
  HashFunction hash_;
  std::mt19937_64 engine_;
  std::uniform_int_distribution<uint32_t> a_dist_ = std::uniform_int_distribution<uint32_t>(1, kP - 1);
  std::uniform_int_distribution<uint32_t> b_dist_ = std::uniform_int_distribution<uint32_t>(0, kP - 1);

  auto try_initialize(std::vector<int> const &numbers) -> bool {
    auto n_len = numbers.size();
    auto ac = a_dist_(engine_);
    auto bc = b_dist_(engine_);

    hash_.n = n_len;
    hash_.a = ac;
    hash_.b = bc;
    buckets_.clear();
    buckets_.resize(n_len);

    for (auto number : numbers) {
      buckets_[hash_(number)].items.emplace_back(number);
    }

    std::size_t count = 0;
    for (auto &&bucket : buckets_) {
      count += bucket.items.size() * bucket.items.size();
    }

    return count <= 4 * n_len;
  }

  auto try_initialize_bucket(Bucket &bucket) -> bool {
    auto n_len = bucket.items.size();
    auto ac = a_dist_(engine_);
    auto bc = b_dist_(engine_);

    bucket.hash.n = n_len * n_len;
    bucket.hash.a = ac;
    bucket.hash.b = bc;

    auto nums = bucket.items;
    bucket.items.clear();
    bucket.items.resize(n_len * n_len);

    for (auto number : nums) {
      assert(number.has_value());
      auto idx = bucket.hash(number.value());
      if (bucket.items[idx].has_value()) {
        bucket.items = std::move(nums);
        return false;
      }
      bucket.items[idx] = number;
    }
    return true;
  }

public:
  // NOLINTNEXTLINE(bugprone-random-generator-seed, cert-msc32-c, cert-msc51-cpp)
  FixedSet() = default;
  auto initialize(std::vector<int> const &numbers) -> void {
    while (!try_initialize(numbers)) {
    }
    for (auto &&bucket : buckets_) {
      while (!try_initialize_bucket(bucket)) {
      }
    }
  }
  [[nodiscard]] auto contains(int number) const -> bool {
    if (buckets_.empty()) {
      return false;
    }

    auto &&bucket = buckets_[hash_(number)];
    if (bucket.items.empty()) {
      return false;
    }
    auto &&item = bucket.items[bucket.hash(number)];

    return item.has_value() && item.value() == number;
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
