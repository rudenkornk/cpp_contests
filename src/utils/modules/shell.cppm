module;

#include <array>
#include <cctype>
#include <cerrno>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <fcntl.h>
#include <filesystem>
#include <format>
#include <map>
#include <stdexcept>
#include <string>
#include <string_view>
#include <sys/types.h>
#include <sys/wait.h>
#include <tuple>
#include <unistd.h>
#include <utility>
#include <vector>

export module utils:shell;

// This is the implementation of run_shell util, written only with stdlib and POSIX.
// Initially, run_shell was written in a much simpler way with a use of <boost/process.hpp>.
// However, boost.process appears to be not ready for C++20 modules, which makes gcc fail to compile it.
// Specificall gcc fails with the following error on that implementation:
// boost/fusion/container/set/set.hpp:106:9: error:
//  ‘template<class ... T> template<class Sequence> boost::fusion::set<T>::set(...)’
//  exposes TU-local entity ‘boost::fusion::detail::enabler’.
//
// Once that part of the boost is compatible with C++20 modules, this implementation could probably be
// reverted to boost variant.

namespace cpp_contests {

namespace detail {
// TODO: remove this NOLINT once all utils are under single interface module declaration.
// NOLINTBEGIN(misc-use-internal-linkage)

// RAII wrapper around a pair of file descriptors created by pipe2().
// Ensures both ends are closed on destruction unless explicitly released.
struct Pipe {
  std::array<int, 2> fds{-1, -1}; // NOLINT(misc-non-private-member-variables-in-classes)

  Pipe() = default;
  Pipe(Pipe const &) = delete;
  Pipe(Pipe &&other) noexcept : fds(other.fds) { other.fds = {-1, -1}; }
  auto operator=(Pipe const &) -> Pipe & = delete;
  auto operator=(Pipe &&) -> Pipe & = delete;

  ~Pipe() {
    if (fds[0] != -1) {
      ::close(fds[0]);
    }
    if (fds[1] != -1) {
      ::close(fds[1]);
    }
  }

  static auto create() -> Pipe {
    Pipe pipe;
    if (::pipe2(pipe.fds.data(), O_CLOEXEC) != 0) {
      throw std::runtime_error("run_shell: pipe2() failed");
    }
    return pipe;
  }

  // Release ownership of one end and return its fd.
  auto release(int end) -> int {
    int const fdesc = fds[end]; // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
    fds[end] = -1;              // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
    return fdesc;
  }

  // Close one end explicitly (idempotent).
  void close_end(int end) {
    if (fds[end] != -1) { // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
      ::close(fds[end]);  // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
      fds[end] = -1;      // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
    }
  }

  [[nodiscard]] auto read_end() const noexcept -> int { return fds[0]; }
  [[nodiscard]] auto write_end() const noexcept -> int { return fds[1]; }
} __attribute__((aligned(8))); // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)

enum class ShellState : std::uint8_t { Normal, SingleQuote, DoubleQuote };

constexpr int EXIT_CODE_DUP2_FAILED = 125;
constexpr int EXIT_CODE_CHDIR_FAILED = 126;
constexpr int EXIT_CODE_EXEC_FAILED = 127;

// Returns true and advances idx past the escaped character if a backslash escape was consumed.
inline auto handle_normal_escape(std::string_view cmd, std::size_t &idx, std::string &current) -> bool {
  if (cmd[idx] == '\\' && idx + 1 < cmd.size()) {
    ++idx;
    current += cmd[idx];
    return true;
  }
  return false;
}

// Process one character in Normal state; returns the next state.
inline auto handle_normal_char(std::string_view cmd, std::size_t &idx, std::string &current,
                               std::vector<std::string> &tokens) -> ShellState {
  char const chr = cmd[idx];
  if (handle_normal_escape(cmd, idx, current)) {
    return ShellState::Normal;
  }
  if (chr == '\'') {
    return ShellState::SingleQuote;
  }
  if (chr == '"') {
    return ShellState::DoubleQuote;
  }
  if (std::isspace(static_cast<unsigned char>(chr)) != 0) {
    if (!current.empty()) {
      tokens.push_back(std::move(current));
      current.clear();
    }
    return ShellState::Normal;
  }
  current += chr;
  return ShellState::Normal;
}

// Process one character in DoubleQuote state; returns the next state.
inline auto handle_doublequote_char(std::string_view cmd, std::size_t &idx, std::string &current) -> ShellState {
  char const chr = cmd[idx];
  if (chr == '\\' && idx + 1 < cmd.size()) {
    char const next_chr = cmd[idx + 1];
    if (next_chr == '"' || next_chr == '\\' || next_chr == '$' || next_chr == '`' || next_chr == '\n') {
      ++idx;
      current += cmd[idx];
      return ShellState::DoubleQuote;
    }
  }
  if (chr == '"') {
    return ShellState::Normal;
  }
  current += chr;
  return ShellState::DoubleQuote;
}

// Minimal shlex-style split: handles single/double quotes and backslash escapes.
inline auto shlex_split(std::string_view cmd) -> std::vector<std::string> {
  std::vector<std::string> tokens;
  std::string current;
  auto state = ShellState::Normal;

  for (std::size_t idx = 0; idx < cmd.size(); ++idx) {
    char const chr = cmd[idx];
    switch (state) {
    case ShellState::Normal:
      state = handle_normal_char(cmd, idx, current, tokens);
      break;
    case ShellState::SingleQuote:
      if (chr == '\'') {
        state = ShellState::Normal;
      } else {
        current += chr;
      }
      break;
    case ShellState::DoubleQuote:
      state = handle_doublequote_char(cmd, idx, current);
      break;
    }
  }
  if (!current.empty()) {
    tokens.push_back(std::move(current));
  }
  return tokens;
}

// Read all bytes from a file descriptor into a string.
inline auto read_all(int file_fd) -> std::string {
  std::string result;
  // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
  std::array<char, 4096> buf{};
  for (;;) {
    ssize_t const num_bytes = ::read(file_fd, buf.data(), buf.size());
    if (num_bytes > 0) {
      result.append(buf.data(), static_cast<std::size_t>(num_bytes));
    } else if (num_bytes == 0) {
      break; // EOF
    } else {
      if (errno == EINTR) {
        continue; // Interrupted by signal; retry.
      }
      throw std::runtime_error(std::format("run_shell: read() failed with errno {}", errno));
    }
  }
  return result;
}

// Write all bytes of data to a file descriptor.
inline void write_all(int file_fd, std::string_view data) {
  while (!data.empty()) {
    ssize_t const num_bytes = ::write(file_fd, data.data(), data.size());
    if (num_bytes > 0) {
      data.remove_prefix(static_cast<std::size_t>(num_bytes));
    } else {
      if (errno == EINTR) {
        continue; // Interrupted by signal; retry.
      }
      throw std::runtime_error(std::format("run_shell: write() failed with errno {}", errno));
    }
  }
}

// Build an environment map from the parent environment plus overrides and extra paths.
inline auto build_env_map(std::map<std::string, std::string> const &extra_env,
                          std::vector<std::filesystem::path> const &extra_paths) -> std::map<std::string, std::string> {
  auto env_map = std::map<std::string, std::string>{};
  // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
  for (char *const *env_ptr = ::environ; env_ptr != nullptr && *env_ptr != nullptr; ++env_ptr) {
    std::string_view const entry{*env_ptr};
    auto const pos = entry.find('=');
    if (pos != std::string_view::npos) {
      env_map.emplace(std::string{entry.substr(0, pos)}, std::string{entry.substr(pos + 1)});
    }
  }
  for (auto const &[key, val] : extra_env) {
    env_map[key] = val;
  }
  if (!extra_paths.empty()) {
    std::string path_prefix;
    for (auto const &extra_path : extra_paths) {
      if (!path_prefix.empty()) {
        path_prefix += ':';
      }
      path_prefix += extra_path.string();
    }
    auto &path_val = env_map["PATH"];
    if (!path_val.empty()) {
      path_prefix += ':';
      path_prefix += path_val;
    }
    path_val = std::move(path_prefix);
  }
  return env_map;
}

// NOLINTEND(misc-use-internal-linkage)
} // namespace detail

export auto
run_shell(std::string const &cmd, std::string const &stdin_data = "",
          std::map<std::string, std::string> const &extra_env = {},
          std::vector<std::filesystem::path> const &extra_paths = {},
          std::filesystem::path const &cwd = std::filesystem::current_path(), bool check = true)
    -> std::tuple<int, std::string, std::string> {

  // --- Split command into argv ---
  auto const argv_strings = detail::shlex_split(cmd);
  if (argv_strings.empty()) {
    throw std::runtime_error("run_shell: empty command");
  }
  auto argv_ptrs = std::vector<char const *>{};
  argv_ptrs.reserve(argv_strings.size() + 1);
  for (auto const &str : argv_strings) {
    argv_ptrs.push_back(str.c_str());
  }
  argv_ptrs.push_back(nullptr);

  // --- Build environment ---
  auto const env_map = detail::build_env_map(extra_env, extra_paths);
  auto env_strings = std::vector<std::string>{};
  env_strings.reserve(env_map.size());
  for (auto const &[key, val] : env_map) {
    env_strings.push_back(std::format("{}={}", key, val));
  }
  auto envp_ptrs = std::vector<char const *>{};
  envp_ptrs.reserve(env_strings.size() + 1);
  for (auto const &str : env_strings) {
    envp_ptrs.push_back(str.c_str());
  }
  envp_ptrs.push_back(nullptr);

  // --- Create pipes (O_CLOEXEC prevents fd leaks into unrelated children) ---
  // [0] = read end, [1] = write end
  // Each Pipe is a RAII wrapper: if any pipe2() call throws, all previously created pipes are
  // closed automatically by the destructors of the already-constructed Pipe objects.
  auto stdin_pipe = detail::Pipe::create();
  auto stdout_pipe = detail::Pipe::create();
  auto stderr_pipe = detail::Pipe::create();

  // --- Fork ---
  pid_t const pid = ::fork();
  if (pid < 0) {
    throw std::runtime_error("run_shell: fork() failed");
    // stdin_pipe, stdout_pipe, stderr_pipe destructors close all fds.
  }

  if (pid == 0) {
    // Child process: wire up stdin/stdout/stderr, then exec.
    // dup2 clears O_CLOEXEC on the target fd, so child's stdio fds survive exec.
    // The original pipe fds retain O_CLOEXEC and are closed automatically by execvpe.
    if (::dup2(stdin_pipe.read_end(), STDIN_FILENO) < 0) {
      ::_exit(detail::EXIT_CODE_DUP2_FAILED);
    }
    if (::dup2(stdout_pipe.write_end(), STDOUT_FILENO) < 0) {
      ::_exit(detail::EXIT_CODE_DUP2_FAILED);
    }
    if (::dup2(stderr_pipe.write_end(), STDERR_FILENO) < 0) {
      ::_exit(detail::EXIT_CODE_DUP2_FAILED);
    }

    if (!cwd.empty() && ::chdir(cwd.c_str()) != 0) {
      ::_exit(detail::EXIT_CODE_CHDIR_FAILED);
    }

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-const-cast)
    ::execvpe(argv_ptrs[0], const_cast<char *const *>(argv_ptrs.data()),
              // NOLINTNEXTLINE(cppcoreguidelines-pro-type-const-cast)
              const_cast<char *const *>(envp_ptrs.data()));
    ::_exit(detail::EXIT_CODE_EXEC_FAILED);
  }

  // Parent: close the child-side ends (our copies; child's copies are in stdin/stdout/stderr).
  stdin_pipe.close_end(0);
  stdout_pipe.close_end(1);
  stderr_pipe.close_end(1);

  // This initially was written using std::thread, spawning read and writes into separate threads
  // to avoid potential locks.
  // However, current version of GCC (gcc 15) have bugs, which do not play well with combination
  // of C++20 modules and std::thread.
  // gcc in that context failed with internal compiler error.
  // Specifically, reproduction could be reached with these commands:
  // ```cpp
  // // utils.cppm
  // export module utils;
  // export import :shell;
  // ```
  //
  // ```cpp
  // // shell.cppm
  // module;
  // #include <thread>
  // export module utils:shell;
  // export void run_shell() {
  //   auto stdout_thread = std::thread([] -> void {});
  // }
  // ```
  // ```bash
  // g++ -std=c++23 -fmodules-ts -x c++ -c shell.cppm -o shell.o
  // g++ -std=c++23 -fmodules-ts -x c++ -c utils.cppm -o utils.o
  // ```
  // The error:
  // ```
  // utils.cppm:1:8: internal compiler error: Segmentation fault
  //     1 | export module utils;
  //       |        ^~~~~~
  // ```
  // The error seems to be related to this resolved issue:
  // https://gcc.gnu.org/bugzilla/show_bug.cgi?id=103701
  // Indeed, compilation of reduced example with trunk gcc succeeds.

  // Read stdout & stderr in threads.
  detail::write_all(stdin_pipe.write_end(), stdin_data);
  stdin_pipe.close_end(1);

  auto stdout_result = detail::read_all(stdout_pipe.read_end());
  stdout_pipe.close_end(0);

  auto stderr_result = detail::read_all(stderr_pipe.read_end());
  stderr_pipe.close_end(0);

  // Wait for child.
  int wstatus = 0;
  for (pid_t wait_result = ::waitpid(pid, &wstatus, 0); wait_result < 0; wait_result = ::waitpid(pid, &wstatus, 0)) {
    if (errno != EINTR) {
      throw std::runtime_error(std::format("run_shell: waitpid() failed with errno {}", errno));
    }
  }
  // NOLINTNEXTLINE(misc-include-cleaner)
  int const exit_code = WIFEXITED(wstatus) ? WEXITSTATUS(wstatus) : -1;

  if (check && exit_code != 0) {
    throw std::runtime_error("Command failed with exit code " + std::to_string(exit_code));
  }

  return std::make_tuple(exit_code, std::move(stdout_result), std::move(stderr_result));
}

} // namespace cpp_contests
