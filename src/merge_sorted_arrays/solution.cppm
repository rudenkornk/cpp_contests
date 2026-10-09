module;

#include <algorithm>
#include <cassert>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <functional>
#include <iostream>
#include <print>
#include <ranges>
#include <span>
#include <type_traits>
#include <utility>
#include <vector>

export module merge_sorted_arrays;

namespace cpp_contests {
namespace {

template <typename C, typename T>
concept PriorityQueueContainer =
    std::ranges::random_access_range<C> && std::same_as<std::ranges::range_value_t<C>, T> &&
    std::same_as<typename C::value_type, T> && requires(C container, T value) {
      { container.front() } -> std::same_as<T &>;
      container.push_back(value);
      container.pop_back();
    };
template <typename Comp, typename T>
concept PriorityQueueCompare = std::strict_weak_order<Comp, T, T>;

// Writing a proper class in C++ is quite a task, so keep it as simple as possible.
template <class T, PriorityQueueContainer<T> Container = std::vector<T>,
          PriorityQueueCompare<T> Compare = std::less<typename Container::value_type>>
class PriorityQueue {
private:
  Container cont_{};
  Compare comp_{};

public:
  // NOLINTBEGIN(readability-identifier-naming)
  using const_reference = Container::const_reference;
  using container_type = Container;
  using reference = Container::reference;
  using size_type = Container::size_type;
  using value_compare = Compare;
  using value_type = Container::value_type;

  [[nodiscard]] auto top() const -> T const & { return cont_.front(); }
  [[nodiscard]] auto empty() const -> bool { return cont_.empty(); }
  [[nodiscard]] auto size() const -> size_type { return cont_.size(); }
  auto swap(PriorityQueue &other) noexcept(std::is_nothrow_swappable_v<Container> &&
                                           std::is_nothrow_swappable_v<Compare>) -> void {
    using std::swap;
    swap(cont_, other.cont_);
    swap(comp_, other.comp_);
  }
  // NOLINTEND(readability-identifier-naming)

private:
  static constexpr bool kNothrowInplaceOps =
      noexcept(std::declval<Container &>()[std::declval<size_type &>()]) &&
      noexcept(static_cast<bool>(
          std::invoke(std::declval<Compare &>(), std::declval<reference>(), std::declval<reference>()))) &&
      noexcept(std::swap(std::declval<reference>(), std::declval<reference>()));

  auto sift_up(Container &arr, size_type idx) noexcept(kNothrowInplaceOps) -> void {
    assert(0 <= idx && idx < arr.size());

    using std::swap;
    while (idx > 0) {
      auto parent = (idx - 1) / 2;

      if (!static_cast<bool>(std::invoke(comp_, arr[parent], arr[idx]))) {
        break;
      }
      swap(arr[parent], arr[idx]);
      idx = parent;
    }
  }

  auto sift_down(Container &arr, size_type idx) noexcept(kNothrowInplaceOps) -> void {
    assert(0 <= idx && idx < arr.size());
    using std::swap;
    const auto len = arr.size();

    while (idx < len / 2) {
      auto child1 = (2 * idx) + 1;
      auto child2 = (2 * idx) + 2;
      auto preferred_child = child1;
      if (child2 < len) {
        preferred_child = std::invoke(comp_, arr[child1], arr[child2]) ? child2 : child1;
      }

      if (!static_cast<bool>(std::invoke(comp_, arr[idx], arr[preferred_child]))) {
        break;
      }
      swap(arr[idx], arr[preferred_child]);
      idx = preferred_child;
    }
  }

public:
  PriorityQueue() = default;
  explicit PriorityQueue(Container container = Container{}, Compare compare = Compare{})
      : cont_(std::move(container)), comp_(std::move(compare)) {
    auto idx = cont_.size() / 2;
    while (idx > 0) {
      --idx;
      sift_down(cont_, idx);
    }
  }
  explicit PriorityQueue(Compare compare) : comp_(std::move(compare)) {}

  // NOLINTBEGIN(readability-identifier-naming)
  auto push(const_reference value) -> void {
    cont_.push_back(value);
    sift_up(cont_, cont_.size() - 1);
  }

  auto push(value_type &&value) -> void {
    cont_.push_back(std::move(value));
    sift_up(cont_, cont_.size() - 1);
  }
  template <class... Args> auto emplace(Args &&...args) -> void {
    cont_.emplace_back(std::forward<Args>(args)...);
    sift_up(cont_, cont_.size() - 1);
  }
  auto pop() noexcept(kNothrowInplaceOps && noexcept(std::declval<Container &>().pop_back()))

      -> void {
    assert(!cont_.empty());
    using std::swap;
    swap(cont_.front(), cont_.back());
    cont_.pop_back();
    if (!cont_.empty()) {
      sift_down(cont_, 0);
    }
  }
  // NOLINTEND(readability-identifier-naming)
};

struct ArrPtr {
  int32_t val;
  std::size_t arr;
  std::size_t idx;
};

auto solve(std::span<int32_t> nums, std::size_t n_len, std::size_t m_len) -> std::vector<int32_t> {
  auto mins_vec = std::vector<ArrPtr>(n_len);
  for (std::size_t i = 0; i < n_len; ++i) {
    mins_vec[i] = ArrPtr{.val = nums[i * m_len], .arr = i, .idx = 0};
  }

  auto compare = [](const ArrPtr &lhs, const ArrPtr &rhs) noexcept -> bool { return lhs.val > rhs.val; };
  auto mins = PriorityQueue<ArrPtr, std::vector<ArrPtr>, decltype(compare)>(std::move(mins_vec), compare);

  auto result = std::vector<int32_t>();
  result.reserve(n_len * m_len);

  while (!mins.empty()) {
    auto top = mins.top();
    result.push_back(top.val);
    mins.pop();

    auto next_idx = top.idx + 1;
    if (next_idx < m_len) {
      top.val = nums[(top.arr * m_len) + next_idx];
      top.idx = next_idx;
      mins.push(top);
    }
  }

  return result;
}

auto run([[maybe_unused]] std::span<char *> args) -> void {
  std::size_t n_len = 0;
  std::size_t m_len = 0;

  std::cin >> n_len >> m_len;

  std::vector<int32_t> nums(n_len * m_len);

  for (std::size_t i = 0; i < n_len * m_len; ++i) {
    std::cin >> nums[i];
  }

  auto result = solve(nums, n_len, m_len);
  for (auto const &value : result) {
    std::print("{} ", value);
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
