module;

#include <cassert>
#include <cstddef>
#include <exception>
#include <fstream>
#include <iostream>
#include <span>
#include <stack>
#include <string>

export module valid_parenthesis;

export namespace cpp_contests {

class Solution {
public:
  static auto isValid(std::string const &seq) -> bool {
    auto p_stack = std::stack<char>{};
    for (auto chr : seq) {
      switch (chr) {
      case '(':
      case '[':
      case '{':
        p_stack.push(chr);
        break;
      case ')':
        if (p_stack.empty()) {
          return false;
        }
        if (p_stack.top() != '(') {
          return false;
        }
        p_stack.pop();
        break;
      case ']':
        if (p_stack.empty()) {
          return false;
        }
        if (p_stack.top() != '[') {
          return false;
        }
        p_stack.pop();
        break;
      case '}':
        if (p_stack.empty()) {
          return false;
        }
        if (p_stack.top() != '{') {
          return false;
        }
        p_stack.pop();
        break;
      default:
        assert(false);
        break;
      }
    }
    return p_stack.empty();
  }
};
} // namespace cpp_contests

namespace {
auto run(std::span<char *> args) -> int {
  if (args.size() < 2) {
    std::cerr << "Provide exactly one file input\n";
    return 1;
  }
  std::string const filename = args[1];

  auto input = std::ifstream{filename};
  if (!input.good()) {
    std::cerr << "Error opening '" << filename << "'\n";
    return 1;
  }
  std::string parens{};
  input >> parens;
  std::cout << cpp_contests::Solution::isValid(parens);
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
