#define BOOST_TEST_MODULE Ali Baba

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
    1 3
    3 1
    5 8
    8 19
    10 15
  )"};

  auto const [exit_code, stdout_str, stderr_str] = run_shell(TestArgsFixture::cli_tools.at("ali_baba_cli"), input);
  BOOST_TEST_CONTEXT(stderr_str) { BOOST_TEST(exit_code == 0); }
  BOOST_TEST(stdout_str == "11\n");
}

} // namespace
