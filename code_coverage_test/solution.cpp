#include <iostream>

#include <solution.hpp>

void first_option() {
  volatile int x = 1;
  int result = 0;
  for (int i = 0; i != 1000; ++i) {
    result += x;
  }
  std::cout << result << "\n";
}

void second_option() {
  volatile int x = 1;
  int result = 0;
  for (int i = 0; i != 1000; ++i) {
    result += 2 * x;
  }
  std::cout << result << "\n";
}
