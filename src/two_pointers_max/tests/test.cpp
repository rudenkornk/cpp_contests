#define BOOST_TEST_MODULE TwoPointersMax

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
    5
    1 3 2 5 4
    6
    R R L R L L
  )"};

  auto const [exit_code, stdout_str, stderr_str] =
      run_shell(TestArgsFixture::cli_tools.at("two_pointers_max_cli"), input);
  BOOST_TEST_CONTEXT(stderr_str) { BOOST_TEST(exit_code == 0); }

  auto output = std::istringstream{stdout_str};
  auto const expected = std::array{3, 3, 3, 5, 5, 5};
  for (auto const answer_expected : expected) {
    int answer = 0;
    BOOST_REQUIRE(static_cast<bool>(output >> answer));
    BOOST_TEST(answer == answer_expected);
  }
  int extra = 0;
  BOOST_TEST(!(output >> extra));
}
} // namespace
