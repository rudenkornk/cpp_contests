#define BOOST_TEST_MODULE Matrix // NOLINT(cppcoreguidelines-macro-usage)
#define _CRT_SECURE_NO_WARNINGS  // NOLINT(bugprone-reserved-identifier,cert-dcl37-c,cert-dcl51-cpp)

#include <cassert>
#include <filesystem>
#include <fstream>
#include <functional>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include <boost/test/included/unit_test.hpp> // NOLINT(misc-include-cleaner)
#include <boost/test/tools/interface.hpp>
#include <boost/test/unit_test_suite.hpp>

#include "missing_numbers/solution.hpp"
#include "utils/test_utils.hpp"
#include "utils/utils.hpp"

using cpp_contests::missing_numbers;
using cpp_contests::run_shell;
using cpp_contests::TestArgsFixture;

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers,readability-identifier-length,readability-magic-numbers)

BOOST_GLOBAL_FIXTURE(TestArgsFixture);

BOOST_AUTO_TEST_CASE(library_function_tests) {
  BOOST_TEST((std::pair(0U, 1U) == missing_numbers({})));
  BOOST_TEST((std::pair(0U, 1U) == missing_numbers({2})));
  BOOST_TEST((std::pair(0U, 1U) == missing_numbers({3, 2})));
  BOOST_TEST((std::pair(0U, 1U) == missing_numbers({4, 3, 2})));
  BOOST_TEST((std::pair(2U, 6U) == missing_numbers({7, 4, 1, 0, 3, 5})));
  BOOST_TEST((std::pair(2U, 5U) == missing_numbers({7, 4, 1, 0, 3, 6})));
  BOOST_TEST((std::pair(2U, 5U) == missing_numbers({7, 4, 1, 0, 3, 6, 9, 11, 10, 8})));
}

namespace {
auto run_cli_test(std::string const &file_content) -> std::pair<unsigned, unsigned> {
  auto content_hash = std::hash<std::string>{}(file_content);
  auto temp_path =
      std::filesystem::temp_directory_path() / ("missing_numbers_test_" + std::to_string(content_hash) + ".txt");

  {
    std::ofstream temp_file(temp_path);
    temp_file << file_content;
  }

  auto [exit_code, stdout_str, stderr_str] =
      run_shell(TestArgsFixture::cli_tools.at("missing_numbers_cli") + " " + temp_path.string());

  // Clean up temporary file
  std::filesystem::remove(temp_path);

  std::istringstream iss(stdout_str);
  unsigned first = 0;
  unsigned second = 0;
  iss >> first >> second;
  return {first, second};
}
} // namespace

BOOST_AUTO_TEST_CASE(cli_integration_tests) {
  BOOST_TEST((std::pair(0U, 1U) == run_cli_test("")));
  BOOST_TEST((std::pair(2U, 5U) == run_cli_test("7 4 1 0 3 6")));
}

// NOLINTEND(cppcoreguidelines-avoid-magic-numbers,readability-identifier-length,readability-magic-numbers)
