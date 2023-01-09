#include <cassert>
#include <fstream>
#include <iostream>
#include <stack>
#include <string>

class Solution {
public:
  bool isValid(std::string s) {
    auto p_stack = std::stack<char>{};
    for (auto c : s) {
      switch (c) {
      case '(':
      case '[':
      case '{':
        p_stack.push(c);
        break;
      case ')':
        if (p_stack.empty())
          return false;
        if (p_stack.top() != '(')
          return false;
        p_stack.pop();
        break;
      case ']':
        if (p_stack.empty())
          return false;
        if (p_stack.top() != '[')
          return false;
        p_stack.pop();
        break;
      case '}':
        if (p_stack.empty())
          return false;
        if (p_stack.top() != '{')
          return false;
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

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cerr << "Provide exactly one file input\n";
    return 1;
  }
  std::string filename = argv[1];
  auto input = std::ifstream{filename};
  if (!input.good()) {
    std::cerr << "Error opening '" << filename << "'\n";
    return 1;
  }
  std::string parens{};
  input >> parens;
  std::cout << Solution{}.isValid(parens);
  return 0;
}
