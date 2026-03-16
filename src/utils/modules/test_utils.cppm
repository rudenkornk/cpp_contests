module;

#include <cstddef>
#include <map>
#include <span>
#include <string>

#include <boost/test/framework.hpp>
#include <boost/test/unit_test.hpp>

export module utils:test_utils;

export namespace cpp_contests {

struct TestArgsFixture {
  // NOLINTNEXTLINE(fuchsia-statically-constructed-objects)
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
