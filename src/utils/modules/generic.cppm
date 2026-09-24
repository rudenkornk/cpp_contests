module;

#include <algorithm>
#include <array>
#include <atomic>
#include <cassert>
#include <chrono>
#include <cstddef>
#include <exception>
#include <format>
#include <functional>
#include <numeric>
#include <random>
#include <ranges>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

export module utils:generic;
import :type_traits;

export namespace cpp_contests {

inline auto size_to_string(size_t size) -> std::string {
  constexpr std::array<std::string_view, 8> units = {"B", "KiB", "MiB", "GiB", "TiB", "PiB", "EiB", "ZiB"};
  constexpr std::size_t base = 1024;
  if (size == 0) {
    return "0 B";
  }

  std::size_t tier = 0;
  double result = 0;
  bool is_exact = true;
  while (size != 0) {
    std::size_t const residual = size % base;
    result = static_cast<double>(residual) + (result / base);
    size /= base;
    if (residual > 0 && size > 0) {
      is_exact = false;
    }
    ++tier;
  }
  --tier;
  if (is_exact) {
    return std::format("{} {}", result,
                       // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-constant-array-index)
                       units[tier]);
  }
  // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-constant-array-index)
  return std::format("{:.1f} {}", result, units[tier]);
}

template <std::size_t N> inline auto get_indices() -> std::array<size_t, N> {
  auto indices = std::array<size_t, N>();
  std::iota(indices.begin(), indices.end(), size_t{0});
  return indices;
}

template <typename Vector, typename VectorIndexers, typename IndexFunction>
void permute(Vector &vec, VectorIndexers &perm, IndexFunction const &index) {
  using value_type = Vector::value_type;
  using indexer = VectorIndexers::value_type;
  static_assert(std::is_nothrow_swappable_v<value_type>);
  static_assert(std::is_nothrow_swappable_v<indexer>);
  static_assert(std::is_nothrow_invocable_v<IndexFunction, indexer>);
  using std::swap;
  assert(vec.size() == perm.size());
  if (vec.size() == 0) {
    return;
  }
#ifndef NDEBUG
  {
    // A valid permutation maps onto every index in [0, size) exactly once.
    // Validate on a copy: mutating checks (e.g. std::unique) would corrupt perm on violation.
    auto indices = std::vector<size_t>(perm.size());
    std::ranges::transform(perm, indices.begin(), index);
    std::ranges::sort(indices);
    assert(std::ranges::adjacent_find(indices) == indices.end());
    assert(indices.front() == 0);
    assert(indices.back() == perm.size() - 1);
  }
  if constexpr (std::is_same_v<value_type, indexer>) {
    assert(&vec != &perm);
  }
#endif // !NDEBUG

  auto &&control = std::vector<size_t>(vec.size());
  std::ranges::iota(control, size_t{0});
  for (auto i = size_t{0}, end = vec.size(); i != end; ++i) {
    while (index(perm[i]) != i) {
      swap(control[i], control[index(perm[i])]);
      swap(perm[i], perm[index(perm[i])]);
    }
  }
  for (auto i = size_t{0}, end = vec.size(); i != end; ++i) {
    while (control[i] != i) {
      swap(vec[i], vec[control[i]]);
      swap(perm[i], perm[control[i]]);
      swap(control[i], control[control[i]]);
    }
  }
}

template <typename Vector, typename VectorIndexers> void permute(Vector &vec, VectorIndexers &perm) {
  permute(vec, perm, std::identity{});
}

template <typename Vector, typename Comparator>
auto get_sort_permutation(Vector const &vec, Comparator const &cmp) -> std::vector<size_t> {
  auto permutation = std::views::iota(size_t{0}, vec.size()) | std::ranges::to<std::vector>();
  std::ranges::sort(permutation, [&](size_t index0, size_t index1) -> auto { return cmp(vec[index0], vec[index1]); });
  return permutation;
}

template <typename Vector> auto get_sort_permutation(Vector const &vec) -> std::vector<size_t> {
  return get_sort_permutation(vec, std::less<>{});
}

template <typename Generator = std::mt19937, unsigned Seed = 0> auto get_random_generator() -> Generator & {
  auto static thread_local generator = Generator{Seed};
  return generator;
}

template <size_t NRuns = 1, typename FG, typename... Args>
auto benchmark(FG const &func, Args &&...args) -> std::chrono::nanoseconds {
  static_assert(NRuns > 0);
  static_assert(CallableTraits<FG>::kNArguments == sizeof...(args));
  auto start = std::chrono::steady_clock::now();
  for (auto i = size_t{1}; i < NRuns; ++i) {
    // Arguments may only be forwarded on the last run: moving from them more than once would be a bug.
    func(args...);
  }
  func(std::forward<Args>(args)...);
  auto end = std::chrono::steady_clock::now();
  return (end - start) / NRuns;
}

template <typename T> class SaveRestore final {
  static_assert(std::is_nothrow_move_assignable_v<T>);
  static_assert(!std::is_reference_v<T>);
  T original_value_;
  T *restore_to_ = nullptr;

public:
  explicit SaveRestore(T &value) noexcept(std::is_nothrow_copy_constructible_v<T>)
      : original_value_{std::as_const(value)}, restore_to_{&value} {}
  explicit SaveRestore(T &&value) noexcept(std::is_nothrow_move_constructible_v<T>)
      : original_value_{std::move(value)}, restore_to_{&value} {}
  explicit SaveRestore(T &&value, T &restore_to) noexcept(std::is_nothrow_move_constructible_v<T>)
      : original_value_{std::move(value)}, restore_to_{&restore_to} {}

  SaveRestore(SaveRestore const &) = delete;
  SaveRestore(SaveRestore &&other) noexcept { swap(other); }
  auto operator=(SaveRestore const &) -> SaveRestore & = delete;
  auto operator=(SaveRestore &&other) & noexcept -> SaveRestore & {
    swap(other);
    return *this;
  }

  void swap(SaveRestore &other) noexcept {
    static_assert(std::is_nothrow_swappable_v<T>);
    using std::swap;
    swap(restore_to_, other.restore_to_);
    swap(original_value_, other.original_value_);
  }

  ~SaveRestore() {
    // Moved-from objects have no target to restore.
    if (restore_to_ != nullptr) {
      *restore_to_ = std::move(original_value_);
    }
  }
};

template <typename T> void swap(SaveRestore<T> &left, SaveRestore<T> &right) noexcept { left.swap(right); }

// Save exceptions in multithreading environment
class ExceptionSaver final {
  std::atomic<size_t> n_captured_exceptions_ = 0;
  std::atomic<size_t> n_saved_exceptions_ = 0;
  std::vector<std::exception_ptr> exceptions_;

public:
  explicit ExceptionSaver(size_t max_exceptions = 1) { exceptions_.resize(max_exceptions); }
  ExceptionSaver(ExceptionSaver const &) = delete;
  ExceptionSaver(ExceptionSaver &&other) noexcept { swap(other); }
  auto operator=(ExceptionSaver const &) -> ExceptionSaver & = delete;
  auto operator=(ExceptionSaver &&other) & noexcept -> ExceptionSaver & {
    swap(other);
    return *this;
  }
  ~ExceptionSaver() noexcept(false) { rethrow(); }

  [[nodiscard]] auto ncaptured() const noexcept -> size_t { return n_captured_exceptions_; }
  [[nodiscard]] auto nsaved() const noexcept -> size_t { return n_saved_exceptions_; }

  void swap(ExceptionSaver &other) noexcept {
    using std::swap;
    n_captured_exceptions_ = other.n_captured_exceptions_.exchange(n_captured_exceptions_);
    n_saved_exceptions_ = other.n_saved_exceptions_.exchange(n_saved_exceptions_);
    swap(exceptions_, other.exceptions_);
  }

  // Wraps callable in a thread-save wrapper
  template <typename Callable> auto wrap(Callable &&callable) {
    using return_type = CallableTraits<Callable>::template type<0>;
    static_assert(std::is_void_v<return_type> ||
                  (std::is_nothrow_default_constructible_v<return_type> && !std::is_reference_v<return_type>));
    return wrap(std::forward<Callable>(callable), std::make_index_sequence<CallableTraits<Callable>::kNArguments>{});
  }

  void rethrow() {
#ifndef NDEBUG
    for (auto i = size_t{0}, end = exceptions_.size(); i != end; ++i) {
      assert(static_cast<bool>(exceptions_[i]) == (i < n_saved_exceptions_));
    }
#endif // !NDEBUG
    if (n_saved_exceptions_ == 0U) {
      return;
    }
    using std::swap;
    auto exc = std::exception_ptr{};
    swap(exc, exceptions_[--n_saved_exceptions_]);
    std::rethrow_exception(exc);
  }
  void drop() noexcept {
    std::fill_n(exceptions_.begin(), n_saved_exceptions_.load(), std::exception_ptr{});
    n_saved_exceptions_ = 0;
#ifndef NDEBUG
    for (auto &&ptr : exceptions_) {
      assert(!ptr);
    }
#endif // !NDEBUG
  }
  void set_max_exceptions(size_t max_exceptions) { exceptions_.resize(max_exceptions); }

private:
  void save_current_exception() noexcept {
    size_t const index = n_captured_exceptions_++;
    if (index >= exceptions_.size()) {
      return;
    }
    ++n_saved_exceptions_;
    exceptions_[index] = std::current_exception();
  }

  template <typename Result> static auto empty_result() -> Result {
    if constexpr (!std::is_void_v<Result>) {
      return Result{};
    }
  }

  template <class Callable, size_t... Indices>
  auto wrap(Callable callable, std::integer_sequence<size_t, Indices...> /*unused*/) {
    using return_type = CallableTraits<Callable>::return_type;
    // The callable is captured by value: capturing by reference would dangle for the common
    // `saver.wrap([...]{...})` pattern where the argument is a temporary.
    // The saver itself must outlive the wrapper, hence capturing `this` is fine.
    return [this, callable = std::move(callable)](
               CallableTraits<Callable>::template arg_type<Indices>... args) noexcept -> auto {
      try {
        return callable(std::forward<typename CallableTraits<Callable>::template arg_type<Indices>>(args)...);
      } catch (...) {
        save_current_exception();
        return empty_result<return_type>();
      }
    };
  }
};

inline void swap(ExceptionSaver &left, ExceptionSaver &right) noexcept { left.swap(right); }

} // namespace cpp_contests
