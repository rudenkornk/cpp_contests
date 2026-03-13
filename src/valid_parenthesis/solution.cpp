#include <cassert>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <span>
#include <stack>
#include <string>

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

auto main(int argc, char **argv) -> int {
  auto const args = std::span(argv, static_cast<std::size_t>(argc));
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
  std::cout << Solution::isValid(parens);
  return 0;
}
