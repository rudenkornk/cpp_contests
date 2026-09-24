module;

#include <cassert>
#include <climits>
#include <type_traits>
#include <utility>

export module reverse_list;

export namespace cpp_contests {

template <typename T> struct ListNode final {
  T data;         // NOLINT(misc-non-private-member-variables-in-classes)
  ListNode *next; // NOLINT(misc-non-private-member-variables-in-classes)
  ListNode(T data, ListNode *next) noexcept(std::is_nothrow_move_constructible_v<T>)
      : data(std::move(data)), next(next) {}
};

template <typename T> auto reverse_list(ListNode<T> *head) noexcept -> ListNode<T> * {
  ListNode<T> *new_head = nullptr;
  while (head) {
    ListNode<T> *const next = head->next;
    head->next = new_head;
    new_head = head;
    head = next;
  }
  return new_head;
}

} // namespace cpp_contests
