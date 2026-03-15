module;

#include <cassert>
#include <climits>
#include <type_traits>
#include <utility>

export module reverse_list.solution;

export namespace cpp_contests {

template <typename T> struct list_node final {
  T data_;          // NOLINT(misc-non-private-member-variables-in-classes)
  list_node *next_; // NOLINT(misc-non-private-member-variables-in-classes)
  list_node(T data, list_node *next) noexcept(std::is_nothrow_move_constructible_v<T>)
      : data_(std::move(data)), next_(next) {}
};

template <typename T> auto reverse_list(list_node<T> *head) noexcept -> list_node<T> * {
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
