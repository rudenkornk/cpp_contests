#define BOOST_TEST_MODULE CountSearchTrees

#include <sstream>
#include <string>

#include <boost/test/included/unit_test.hpp>

import utils;

using cpp_contests::run_shell;
using cpp_contests::TestArgsFixture;

BOOST_GLOBAL_FIXTURE(TestArgsFixture);

namespace {

BOOST_AUTO_TEST_CASE(Example) {
  auto const input = std::string{R"(
    3
    1 2 3
  )"};

  auto const [exit_code, stdout_str, stderr_str] =
      run_shell(TestArgsFixture::cli_tools.at("count_search_trees_cli"), input);
  BOOST_TEST_CONTEXT(stderr_str) { BOOST_TEST(exit_code == 0); }

  auto output = std::istringstream{stdout_str};
  int answer = 0;
  BOOST_REQUIRE(static_cast<bool>(output >> answer));
  BOOST_TEST(answer == 5);
  int extra = 0;
  BOOST_TEST(!(output >> extra));
}

} // namespace
