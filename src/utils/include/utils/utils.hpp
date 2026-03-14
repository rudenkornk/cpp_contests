#pragma once

#include <algorithm>
#include <array>
#include <atomic>
#include <cassert>
#include <chrono>
#include <cstddef>
#include <exception>
#include <filesystem>
#include <format>
#include <functional>
#include <map>
#include <numeric>
#include <random>
#include <stdexcept>
#include <string>
#include <string_view>
#include <thread>
#include <tuple>
#include <utility>
#include <vector>

#include <boost/process.hpp> // NOLINT(misc-include-cleaner)

#include "utils/type_traits.hpp"

namespace cpp_contests {

inline auto size_to_string(size_t size) -> std::string {
  // NOLINTNEXTLINE(altera-id-dependent-backward-branch)
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
  return std::format("{:.1f} {}", result,
                     // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-constant-array-index)
                     units[tier]);
}

template <std::size_t N> inline auto get_indices() -> std::array<size_t, N> {
  auto indices = std::array<size_t, N>();
  std::iota(indices.begin(), indices.end(), size_t{0});
  return indices;
}

template <typename Vector, typename VectorIndexers, typename IndexFunction>
void permute(Vector &vec, VectorIndexers &perm, IndexFunction const &Index) {
  using T = typename Vector::value_type;
  using Indexer = typename VectorIndexers::value_type;
  static_assert(std::is_nothrow_swappable_v<T>);
  static_assert(std::is_nothrow_swappable_v<Indexer>);
  static_assert(std::is_nothrow_invocable_v<IndexFunction, Indexer>);
  using std::swap; // NOLINT(misc-include-header,misc-include-cleaner)
  assert(vec.size() == perm.size());
  if (vec.size() == 0) {
    return;
  }
#ifndef NDEBUG
  assert(std::unique(perm.begin(), perm.end(), [&](Indexer const &lhs, Indexer const &rhs) -> auto {
           return Index(lhs) == Index(rhs);
         }) == perm.end());
  assert(Index(*std::min_element(perm.begin(), perm.end(),
                                 [&](Indexer const &lhs, Indexer const &rhs) -> auto { return lhs < rhs; })) == 0);
  assert(Index(*std::max_element(perm.begin(), perm.end(), [&](Indexer const &lhs, Indexer const &rhs) -> auto {
           return lhs < rhs;
         })) == perm.size() - 1);
  if constexpr (std::is_same_v<T, Indexer>) {
    assert(&vec != &perm);
  }
#endif // !NDEBUG

  auto &&control = std::vector<size_t>(vec.size());
  std::ranges::iota(control, size_t{0});
  for (auto i = size_t{0}, end = vec.size(); i != end; ++i) {
    while (Index(perm[i]) != i) {
      swap(control[i], control[Index(perm[i])]);
      swap(perm[i], perm[Index(perm[i])]);
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
  auto permutation = get_indices(vec.size());
  std::sort(permutation.begin(), permutation.end(),
            [&](size_t index0, size_t index1) -> auto { return cmp(vec[index0], vec[index1]); });
  return permutation;
}

template <typename Vector> auto get_sort_permutation(Vector const &vec) -> std::vector<size_t> {
  return get_sort_permutation(vec, std::less<>{});
}

template <typename Generator = std::mt19937, unsigned seed = 0> auto get_random_generator() -> Generator & {
  auto static thread_local generator = Generator{seed};
  return generator;
}

template <size_t NRuns = 1, typename FG, typename... Args>
auto benchmark(FG const &Func, Args &&...args) -> std::chrono::nanoseconds {
  static_assert(CallableTraits<FG>::nArguments == sizeof...(args));
  auto start = std::chrono::steady_clock::now();
  for (auto i = size_t{0}; i != NRuns; ++i) {
    Func(std::forward<Args>(args)...);
  }
  auto end = std::chrono::steady_clock::now();
  return (end - start) / NRuns;
}

template <typename T> class SaveRestore final {
  static_assert(std::is_nothrow_move_assignable_v<T>);
  static_assert(!std::is_reference_v<T>);
  T originalValue;
  T *restoreTo = nullptr;

public:
  explicit SaveRestore(T &value) noexcept(std::is_nothrow_copy_constructible_v<T>)
      : originalValue{std::as_const(value)}, restoreTo{&value} {}
  explicit SaveRestore(T &&value) noexcept(std::is_nothrow_move_constructible_v<T>)
      : originalValue{std::move(value)}, restoreTo{&value} {}
  explicit SaveRestore(T &&value, T &restoreTo) noexcept(std::is_nothrow_move_constructible_v<T>)
      : originalValue{std::move(value)}, restoreTo{&restoreTo} {}

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
    swap(restoreTo, other.restoreTo);
    swap(originalValue, other.originalValue);
  }

  ~SaveRestore() { *restoreTo = std::move(originalValue.value()); }
};

template <typename T> void swap(SaveRestore<T> &left, SaveRestore<T> &right) noexcept { left.swap(right); }

// Save exceptions in multithreading environment
class ExceptionSaver final {
  std::atomic<size_t> nCapturedExceptions = 0;
  std::atomic<size_t> nSavedExceptions = 0;
  std::vector<std::exception_ptr> exceptions;

public:
  explicit ExceptionSaver(size_t maxExceptions = 1) { exceptions.resize(maxExceptions); }
  ExceptionSaver(ExceptionSaver const &) = delete;
  ExceptionSaver(ExceptionSaver &&other) noexcept { swap(other); }
  auto operator=(ExceptionSaver const &) -> ExceptionSaver & = delete;
  auto operator=(ExceptionSaver &&other) & noexcept -> ExceptionSaver & {
    swap(other);
    return *this;
  }
  ~ExceptionSaver() noexcept(false) { rethrow(); }

  [[nodiscard]] auto ncaptured() const noexcept -> size_t { return nCapturedExceptions; }
  [[nodiscard]] auto nsaved() const noexcept -> size_t { return nSavedExceptions; }

  void swap(ExceptionSaver &other) noexcept {
    using std::swap;
    nCapturedExceptions = other.nCapturedExceptions.exchange(nCapturedExceptions);
    nSavedExceptions = other.nSavedExceptions.exchange(nSavedExceptions);
    swap(exceptions, other.exceptions);
  }

  // Wraps callable in a thread-save wrapper
  template <typename Callable> auto wrap(Callable &&callable) {
    using ReturnType = typename CallableTraits<Callable>::template Type<0>;
    static_assert(std::is_void_v<ReturnType> ||
                  (std::is_nothrow_default_constructible_v<ReturnType> && !std::is_reference_v<ReturnType>));
    return _wrap(std::forward<Callable>(callable), std::make_index_sequence<CallableTraits<Callable>::nArguments>{});
  }

  void rethrow() {
#ifndef NDEBUG
    for (auto i = size_t{0}, end = exceptions.size(); i != end; ++i) {
      assert(static_cast<bool>(exceptions[i]) == (i < nSavedExceptions));
    }
#endif // !NDEBUG
    if (nSavedExceptions == 0U) {
      return;
    }
    using std::swap;
    auto exc = std::exception_ptr{};
    swap(exc, exceptions[--nSavedExceptions]);
    std::rethrow_exception(exc);
  }
  void drop() noexcept {
    std::fill_n(exceptions.begin(), nSavedExceptions.load(), std::exception_ptr{});
    nSavedExceptions = 0;
#ifndef NDEBUG
    for (auto &&ptr : exceptions) {
      assert(!ptr);
    }
#endif // !NDEBUG
  }
  void set_max_exceptions(size_t max_exceptions) { exceptions.resize(max_exceptions); }

private:
  template <class Callable, size_t... Indices>
  auto wrap_(Callable const &callable, std::integer_sequence<size_t, Indices...> /*unused*/) {
    using ReturnType = typename CallableTraits<Callable>::ReturnType;
    return [&](typename CallableTraits<Callable>::template ArgType<Indices>... args) noexcept -> auto {
      try {
        return callable(std::forward<typename CallableTraits<Callable>::template ArgType<Indices>>(args)...);
      } catch (...) {
        size_t const index = nCapturedExceptions++;
        if (index < exceptions.size()) {
          ++nSavedExceptions;
          exceptions[index] = std::current_exception();
        }
        if constexpr (!std::is_void_v<ReturnType>) {
          return ReturnType{};
        }
      }
    };
  }
};

inline void swap(ExceptionSaver &left, ExceptionSaver &right) noexcept { left.swap(right); }

static inline auto
// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
run_shell(std::string const &cmd, std::string const &stdin_data = "",
          std::map<std::string, std::string> const &extra_env = {},
          std::vector<std::filesystem::path> const &extra_paths = {},
          std::filesystem::path const &cwd = std::filesystem::current_path(), bool check = true)
    -> std::tuple<int, std::string, std::string> {

  // NOLINTBEGIN(misc-include-cleaner)
  namespace bp = boost::process;

  bp::ipstream stdout_stream;
  bp::ipstream stderr_stream;

  auto env = boost::this_process::environment();

  for (auto const &[key, value] : extra_env) {
    env[key] = value;
  }

  if (!extra_paths.empty()) {
    std::string path_value;
    for (auto const &path : extra_paths) {
      if (!path_value.empty()) {
        path_value += ":";
      }
      path_value += path.string();
    }
    if (!env["PATH"].empty()) {
      path_value += ":" + env["PATH"].to_string();
    }
    env["PATH"] = path_value;
  }

  bp::child process;
  bp::opstream stdin_stream;

  if (!stdin_data.empty()) {
    process = bp::child(cmd, env, bp::std_in<stdin_stream, bp::std_out> stdout_stream, bp::std_err > stderr_stream,
                        bp::start_dir = cwd.string());
    stdin_stream << stdin_data;
    stdin_stream.flush();
  } else {
    process =
        bp::child(cmd, env, bp::std_out > stdout_stream, bp::std_err > stderr_stream, bp::start_dir = cwd.string());
  }

  std::string stdout_result;
  std::string stderr_result;

  auto read_stream = [](bp::ipstream &stream, std::string &result) -> void {
    std::string line;
    while (std::getline(stream, line)) {
      result += line + "\n";
    }
  };

  std::thread stdout_thread(read_stream, std::ref(stdout_stream), std::ref(stdout_result));
  std::thread stderr_thread(read_stream, std::ref(stderr_stream), std::ref(stderr_result));

  // Close stdin after starting read threads to signal EOF
  if (!stdin_data.empty()) {
    stdin_stream.pipe().close();
  }

  stdout_thread.join();
  stderr_thread.join();

  process.wait();
  int const exit_code = process.exit_code();

  if (check && exit_code != 0) {
    throw std::runtime_error("Command failed with exit code " + std::to_string(exit_code));
  }

  return std::make_tuple(exit_code, std::move(stdout_result), std::move(stderr_result));

  // NOLINTEND(misc-include-cleaner)
}

} // namespace cpp_contests
