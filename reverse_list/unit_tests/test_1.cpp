#define BOOST_TEST_MODULE Test
#define _CRT_SECURE_NO_WARNINGS

#include <cassert>
#include <forward_list>
#include <utility>
#include <vector>

#include <boost/test/included/unit_test.hpp>

#include "solution.hpp"

void test_reverse_list(std::initializer_list<int> init) {
  // Use vector as container to avoid memory leaks
  std::vector<list_node<int>> container;
  size_t n = init.size();
  container.reserve(n);

  list_node<int> *head = nullptr;
  for (auto it = std::rbegin(init); it != std::rend(init); ++it) {
    container.emplace_back(*it, head);
    head = &container.back();
  }
  std::forward_list<int> reference_list(init);
  reference_list.reverse();
  head = reverse_list(head);

  auto *it = head;
  auto reference_it = reference_list.begin();
  while (n-- != 0) {
    assert(it->data_ == *reference_it);
    it = it->next_;
    ++reference_it;
  }
}

BOOST_AUTO_TEST_CASE(main_test) {
  test_reverse_list({});
  test_reverse_list({0});
  test_reverse_list({0, 1, 2, 3, 4});
}
