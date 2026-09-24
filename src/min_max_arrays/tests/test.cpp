#define BOOST_TEST_MODULE MinMaxArrays

#include <array>
#include <sstream>
#include <string>
#include <utility>

#include <boost/test/included/unit_test.hpp>

import utils;

using cpp_contests::run_shell;
using cpp_contests::TestArgsFixture;

BOOST_GLOBAL_FIXTURE(TestArgsFixture);

BOOST_AUTO_TEST_CASE(Example) {
  auto const input = std::string{R"(
    4 3 5
    1 2 3 4 5
    1 1 1 1 1
    0 99999 99999 99999 99999
    0 0 0 0 99999
    5 4 3 2 1
    99999 99999 99999 0 0
    99999 99999 0 0 0
    12
    1 1
    1 2
    1 3
    2 1
    2 2
    2 3
    3 1
    3 2
    3 3
    4 1
    4 2
    4 3
  )"};

  auto const [exit_code, stdout_str, stderr_str] =
      run_shell(TestArgsFixture::cli_tools.at("min_max_arrays_cli"), input);
  BOOST_TEST(exit_code == 0);
  BOOST_TEST(stderr_str.empty());

  // Several queries admit multiple optimal positions.
  auto const valid_positions =
      std::array{std::pair{3, 3}, std::pair{4, 4}, std::pair{3, 3}, std::pair{5, 5}, std::pair{4, 5}, std::pair{3, 5},
                 std::pair{1, 1}, std::pair{1, 5}, std::pair{1, 5}, std::pair{4, 4}, std::pair{4, 4}, std::pair{3, 4}};
  auto output = std::istringstream{stdout_str};
  for (auto const &[first, last] : valid_positions) {
    int answer = 0;
    BOOST_REQUIRE(static_cast<bool>(output >> answer));
    BOOST_TEST(answer >= first);
    BOOST_TEST(answer <= last);
  }
  int extra = 0;
  BOOST_TEST(!(output >> extra));
}
