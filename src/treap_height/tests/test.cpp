#define BOOST_TEST_MODULE TreapHeight

#include <cmath>
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
    3 2
  )"};

  auto const [exit_code, stdout_str, stderr_str] = run_shell(TestArgsFixture::cli_tools.at("treap_height_cli"), input);
  BOOST_TEST_CONTEXT(stderr_str) { BOOST_TEST(exit_code == 0); }

  auto output = std::istringstream{stdout_str};
  double answer = 0;
  BOOST_REQUIRE(static_cast<bool>(output >> answer));
  auto constexpr expected = 0.666667;
  auto constexpr tolerance = 1e-6;
  BOOST_TEST(std::abs(answer - expected) <= tolerance);
  std::string extra;
  BOOST_TEST(!(output >> extra));
}

} // namespace
