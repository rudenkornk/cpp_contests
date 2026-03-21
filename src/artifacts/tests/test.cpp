#define BOOST_TEST_MODULE Artifacts
#define _CRT_SECURE_NO_WARNINGS // NOLINT(bugprone-reserved-identifier,cert-dcl37-c,cert-dcl51-cpp)

#include <algorithm>
#include <cctype>
#include <ranges>
#include <string>

#include <boost/test/included/unit_test.hpp>
#include <boost/test/tools/interface.hpp>
#include <boost/test/unit_test_suite.hpp>

import utils;

using cpp_contests::run_shell;
using cpp_contests::TestArgsFixture;

BOOST_GLOBAL_FIXTURE(TestArgsFixture);

namespace {
auto run_artifacts(std::string const &input) -> std::string {
  auto [exit_code, stdout_str, stderr_str] = run_shell(TestArgsFixture::cli_tools.at("artifacts_cli"), input);
  // Trim trailing whitespace/newlines.
  auto is_not_space = [](unsigned char chr) -> bool { return std::isspace(chr) == 0; };
  auto last_nonspace = std::ranges::find_if(std::ranges::reverse_view(stdout_str), is_not_space);
  stdout_str.erase(last_nonspace.base(), stdout_str.end());
  return stdout_str;
}
} // namespace

BOOST_AUTO_TEST_CASE(cli_integration_tests) {
  std::string input;

  // Test 1: trivial single artifact.
  input = R"(
    1 1
    42
  )";
  BOOST_TEST(run_artifacts(input) == "42");

  // Test 2: single epoch, pick minimum value.
  input = R"(
    1 3
    9 3 1
  )";
  BOOST_TEST(run_artifacts(input) == "1");

  // Test 3: two epochs, m=1, forced selection.
  input = R"(
    2 1
    3
    7
  )";
  BOOST_TEST(run_artifacts(input) == "3 7");

  // Test 4: existing baseline.
  input = R"(
    3 2
    2 2
    6 7
    99 1
  )";
  BOOST_TEST(run_artifacts(input) == "1 2 6");

  // Test 5: n=3 m=2, optimal window is not the first one found during init.
  // Sorted: (1,e0),(2,e1),(3,e0),(3,e2),(4,e1),(5,e2)
  // Init window [0..3] score=2, but window [1..3] score=1 is better.
  // Expected: 2 3 3
  input = R"(
    3 2
    1 3
    2 4
    3 5
  )";
  BOOST_TEST(run_artifacts(input) == "2 3 3");

  // Test 6: n=2 m=2, optimal window is not the first one found during init.
  // Sorted: (1,e0),(49,e1),(50,e0),(100,e1)
  // Init window [0..1] score=48, but window [1..2] score=1 is better.
  // Expected: 49 50
  input = R"(
    2 2
    1 49
    50 100
  )";
  BOOST_TEST(run_artifacts(input) == "49 50");

  // Test 7: n=2 m=3, optimal window is not the first one found during init.
  // Sorted: (1,e1),(2,e1),(3,e1),(10,e0),(20,e0),(30,e0)
  // Init window [0..3] score=9, but window [2..3] score=7 is better.
  // Expected: 3 10
  input = R"(
    2 3
    10 20 30
    1 2 3
  )";
  BOOST_TEST(run_artifacts(input) == "3 10");

  // Test 8: n=3 m=3, optimal window is not the first one found during init.
  // Sorted: (1,e0),(2,e0),(3,e0),(10,e1),(11,e1),(12,e1),(100,e2),(101,e2),(102,e2)
  // Init window [0..6] score=99, but window [2..6] score=97 is better.
  // Expected: 3 10 100
  input = R"(
    3 3
    1 2 3
    10 11 12
    100 101 102
  )";
  BOOST_TEST(run_artifacts(input) == "3 10 100");
}
