#define BOOST_TEST_MODULE MinimalCoveringCircle
#define _CRT_SECURE_NO_WARNINGS // NOLINT(bugprone-reserved-identifier,cert-dcl37-c,cert-dcl51-cpp,readability-identifier-naming)

#include <algorithm>
#include <cctype>
#include <cmath>
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
auto run_minimal_covering_circle(std::string const& input) -> std::string {
  auto [exit_code, stdout_str, stderr_str] =
      run_shell(TestArgsFixture::cli_tools.at("minimal_covering_circle_cli"), input);
  auto is_not_space = [](unsigned char chr) -> bool { return std::isspace(chr) == 0; };
  auto last_nonspace = std::ranges::find_if(std::ranges::reverse_view(stdout_str), is_not_space);
  stdout_str.erase(last_nonspace.base(), stdout_str.end());
  return stdout_str;
}

BOOST_AUTO_TEST_CASE(CliIntegrationTests) {
  constexpr double tolerance = 1e-3;
  auto const* first_input = R"(
    3 3
    0 5
    3 4
    -4 -3
  )";
  BOOST_TEST(run_minimal_covering_circle(first_input) == "5.000000");

  auto const* second_input = R"(
    3 2
    0 1
    2 1
    1 100
  )";
  BOOST_TEST(std::abs(std::stod(run_minimal_covering_circle(second_input)) - std::stod("1.414246")) < tolerance);
}
} // namespace
