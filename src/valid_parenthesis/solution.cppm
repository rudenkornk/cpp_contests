module;

#include <cassert>
#include <cstddef>
#include <exception>
#include <fstream>
#include <iostream>
#include <print>
#include <span>
#include <stack>
#include <string>

export module valid_parenthesis;

export namespace cpp_contests {

class Solution {
public:
  static auto is_valid(std::string const &seq) -> bool {
    auto p_stack = std::stack<char>{};
    for (auto chr : seq) {
      if (chr == '(' || chr == '[' || chr == '{') {
        p_stack.push(chr);
        continue;
      }
      assert(chr == ')' || chr == ']' || chr == '}');
      if (p_stack.empty()) {
        return false;
      }
      auto top = p_stack.top();
      if ((chr == ')' && top != '(') || (chr == ']' && top != '[') || (chr == '}' && top != '{')) {
        return false;
      }
      p_stack.pop();
    }
    return p_stack.empty();
  }
};
} // namespace cpp_contests

namespace {
auto run(std::span<char *> args) -> int {
  if (args.size() < 2) {
    std::println(std::cerr, "Provide exactly one file input");
    return 1;
  }
  std::string const filename = args[1];

  auto input = std::ifstream{filename};
  if (!input.good()) {
    std::println(std::cerr, "Error opening '{}'", filename);
    return 1;
  }
  std::string parens{};
  input >> parens;
  std::println("{:d}", cpp_contests::Solution::is_valid(parens));
  return 0;
}
} // namespace

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
extern "C++" auto main(int argc, char **argv) -> int {
  try {
    return run(std::span(argv, static_cast<std::size_t>(argc)));
  } catch (std::exception const &e) {
    std::cerr << e.what() << "\n";
    return 1;
  }
}
#pragma GCC diagnostic pop
