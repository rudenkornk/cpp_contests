#define BOOST_TEST_MODULE MemoryManager

#include <array>
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
    6 8
    2
    3
    -1
    3
    3
    -5
    2
    2
  )"};

  auto const [exit_code, stdout_str, stderr_str] =
      run_shell(TestArgsFixture::cli_tools.at("memory_manager_cli"), input);
  BOOST_TEST_CONTEXT(stderr_str) { BOOST_TEST(exit_code == 0); }

  auto const expected = std::array{1, 3, -1, -1, 1, -1};
  auto output = std::istringstream{stdout_str};
  for (auto answer : expected) {
    int actual = 0;
    BOOST_REQUIRE(static_cast<bool>(output >> actual));
    BOOST_TEST(actual == answer);
  }
  auto extra = std::string{};
  BOOST_TEST(!(output >> extra));
}

} // namespace
