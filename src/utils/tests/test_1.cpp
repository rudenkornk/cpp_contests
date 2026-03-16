#define BOOST_TEST_MODULE Matrix // NOLINT(cppcoreguidelines-macro-usage)
#define _CRT_SECURE_NO_WARNINGS  // NOLINT(bugprone-reserved-identifier,cert-dcl37-c,cert-dcl51-cpp)

#include <cstddef>
#include <cstdlib>
#include <stdexcept>

#include <boost/test/included/unit_test.hpp>
#include <boost/test/tools/interface.hpp>
#include <boost/test/unit_test_suite.hpp>

import utils.generic;
import utils.math;
import utils.shell;

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
    // NOLINTNEXTLINE(cppcoreguidelines-init-variables)
    double const res = cpp_contests::sqrt(curr * curr);
    double const diff = res - curr;
    result &= std::abs(diff) < eps;
  }
  return result;
}

} // namespace

BOOST_AUTO_TEST_CASE(sqrt_test) { static_assert(sqrt_check()); }

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)

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

// NOLINTEND(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
