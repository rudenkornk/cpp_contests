#define BOOST_TEST_MODULE Utils
#define _CRT_SECURE_NO_WARNINGS // NOLINT(bugprone-reserved-identifier,cert-dcl37-c,cert-dcl51-cpp)

#include <cstddef>
#include <cstdlib>
#include <format>
#include <limits>
#include <stdexcept>
#include <string>

#include <boost/test/included/unit_test.hpp>
#include <boost/test/tools/interface.hpp>
#include <boost/test/unit_test_suite.hpp>

import utils;

using cpp_contests::size_to_string;

BOOST_AUTO_TEST_CASE(size_to_string_test) {
  BOOST_TEST(size_to_string(std::size_t{0}) == "0 B");
  BOOST_TEST(size_to_string(std::size_t{1}) == "1 B");
  BOOST_TEST(size_to_string(std::size_t{1024}) == "1 KiB");
  BOOST_TEST(size_to_string(std::size_t{1024} * 1024) == "1 MiB");
  BOOST_TEST(size_to_string(std::size_t{1024} * 1024 * 1024) == "1 GiB");
  BOOST_TEST(size_to_string(std::size_t{1024} * 1024 * 1024 * 1024) == "1 TiB");
  BOOST_TEST(size_to_string(std::size_t{1024} * 1024 * 1024 * 1024 * 1024) == "1 PiB");
  BOOST_TEST(size_to_string(std::size_t{1024} * 1024 * 1024 * 1024 * 1024 * 1024) == "1 EiB");
  BOOST_TEST(size_to_string(std::size_t{17}) == "17 B");
  BOOST_TEST(size_to_string(std::size_t{1025}) == "1.0 KiB");
  BOOST_TEST(size_to_string(std::size_t{1129}) == "1.1 KiB");
}

namespace {
// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
constexpr auto pow2(double val, std::size_t n) -> double {
  double res = 1;
  for (std::size_t i = 0; i < n; ++i) {
    res *= val;
  }
  return res;
}

constexpr auto sqrt_check() -> bool {
  const double start = 1e-6;
  const double mult = 1.1;
  const double eps = 1e-7;
  const std::size_t N_iter = 300;
  bool result = true;
  for (std::size_t i = 0; i < N_iter; ++i) {
    double const curr = pow2(mult, i) * start;
    double const res = cpp_contests::sqrt(curr * curr);
    double const diff = res - curr;
    result &= std::abs(diff) < eps;
  }
  return result;
}

} // namespace

BOOST_AUTO_TEST_CASE(sqrt_test) { static_assert(sqrt_check()); }

BOOST_AUTO_TEST_CASE(sqrt_special_values) {
  constexpr double inf = std::numeric_limits<double>::infinity();
  constexpr double perfect_square = 4.0;
  constexpr double its_root = 2.0;
  static_assert(cpp_contests::sqrt(0.0) == 0.0);
  static_assert(cpp_contests::sqrt(perfect_square) == its_root);
  static_assert(cpp_contests::sqrt(inf) == inf);
  // NaN is the only value that is not equal to itself.
  // NOLINTNEXTLINE(misc-redundant-expression)
  static_assert(cpp_contests::sqrt(-1.0) != cpp_contests::sqrt(-1.0));
  // NOLINTNEXTLINE(misc-redundant-expression)
  static_assert(cpp_contests::sqrt(std::numeric_limits<double>::quiet_NaN()) !=
                cpp_contests::sqrt(std::numeric_limits<double>::quiet_NaN()));
  BOOST_TEST(cpp_contests::sqrt(perfect_square) == its_root);
}

BOOST_AUTO_TEST_CASE(run_shell_basic) {
  auto const &[exit_code, out, err] = cpp_contests::run_shell("echo hello");
  BOOST_TEST(exit_code == 0);
  BOOST_TEST(out == "hello\n");
  BOOST_TEST(err.empty());
}

BOOST_AUTO_TEST_CASE(run_shell_stdin) {
  auto const &[exit_code, out, err] = cpp_contests::run_shell("cat", "world\n");
  BOOST_TEST(exit_code == 0);
  BOOST_TEST(out == "world\n");
  BOOST_TEST(err.empty());
}

BOOST_AUTO_TEST_CASE(run_shell_exit_code) {
  auto const &[exit_code, out, err] = cpp_contests::run_shell("false", "", {}, {}, {}, false);
  BOOST_TEST(exit_code == 1);
  (void)out;
  (void)err;
}

BOOST_AUTO_TEST_CASE(run_shell_check_throws) {
  BOOST_CHECK_THROW(cpp_contests::run_shell("false"), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(run_shell_extra_env) {
  auto const &[exit_code, out, err] = cpp_contests::run_shell("sh -c 'echo $MY_VAR'", "", {{"MY_VAR", "hello_env"}});
  BOOST_TEST(exit_code == 0);
  BOOST_TEST(out == "hello_env\n");
  BOOST_TEST(err.empty());
}

BOOST_AUTO_TEST_CASE(run_shell_stderr) {
  auto const &[exit_code, out, err] = cpp_contests::run_shell("sh -c 'echo errline >&2'", "", {}, {}, {}, false);
  BOOST_TEST(exit_code == 0);
  BOOST_TEST(out.empty());
  BOOST_TEST(err == "errline\n");
}

BOOST_AUTO_TEST_CASE(run_shell_quoted_args) {
  auto const &[exit_code, out, err] = cpp_contests::run_shell("echo \"hello world\"");
  BOOST_TEST(exit_code == 0);
  BOOST_TEST(out == "hello world\n");
  BOOST_TEST(err.empty());
}

namespace {
// Well above the typical 64 KiB pipe capacity: stresses the poll()-based multiplexer.
constexpr std::size_t large_io_size = std::size_t{1} << 20U;
} // namespace

BOOST_AUTO_TEST_CASE(run_shell_large_roundtrip) {
  // A sequential write-then-read implementation deadlocks here: the child echoes data back
  // while run_shell is still feeding stdin.
  auto const input = std::string(large_io_size, 'x');
  auto const &[exit_code, out, err] = cpp_contests::run_shell("cat", input);
  BOOST_TEST(exit_code == 0);
  BOOST_TEST(out.size() == input.size());
  BOOST_TEST(err.empty());
}

BOOST_AUTO_TEST_CASE(run_shell_large_interleaved_output) {
  // Both streams exceed the pipe capacity: deadlocks unless stdout and stderr are drained concurrently.
  auto const command =
      std::format(R"(sh -c 'head -c {0} /dev/zero | tr "\0" a; head -c {0} /dev/zero | tr "\0" b >&2')", large_io_size);
  auto const &[exit_code, out, err] = cpp_contests::run_shell(command);
  BOOST_TEST(exit_code == 0);
  BOOST_TEST(out.size() == large_io_size);
  BOOST_TEST(err.size() == large_io_size);
  BOOST_TEST(out.front() == 'a');
  BOOST_TEST(err.front() == 'b');
}

BOOST_AUTO_TEST_CASE(run_shell_child_ignores_stdin) {
  // The child exits without reading: run_shell must survive EPIPE/SIGPIPE instead of dying.
  auto const input = std::string(large_io_size, 'x');
  auto const &[exit_code, out, err] = cpp_contests::run_shell("true", input);
  BOOST_TEST(exit_code == 0);
  BOOST_TEST(out.empty());
  BOOST_TEST(err.empty());
}

BOOST_AUTO_TEST_CASE(run_shell_signal_exit_code) {
  // Signal deaths follow the shell convention: 128 + signal number (SIGTERM == 15).
  auto const &[exit_code, out, err] = cpp_contests::run_shell("sh -c 'kill -TERM $$'", "", {}, {}, {}, false);
  BOOST_TEST(exit_code == 128 + 15);
  (void)out;
  (void)err;
}
