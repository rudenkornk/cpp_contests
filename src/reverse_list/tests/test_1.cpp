#define BOOST_TEST_MODULE Matrix // NOLINT(cppcoreguidelines-macro-usage)
#define _CRT_SECURE_NO_WARNINGS  // NOLINT(bugprone-reserved-identifier,cert-dcl37-c,cert-dcl51-cpp)

#include <cassert>
#include <cstddef>
#include <forward_list>
#include <initializer_list>
#include <ranges>
#include <vector>

#include <boost/test/included/unit_test.hpp> // NOLINT(misc-include-cleaner)
#include <boost/test/unit_test_suite.hpp>

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers,readability-identifier-length,readability-magic-numbers)
#include "reverse_list/solution.hpp"

using cpp_contests::list_node;
using cpp_contests::reverse_list;

namespace {
void test_reverse_list(std::initializer_list<int> init) {
  // Use vector as container to avoid memory leaks
  std::vector<list_node<int>> container;
  std::size_t n = init.size();
  container.reserve(n);

  list_node<int> *head = nullptr;
  for (int const it : std::ranges::reverse_view(init)) {
    container.emplace_back(it, head);
    head = &container.back();
  }
  std::forward_list<int> reference_list(init);
  reference_list.reverse();
  head = reverse_list(head);

  auto *it = head;
  auto reference_it = reference_list.begin();
  while (n-- != 0 && it != nullptr) {
    assert(it->data_ == *reference_it);
    it = it->next_;
    ++reference_it;
  }
}
} // namespace

BOOST_AUTO_TEST_CASE(main_test) {
  test_reverse_list({});
  test_reverse_list({0});
  test_reverse_list({0, 1, 2, 3, 4});
}

// NOLINTEND(cppcoreguidelines-avoid-magic-numbers,readability-identifier-length,readability-magic-numbers)
