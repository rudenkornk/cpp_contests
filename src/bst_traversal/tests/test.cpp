#define BOOST_TEST_MODULE BstTraversal

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
        6
        5 3 2 3 5 6
    )"};

  auto const [exit_code, stdout_str, stderr_str] = run_shell(TestArgsFixture::cli_tools.at("bst_traversal_cli"), input);
  BOOST_TEST_CONTEXT(stderr_str) { BOOST_TEST(exit_code == 0); }

  auto const expected = std::array{2, 3, 3, 6, 5, 5, 2, 3, 3, 5, 5, 6};
  auto output = std::istringstream{stdout_str};
  for (auto value : expected) {
    int answer = 0;
    BOOST_REQUIRE(static_cast<bool>(output >> answer));
    BOOST_TEST(answer == value);
  }
  int extra = 0;
  BOOST_TEST(!(output >> extra));
}

} // namespace
