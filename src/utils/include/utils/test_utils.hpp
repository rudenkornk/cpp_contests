#pragma once

#include <map>
#include <string>

#include <boost/test/unit_test.hpp>

namespace cpp_contests {

struct TestArgsFixture {
  static inline std::map<std::string, std::string> cli_tools;

  TestArgsFixture() {
    auto argc = boost::unit_test::framework::master_test_suite().argc;
    auto argv = boost::unit_test::framework::master_test_suite().argv;

    for (int i = 1; i < argc; ++i) {
      std::string arg = argv[i];
      auto pos = arg.find('=');
      if (arg.rfind("--", 0) == 0 && pos != std::string::npos) {
        std::string key = arg.substr(2, pos - 2);
        std::string value = arg.substr(pos + 1);
        cli_tools[key] = value;
      }
    }
  }
};

} // namespace cpp_contests
