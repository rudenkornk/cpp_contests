#define BOOST_TEST_MODULE Postman

#include <cstdint>
#include <sstream>
#include <string>

#include <boost/test/included/unit_test.hpp>

import utils;

using cpp_contests::run_shell;
using cpp_contests::TestArgsFixture;

BOOST_GLOBAL_FIXTURE(TestArgsFixture);

namespace {

BOOST_AUTO_TEST_CASE(Example) {
  auto const input = std::string{"6\n239 13\n"};

  auto const [exit_code, stdout_str, stderr_str] = run_shell(TestArgsFixture::cli_tools.at("postman_cli"), input);
  BOOST_TEST_CONTEXT(stderr_str) { BOOST_TEST(exit_code == 0); }

  auto output = std::istringstream{stdout_str};
  auto answer = std::uint64_t{};
  BOOST_REQUIRE(static_cast<bool>(output >> answer));
  BOOST_TEST(answer == 8510257371ULL);
  std::string extra;
  BOOST_TEST(!(output >> extra));
}

} // namespace
