// NOLINTBEGIN(cppcoreguidelines-pro-type-vararg)
// NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)

#define BOOST_TEST_MODULE Test  // NOLINT
#define _CRT_SECURE_NO_WARNINGS // NOLINT

#include <string>

#include <boost/process.hpp>
#include <boost/test/included/unit_test.hpp>

#include "lru_cache/solution.hpp"
#include "utils/test_utils.hpp"
#include "utils/utils.hpp"

using namespace cpp_contests;

BOOST_GLOBAL_FIXTURE(TestArgsFixture);

static int run_cli_test(std::string const &input) {
  auto &&[exit_code, stdout, stderr] =
      run_shell(TestArgsFixture::cli_tools.at("lru_cache_cli"), input);
  return std::stoi(stdout);
}

BOOST_AUTO_TEST_CASE(library_function_tests) {
  BOOST_TEST(lru_hits({}, 0, 0) == 0);
  BOOST_TEST(lru_hits({1}, 0, 0) == 0);
  BOOST_TEST(lru_hits({1, 2, 3, 4, 5}, 0, 0) == 0);
  BOOST_TEST(lru_hits({1}, sizeof(int), 0) == 0);
  BOOST_TEST(lru_hits({1, 1, 1}, sizeof(int), 0) == 2);
  BOOST_TEST(lru_hits({1, 1, 2}, sizeof(int), 0) == 1);
  BOOST_TEST(lru_hits({1, 2, 1}, sizeof(int), 0) == 0);
  BOOST_TEST(lru_hits({1, 2, 3}, 2 * sizeof(int), 0) == 0);
  BOOST_TEST(lru_hits({1, 2, 1}, 2 * sizeof(int), 0) == 1);
  BOOST_TEST(lru_hits({1, 2, 3, 1, 2}, 2 * sizeof(int), 0) == 0);
  BOOST_TEST(lru_hits({1, 2, 3, 2, 3}, 2 * sizeof(int), 0) == 2);
  BOOST_TEST(lru_hits({1, 2, 3, 1, 2, 4, 5, 1, 7, 3, 2, 6, 1, 2},
                      4 * sizeof(int), 0) == 4);
}

BOOST_AUTO_TEST_CASE(cli_integration_tests) {
  BOOST_TEST(run_cli_test("0 0\n") == 0);
  BOOST_TEST(run_cli_test("0 1 1\n") == 0);
  BOOST_TEST(run_cli_test("0 5 1 2 3 4 5\n") == 0);
  BOOST_TEST(run_cli_test("1 1 1\n") == 0);
  BOOST_TEST(run_cli_test("1 3 1 1 1\n") == 2);
  BOOST_TEST(run_cli_test("1 3 1 1 2\n") == 1);
  BOOST_TEST(run_cli_test("1 3 1 2 1\n") == 0);
  BOOST_TEST(run_cli_test("2 3 1 2 3\n") == 0);
  BOOST_TEST(run_cli_test("2 3 1 2 1\n") == 1);
  BOOST_TEST(run_cli_test("2 4 1 2 3 1 2\n") == 0);
  BOOST_TEST(run_cli_test("2 5 1 2 3 2 3\n") == 2);
  BOOST_TEST(run_cli_test("4 14 1 2 3 1 2 4 5 1 7 3 2 6 1 2\n") == 4);
}

// NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)
// NOLINTEND(cppcoreguidelines-pro-type-vararg)
