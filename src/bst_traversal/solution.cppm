module;

#include <cassert>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <functional>
#include <iostream>
#include <limits>
#include <print>
#include <ranges>
#include <span>
#include <stack>
#include <utility>
#include <vector>

export module bst_traversal;

namespace cpp_contests {
namespace {

enum class Order : std::uint8_t { Pre, In, Post };

auto constexpr kNoChild = std::numeric_limits<std::size_t>::max();

// NOLINTBEGIN(misc-non-private-member-variables-in-classes)
struct Node {
  std::uint32_t value = std::numeric_limits<std::uint32_t>::max();
  std::size_t left = kNoChild;
  std::size_t right = kNoChild;

  auto operator==(Node const &other) const -> bool = default;
};

// Callbacks are reused as lvalues throughout each traversal.
// NOLINTBEGIN(cppcoreguidelines-missing-std-forward)
struct Tree {
  std::vector<Node> tree;

  template <Order TraversalOrder = Order::Pre, typename Fn>
    requires std::invocable<Fn &, Node &>
  auto recursive_walk(Fn &&callable, std::size_t idx = 0) -> void {
    assert(idx < tree.size());

    auto &&node = tree[idx];
    if constexpr (TraversalOrder == Order::Pre) {
      std::invoke(callable, node);
    }
    if (node.left != kNoChild) {
      recursive_walk<TraversalOrder>(std::forward<Fn>(callable), node.value);
    }
    if constexpr (TraversalOrder == Order::In) {
      std::invoke(callable, node);
    }
    if (node.right != kNoChild) {
      recursive_walk<TraversalOrder>(std::forward<Fn>(callable), node.value);
    }
    if constexpr (TraversalOrder == Order::Post) {
      std::invoke(callable, node);
    };
  }

  template <Order TraversalOrder = Order::Pre, typename Fn>
    requires std::invocable<Fn &, Node &>
  auto walk(Fn &&callable) -> void;

  template <Order TraversalOrder, typename Fn>
    requires std::invocable<Fn &, Node &> && (TraversalOrder == Order::Pre)
  auto walk(Fn &&callable) -> void {
    auto stack = std::stack<std::size_t>{};
    stack.push(0);

    while (!stack.empty()) {
      auto node_idx = stack.top();
      stack.pop();

      auto &&node = tree[node_idx];
      std::invoke(callable, node);
      if (node.right != kNoChild) {
        stack.push(node.right);
      }
      if (node.left != kNoChild) {
        stack.push(node.left);
      }
    }
  }

  template <Order TraversalOrder, typename Fn>
    requires std::invocable<Fn &, Node &> && (TraversalOrder == Order::In)
  auto walk(Fn &&callable) -> void {
    auto stack = std::stack<std::size_t>{};
    std::size_t current_idx = 0;

    while (current_idx != kNoChild || !stack.empty()) {
      if (current_idx != kNoChild) {
        stack.push(current_idx);
        current_idx = tree[current_idx].left;
        continue;
      }
      auto top = stack.top();
      stack.pop();
      std::invoke(callable, tree[top]);
      current_idx = tree[top].right;
    }
  }

  template <Order TraversalOrder, typename Fn>
    requires std::invocable<Fn &, Node &> && (TraversalOrder == Order::Post)
  auto walk(Fn &&callable) -> void {
    auto stack = std::stack<std::size_t>{};
    auto reversed_idx = std::vector<std::size_t>();
    reversed_idx.reserve(tree.size());
    stack.push(0);

    while (!stack.empty()) {
      auto node_idx = stack.top();
      stack.pop();
      auto &&node = tree[node_idx];
      reversed_idx.push_back(node_idx);
      if (node.left != kNoChild) {
        stack.push(node.left);
      }
      if (node.right != kNoChild) {
        stack.push(node.right);
      }
    }

    for (auto &&idx : std::views::reverse(reversed_idx)) {
      std::invoke(callable, tree[idx]);
    }
  }
};
// NOLINTEND(cppcoreguidelines-missing-std-forward)
// NOLINTEND(misc-non-private-member-variables-in-classes)

// NOLINTNEXTLINE(readability-function-size)
auto construct(std::vector<uint32_t> const &arr) -> Tree {
  auto raw = std::vector<Node>();
  raw.reserve(arr.size());

  auto stack = std::stack<std::size_t>{};
  for (std::size_t idx = 0; idx < arr.size(); ++idx) {
    raw.push_back(Node{.value = arr[idx]});
    if (stack.empty()) {
      stack.push(idx);
      continue;
    }
    auto &&top = raw[stack.top()];
    if (top.value > arr[idx]) {
      top.left = idx;
      stack.push(idx);
    } else {
      auto top_idx = stack.top();
      while (!stack.empty() && (raw[stack.top()].value <= arr[idx])) {
        top_idx = stack.top();
        stack.pop();
      }
      raw[top_idx].right = idx;
      stack.push(idx);
    }
  }
  return Tree{.tree = std::move(raw)};
}

auto solve(std::vector<uint32_t> &arr) -> void {
  if (arr.empty()) {
    return;
  }

  auto tree = construct(arr);
  tree.walk<Order::Post>([&](Node &node) -> void { std::print("{} ", node.value); });
  std::println();
  tree.walk<Order::In>([&](Node &node) -> void { std::print("{} ", node.value); });
}

auto run([[maybe_unused]] std::span<char *> args) -> void {
  std::size_t n_len = 0;

  std::cin >> n_len;
  auto arr = std::vector<uint32_t>(n_len);

  for (std::size_t idx = 0; idx < n_len; ++idx) {
    std::cin >> arr[idx];
  }
  solve(arr);
}

} // namespace
} // namespace cpp_contests

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
extern "C++" auto main(int argc, char **argv) -> int {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  try {
    cpp_contests::run(std::span(argv, static_cast<std::size_t>(argc)));
  } catch (std::exception const &e) {
    std::cerr << e.what() << "\n";
    return 1;
  }
}
#pragma GCC diagnostic pop
