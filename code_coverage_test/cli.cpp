#include <iostream>

#include "solution.hpp"

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cerr << "Provide exactly one argument\n";
    return 1;
  }
  std::string branch = argv[1];
  if (branch == "1") {
    first_option();
  } else {
    second_option();
  }
  return 0;
}
