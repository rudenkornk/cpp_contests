#define BOOST_TEST_MODULE TreeIsomorphism

#include <algorithm>
#include <array>
#include <cstddef>
#include <sstream>
#include <string>
#include <utility>

#include <boost/test/included/unit_test.hpp>

import utils;

using cpp_contests::run_shell;
using cpp_contests::TestArgsFixture;

BOOST_GLOBAL_FIXTURE(TestArgsFixture);

namespace {

BOOST_AUTO_TEST_CASE(Example) {
  auto const input = std::string{R"(
    4
    3 1
    2 3
    4 3
    2 4
    4 1
    3 4
  )"};

  auto const [exit_code, stdout_str, stderr_str] =
      run_shell(TestArgsFixture::cli_tools.at("tree_isomorphism_cli"), input);
  BOOST_TEST_CONTEXT(stderr_str) { BOOST_TEST(exit_code == 0); }

  auto mapping = std::array<int, 4>{};
  auto output = std::istringstream{stdout_str};
  for (auto &vertex : mapping) {
    BOOST_REQUIRE(static_cast<bool>(output >> vertex));
    BOOST_REQUIRE(vertex >= 1);
    BOOST_REQUIRE(vertex <= 4);
  }
  auto sorted_mapping = mapping;
  std::ranges::sort(sorted_mapping);
  auto const expected_vertices = std::array{1, 2, 3, 4};
  BOOST_TEST(sorted_mapping == expected_vertices, boost::test_tools::per_element());

  auto const y_edges = std::array{std::pair{3, 1}, std::pair{2, 3}, std::pair{4, 3}};
  auto const x_edges = std::array{std::pair{2, 4}, std::pair{1, 4}, std::pair{3, 4}};
  for (auto const &[u, v] : y_edges) {
    auto const [a, b] =
        std::minmax(mapping.at(static_cast<std::size_t>(u - 1)), mapping.at(static_cast<std::size_t>(v - 1)));
    auto const found = std::ranges::find(x_edges, std::pair{a, b}) != x_edges.end();
    BOOST_TEST(found);
  }
  auto extra = std::string{};
  BOOST_TEST(!(output >> extra));
}

} // namespace
