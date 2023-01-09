#pragma once

#include <algorithm>
#include <array>
#include <atomic>
#include <cassert>
#include <chrono>
#include <exception>
#include <numeric>
#include <random>
#include <string>
#include <string_view>
#include <vector>

#include <fmt/core.h>

#include "utils/type_traits.hpp"

namespace cpp_contests {

inline std::string size_to_string(size_t size) {
  // NOLINTNEXTLINE
  constexpr std::array<std::string_view, 8> units = {
      "B", "KiB", "MiB", "GiB", "TiB", "PiB", "EiB", "ZiB"};
  constexpr std::size_t base = 1024;
  if (size == 0) {
    return "0 B";
  }

  std::size_t tier = 0;
  double result = 0;
  bool is_exact = true;
  while (size != 0) {
    std::size_t residual = size % base;
    result = static_cast<double>(residual) + result / base;
    size /= base;
    if (residual > 0 && size > 0)
      is_exact = false;
    ++tier;
  }
  --tier;
  if (is_exact)
    return fmt::format("{} {}", result, units[tier]); // TODO std::format
  else
    return fmt::format("{:.1f} {}", result, units[tier]); // TODO std::format
}

template <std::size_t N> inline std::array<size_t, N> get_indices() {
  auto indices = std::array<size_t, N>();
  std::iota(indices.begin(), indices.end(), size_t{0});
  return indices;
}

template <typename Vector, typename VectorIndexers, typename IndexFunction>
void permute(Vector &v, VectorIndexers &perm, IndexFunction const &Index) {
  using T = typename Vector::value_type;
  using Indexer = typename VectorIndexers::value_type;
  static_assert(std::is_nothrow_swappable_v<T>);
  static_assert(std::is_nothrow_swappable_v<Indexer>);
  static_assert(std::is_nothrow_invocable_v<IndexFunction, Indexer>);
  using std::swap;
  assert(v.size() == perm.size());
  if (v.size() == 0)
    return;
#ifndef NDEBUG
  assert(std::unique(perm.begin(), perm.end(),
                     [&](Indexer const &lhs, Indexer const &rhs) {
                       return Index(lhs) == Index(rhs);
                     }) == perm.end());
  assert(Index(*std::min_element(perm.begin(), perm.end(),
                                 [&](Indexer const &lhs, Indexer const &rhs) {
                                   return lhs < rhs;
                                 })) == 0);
  assert(Index(*std::max_element(perm.begin(), perm.end(),
                                 [&](Indexer const &lhs, Indexer const &rhs) {
                                   return lhs < rhs;
                                 })) == perm.size() - 1);
  if constexpr (std::is_same_v<T, Indexer>) {
    assert(&v != &perm);
  }
#endif // !NDEBUG

  auto &&control = std::vector<size_t>(v.size());
  std::iota(control.begin(), control.end(), size_t{0});
  for (auto i = size_t{0}, e = v.size(); i != e; ++i) {
    while (Index(perm[i]) != i) {
      swap(control[i], control[Index(perm[i])]);
      swap(perm[i], perm[Index(perm[i])]);
    }
  }
  for (auto i = size_t{0}, e = v.size(); i != e; ++i) {
    while (control[i] != i) {
      swap(v[i], v[control[i]]);
      swap(perm[i], perm[control[i]]);
      swap(control[i], control[control[i]]);
    }
  }
}

template <typename Vector, typename VectorIndexers>
void permute(Vector &v, VectorIndexers &perm) {
  permute(v, perm, std::identity{});
}

template <typename Vector, typename Comparator>
std::vector<size_t> get_sort_permutation(Vector const &v,
                                         Comparator const &cmp) {
  auto permutation = get_indices(v.size());
  std::sort(
      permutation.begin(), permutation.end(),
      [&](size_t index0, size_t index1) { return cmp(v[index0], v[index1]); });
  return permutation;
}

template <typename Vector>
std::vector<size_t> get_sort_permutation(Vector const &v) {
  return get_sort_permutation(v, std::less<>{});
}

template <typename Generator = std::mt19937, unsigned seed = 0>
Generator &get_random_generator() {
  auto static thread_local generator = Generator{seed};
  return generator;
}

template <size_t NRuns = 1, typename FG, typename... Args>
std::chrono::nanoseconds benchmark(FG const &Func, Args &&...args) {
  static_assert(CallableTraits<FG>::nArguments == sizeof...(args));
  auto start = std::chrono::steady_clock::now();
  for (auto i = size_t{0}; i != NRuns; ++i)
    Func(std::forward<Args>(args)...);
  auto end = std::chrono::steady_clock::now();
  return (end - start) / NRuns;
}

template <typename T> class SaveRestore final {
  static_assert(std::is_nothrow_move_assignable_v<T>);
  static_assert(!std::is_reference_v<T>);
  T originalValue;
  T *restoreTo = nullptr;

public:
  explicit SaveRestore(T &value) noexcept(
      std::is_nothrow_copy_constructible_v<T>)
      : originalValue{std::as_const(value)}, restoreTo{&value} {}
  explicit SaveRestore(T &&value) noexcept(
      std::is_nothrow_move_constructible_v<T>)
      : originalValue{std::move(value)}, restoreTo{&value} {}
  explicit SaveRestore(T &&value, T &restoreTo) noexcept(
      std::is_nothrow_move_constructible_v<T>)
      : originalValue{std::move(value)}, restoreTo{&restoreTo} {}

  SaveRestore(SaveRestore const &) = delete;
  SaveRestore(SaveRestore &&other) noexcept { swap(other); }
  SaveRestore &operator=(SaveRestore const &) = delete;
  SaveRestore &operator=(SaveRestore &&other) & noexcept {
    swap(other);
    return *this;
  }

  void swap(SaveRestore &other) noexcept {
    static_assert(std::is_nothrow_swappable_v<T>);
    using std::swap;
    swap(restoreTo, other.restoreTo);
    swap(originalValue, other.originalValue);
  }

  ~SaveRestore() { *restoreTo = std::move(originalValue.value()); }
};

template <typename T>
void swap(SaveRestore<T> &left, SaveRestore<T> &right) noexcept {
  left.swap(right);
}

// Save exceptions in multithreading environment
class ExceptionSaver final {
  std::atomic<size_t> nCapturedExceptions = 0;
  std::atomic<size_t> nSavedExceptions = 0;
  std::vector<std::exception_ptr> exceptions;

public:
  ExceptionSaver(size_t maxExceptions = 1) { exceptions.resize(maxExceptions); }
  ExceptionSaver(ExceptionSaver const &) = delete;
  ExceptionSaver(ExceptionSaver &&other) noexcept { swap(other); }
  ExceptionSaver &operator=(ExceptionSaver const &) = delete;
  ExceptionSaver &operator=(ExceptionSaver &&other) & noexcept {
    swap(other);
    return *this;
  }
  ~ExceptionSaver() noexcept(false) { rethrow(); }

  [[nodiscard]] size_t ncaptured() const noexcept {
    return nCapturedExceptions;
  }
  [[nodiscard]] size_t nsaved() const noexcept { return nSavedExceptions; }

  void swap(ExceptionSaver &other) noexcept {
    using std::swap;
    nCapturedExceptions =
        other.nCapturedExceptions.exchange(nCapturedExceptions);
    nSavedExceptions = other.nSavedExceptions.exchange(nSavedExceptions);
    swap(exceptions, other.exceptions);
  }

  // Wraps callable in a thread-save wrapper
  template <typename Callable> auto wrap(Callable &&callable) {
    using ReturnType = typename CallableTraits<Callable>::template Type<0>;
    static_assert(std::is_void_v<ReturnType> ||
                  (std::is_nothrow_default_constructible_v<ReturnType> &&
                   !std::is_reference_v<ReturnType>));
    return _wrap(
        std::forward<Callable>(callable),
        std::make_index_sequence<CallableTraits<Callable>::nArguments>{});
  }

  void rethrow() {
#ifndef NDEBUG
    for (auto i = size_t{0}, e = exceptions.size(); i != e; ++i)
      assert(static_cast<bool>(exceptions[i]) == (i < nSavedExceptions));
#endif // !NDEBUG
    if (!nSavedExceptions)
      return;
    using std::swap;
    auto e = std::exception_ptr{};
    swap(e, exceptions[--nSavedExceptions]);
    std::rethrow_exception(e);
  }
  void drop() noexcept {
    std::fill_n(exceptions.begin(), nSavedExceptions.load(),
                std::exception_ptr{});
    nSavedExceptions = 0;
#ifndef NDEBUG
    for (auto &&ptr : exceptions)
      assert(!ptr);
#endif // !NDEBUG
  }
  void set_max_exceptions(size_t max_exceptions) {
    exceptions.resize(max_exceptions);
  }

private:
  template <class Callable, size_t... Indices>
  auto wrap_(Callable const &callable,
             std::integer_sequence<size_t, Indices...>) {
    using ReturnType = typename CallableTraits<Callable>::ReturnType;
    return [&](typename CallableTraits<Callable>::template ArgType<
               Indices>... args) noexcept {
      try {
        return callable(
            std::forward<
                typename CallableTraits<Callable>::template ArgType<Indices>>(
                args)...);
      } catch (...) {
        size_t index = nCapturedExceptions++;
        if (index < exceptions.size()) {
          ++nSavedExceptions;
          exceptions[index] = std::current_exception();
        }
        if constexpr (!std::is_void_v<ReturnType>)
          return ReturnType{};
      }
    };
  }
};

void swap(ExceptionSaver &left, ExceptionSaver &right) noexcept {
  left.swap(right);
}
} // namespace cpp_contests
