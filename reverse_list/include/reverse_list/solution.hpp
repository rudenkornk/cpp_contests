#pragma once

#include <cassert>
#include <climits>
#include <numeric>
#include <vector>

namespace cpp_contests {

template <typename T> struct list_node final {
  T data_;
  list_node *next_;
  list_node(T data,
            list_node *next) noexcept(std::is_nothrow_move_constructible_v<T>)
      : data_(std::move(data)), next_(next) {}
};

template <typename T> list_node<T> *reverse_list(list_node<T> *head) noexcept {
  list_node<T> *new_head = nullptr;
  while (head) {
    list_node<T> *next = head->next_;
    head->next_ = new_head;
    new_head = head;
    head = next;
  }
  return new_head;
}

} // namespace cpp_contests
