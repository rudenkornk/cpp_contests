#define BOOST_TEST_MODULE SlidingKStatistic

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
    4 6 1
    1 2 3 4
    RLRLRL
  )"};

  auto const [exit_code, stdout_str, stderr_str] =
      run_shell(TestArgsFixture::cli_tools.at("sliding_k_statistic_cli"), input);
  BOOST_TEST_CONTEXT(stderr_str) { BOOST_TEST(exit_code == 0); }

  auto output = std::istringstream{stdout_str};
  for (auto const expected : {1, 2, 2, 3, 3, 4}) {
    int answer = 0;
    BOOST_REQUIRE(static_cast<bool>(output >> answer));
    BOOST_TEST(answer == expected);
  }
  int extra = 0;
  BOOST_TEST(!(output >> extra));
}

} // namespace
