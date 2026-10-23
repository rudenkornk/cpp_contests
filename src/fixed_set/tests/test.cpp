#define BOOST_TEST_MODULE FixedSet

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
    3
    1 2 3
    4
    1 2 3 4
  )"};

  auto const [exit_code, stdout_str, stderr_str] = run_shell(TestArgsFixture::cli_tools.at("fixed_set_cli"), input);
  BOOST_TEST_CONTEXT(stderr_str) { BOOST_TEST(exit_code == 0); }

  auto const expected = std::array{"Yes", "Yes", "Yes", "No"};
  auto output = std::istringstream{stdout_str};
  for (auto const *answer : expected) {
    auto actual = std::string{};
    BOOST_REQUIRE(static_cast<bool>(output >> actual));
    BOOST_TEST(actual == answer);
  }
  auto extra = std::string{};
  BOOST_TEST(!(output >> extra));
}

} // namespace
