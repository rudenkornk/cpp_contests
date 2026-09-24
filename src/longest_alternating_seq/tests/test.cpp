#define BOOST_TEST_MODULE LongestAlternatingSeq
#define _CRT_SECURE_NO_WARNINGS // NOLINT(bugprone-reserved-identifier,cert-dcl37-c,cert-dcl51-cpp,readability-identifier-naming)

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
auto run_longest_alternating_seq(std::string const &input) -> std::string {
  auto [exit_code, stdout_str, stderr_str] =
      run_shell(TestArgsFixture::cli_tools.at("longest_alternating_seq_cli"), input);
  auto is_not_space = [](unsigned char chr) -> bool { return std::isspace(chr) == 0; };
  auto last_nonspace = std::ranges::find_if(std::ranges::reverse_view(stdout_str), is_not_space);
  stdout_str.erase(last_nonspace.base(), stdout_str.end());
  return stdout_str;
}

BOOST_AUTO_TEST_CASE(CliIntegrationTests) {
  auto const *input = R"(
    10
    1 4 2 3 5 8 6 7 9 10
  )";
  BOOST_TEST(run_longest_alternating_seq(input) == "1 4 2 8 6 7");
}
} // namespace
