module;

// This module deliberately does NOT use `import std;`. The Boost.Test headers are not modularized and must be included
// textually into the global module fragment; under Clang, Boost.Test's textual `<map>` (and friends) then conflict with
// the same types re-declared by `import std` ("cannot befriend target of using declaration"). Keeping the standard
// headers textual too sidesteps the clash. The rest of the project still uses `import std;`.
#include <cstddef>
#include <map>
#include <span>
#include <string>

#include <boost/test/framework.hpp>
#include <boost/test/unit_test.hpp>

export module utils:test_utils;

export namespace cpp_contests {

struct TestArgsFixture {
  static inline std::map<std::string, std::string> cli_tools;

  TestArgsFixture() {
    auto argc = boost::unit_test::framework::master_test_suite().argc;
    auto *argv = boost::unit_test::framework::master_test_suite().argv;
    auto const args = std::span(argv, static_cast<std::size_t>(argc));

    for (int i = 1; i < argc; ++i) {
      std::string const arg = args[i];
      auto pos = arg.find('=');
      if (arg.starts_with("--") && pos != std::string::npos) {
        std::string const key = arg.substr(2, pos - 2);
        std::string const value = arg.substr(pos + 1);
        cli_tools[key] = value;
      }
    }
  }
};

} // namespace cpp_contests
