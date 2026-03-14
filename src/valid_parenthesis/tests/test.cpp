#define BOOST_TEST_MODULE Matrix // NOLINT(cppcoreguidelines-macro-usage)
#define _CRT_SECURE_NO_WARNINGS  // NOLINT(bugprone-reserved-identifier,cert-dcl37-c,cert-dcl51-cpp)

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <functional>
#include <sstream>
#include <string>

#include <boost/test/included/unit_test.hpp> // NOLINT(misc-include-cleaner)
#include <boost/test/tools/interface.hpp>
#include <boost/test/unit_test_suite.hpp>

#include "utils/test_utils.hpp"
#include "utils/utils.hpp"

using cpp_contests::run_shell;
using cpp_contests::TestArgsFixture;

BOOST_GLOBAL_FIXTURE(TestArgsFixture);

namespace {
auto run_cli_test(std::string const &file_content) -> bool {
  auto content_hash = std::hash<std::string>{}(file_content);
  auto temp_path =
      std::filesystem::temp_directory_path() / ("valid_parenthesis_test_" + std::to_string(content_hash) + ".txt");

  {
    std::ofstream temp_file(temp_path);
    temp_file << file_content;
  }

  auto [exit_code, stdout_str, stderr_str] =
      run_shell(TestArgsFixture::cli_tools.at("valid_parenthesis_cli") + " " + temp_path.string());

  // Clean up temporary file
  std::filesystem::remove(temp_path);

  std::istringstream iss(stdout_str);
  int result = 0;
  iss >> result;
  return result != 0;
}
} // namespace

BOOST_AUTO_TEST_CASE(cli_integration_tests) {
  BOOST_TEST(run_cli_test("()") == true);
  BOOST_TEST(run_cli_test("()[]{}") == true);
  BOOST_TEST(run_cli_test("(]") == false);
  BOOST_TEST(run_cli_test("([)]") == false);
  BOOST_TEST(run_cli_test("{[]}") == true);
  BOOST_TEST(run_cli_test("") == true);
  BOOST_TEST(run_cli_test("(") == false);
  BOOST_TEST(run_cli_test(")") == false);
  BOOST_TEST(run_cli_test("((") == false);
  BOOST_TEST(run_cli_test("))") == false);
  BOOST_TEST(run_cli_test("()()") == true);
  BOOST_TEST(run_cli_test("(())") == true);
  BOOST_TEST(run_cli_test("(()())") == true);
}
