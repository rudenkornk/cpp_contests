module;

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <functional>
#include <iostream>
#include <limits>
#include <numeric>
#include <optional>
#include <print>
#include <span>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

export module tree_isomorphism;

namespace cpp_contests {
namespace {

using city = uint32_t;
using tree = std::vector<std::vector<city>>;

auto constexpr kDebug = false;

template <typename T, typename Compare = std::less<T>>
auto sort_permutation(const std::vector<T> &vec, Compare compare = Compare{}) -> std::vector<std::size_t> {
  std::vector<std::size_t> perm(vec.size());
  std::ranges::iota(perm, 0);
  std::sort(perm.begin(), perm.end(),
            [&](std::size_t lhs, std::size_t rhs) -> auto { return compare(vec[lhs], vec[rhs]); });
  return perm;
}

template <typename T> auto permute(const std::vector<T> &vec, const std::vector<std::size_t> &perm) -> std::vector<T> {
  std::vector<T> sorted_vec(vec.size());
  std::transform(perm.begin(), perm.end(), sorted_vec.begin(), [&](std::size_t idx) -> auto { return vec[idx]; });
  return sorted_vec;
}

struct Result {
  std::vector<std::vector<city>> label_map;
  std::vector<std::vector<city>> label_to_city;
  std::vector<city> labels;
  city root;
  std::optional<city> second_root;
};

[[maybe_unused]] auto print(Result const &result) -> void { // NOLINT(readability-function-size)
  std::println("Label map:");
  for (std::size_t idx = 0; idx < result.label_map.size(); ++idx) {
    std::print("  {}:  [", idx);
    auto &&label = result.label_map[idx];
    for (std::size_t item_idx = 0; item_idx < label.size(); ++item_idx) {
      std::print("{}", label[item_idx]);
      if (item_idx + 1 < label.size()) {
        std::print(", ");
      }
    }
    std::println("]");
  }
  std::println("Labels:");
  for (std::size_t idx = 0; idx < result.labels.size(); ++idx) {
    std::println("  {}: {}", idx, result.labels[idx]);
  }
  std::println("Label to city:");
  for (std::size_t idx = 0; idx < result.label_to_city.size(); ++idx) {
    std::print("  {}:  [", idx);
    auto &&label = result.label_to_city[idx];
    for (std::size_t item_idx = 0; item_idx < label.size(); ++item_idx) {
      std::print("{}", label[item_idx]);
      if (item_idx + 1 < label.size()) {
        std::print(", ");
      }
    }
    std::println("]");
  }

  std::println("Root: {}", result.root);
  std::println("Second root: {}", result.second_root.has_value() ? std::to_string(result.second_root.value()) : "none");
}

// NOLINTNEXTLINE(readability-function-cognitive-complexity, readability-function-size)
auto invariant(tree const &input_tree) -> Result {
  auto leaves = std::vector<city>{};
  auto degrees = std::vector<city>(input_tree.size());
  auto labels = std::vector<city>(input_tree.size());
  auto raw_labels = std::vector<std::vector<city>>(input_tree.size());
  auto label_map = std::vector<std::vector<city>>{std::vector<city>{}};

  // Create initial list of leaves and degrees of each node.
  for (city vertex = 0; vertex < input_tree.size(); ++vertex) {
    if (input_tree[vertex].size() <= 1) {
      leaves.push_back(vertex);
    }
    degrees[vertex] = static_cast<city>(input_tree[vertex].size());
    labels[vertex] = 0;
  }

  auto root = city{};
  auto second_root = std::optional<city>{};

  // Peel one layer of leaves at a time, constructing invariant representation in the process.
  auto next_leaves = std::vector<city>{};
  auto new_raw_labels = std::vector<std::vector<city>>{};
  while (!leaves.empty()) {
    next_leaves.clear();
    for (auto &&leaf : leaves) {
      for (auto &&neighbor : input_tree[leaf]) {
        raw_labels[neighbor].push_back(labels[leaf]);
        if (--degrees[neighbor] == 1) {
          if (degrees[neighbor] == 1) {
            next_leaves.push_back(neighbor);
          }
        }
      }
    }

    // Generate & assign new labels.
    // We need to collect all new raw labels ([0, 0, 1], [1, 1, 0], ...), then sort each of them.
    // And then sort the vector of raw labels themselves.
    // This should ensure stable encoding of all isomorphic trees.
    new_raw_labels.clear();
    new_raw_labels.reserve(next_leaves.size());
    for (auto &&leaf : next_leaves) {
      std::ranges::sort(raw_labels[leaf]);
      new_raw_labels.push_back(raw_labels[leaf]);
    }
    auto perm = sort_permutation(new_raw_labels);
    next_leaves = permute(next_leaves, perm);
    new_raw_labels = permute(new_raw_labels, perm);

    // Sorted, now generate new labels.
    for (std::size_t idx = 0; idx < next_leaves.size(); ++idx) {
      if (new_raw_labels[idx] != label_map.back()) {
        label_map.push_back(new_raw_labels[idx]);
      }
      labels[next_leaves[idx]] = static_cast<city>(label_map.size() - 1);
    }

    // Save roots.
    if (next_leaves.empty()) {
      root = leaves[0];
      if (leaves.size() == 2) {
        second_root = leaves[1];
      }
    }

    leaves = std::move(next_leaves);
  }

  // For convenience, also generate a mapping from label to cities with that label.
  auto label_to_city = std::vector<std::vector<city>>(label_map.size());
  for (city vertex = 0; vertex < labels.size(); ++vertex) {
    label_to_city[labels[vertex]].push_back(vertex);
  }

  return Result{.label_map = std::move(label_map),
                .label_to_city = std::move(label_to_city),
                .labels = std::move(labels),
                .root = root,
                .second_root = second_root};
}

// NOLINTNEXTLINE(readability-function-size)
auto isomorphism(Result const &x_result, tree const &x_tree, Result const &y_result, tree const &y_tree)
    -> std::optional<std::vector<city>> {
  using std::swap;
  if (x_result.second_root.has_value() != y_result.second_root.has_value()) {
    return std::nullopt;
  }
  if (x_result.labels.size() != y_result.labels.size()) {
    return std::nullopt;
  }
  if (x_result.label_map != y_result.label_map) {
    return std::nullopt;
  }
  auto constexpr null = std::numeric_limits<city>::max();
  auto result = std::vector(y_result.labels.size(), null);
  auto buckets = std::vector<std::vector<city>>(x_result.label_map.size());
  auto stack = std::vector<std::tuple<city, city, city, city>>{};

  if (!x_result.second_root.has_value()) {
    stack.emplace_back(y_result.root, null, x_result.root, null);
  } else {
    auto y1 = y_result.root;
    auto y2 = y_result.second_root.value();
    auto x1 = x_result.root;
    auto x2 = x_result.second_root.value();
    if (x_result.labels[x1] != y_result.labels[y1]) {
      swap(x1, x2);
    }

    stack.emplace_back(y1, y2, x1, x2);
    stack.emplace_back(y2, y1, x2, x1);
  }

  while (!stack.empty()) {
    auto [y, y_parent, x, x_parent] = stack.back();
    stack.pop_back();

    result[y] = x;
    for (auto &&child : x_tree[x]) {
      if (child == x_parent) {
        continue;
      }
      buckets[x_result.labels[child]].push_back(child);
    }

    for (auto &&child : y_tree[y]) {
      if (child == y_parent) {
        continue;
      }
      auto x_child = buckets[y_result.labels[child]].back();
      buckets[y_result.labels[child]].pop_back();
      stack.emplace_back(child, y, x_child, x);
    }
  }

  return result;
}

auto solve(tree const &x_tree, tree const &y_tree) -> void {
  auto xk_result = invariant(x_tree);
  auto yk_result = invariant(y_tree);

  if constexpr (kDebug) {
    std::println("X:");
    print(xk_result);
    std::println("\nY:");
    print(yk_result);
  }

  auto iso = isomorphism(xk_result, x_tree, yk_result, y_tree);
  if (!iso.has_value()) {
    std::println("-1");
    return;
  }
  for (auto &&vertex : iso.value()) {
    std::println("{}", vertex + 1);
  }
}

auto run([[maybe_unused]] std::span<char *> args) -> void {
  std::size_t n_len = 0;

  std::cin >> n_len;

  tree xk(n_len);
  tree yk(n_len);

  for (std::size_t i = 0; i < n_len - 1; ++i) {
    auto first = city{};
    auto second = city{};
    std::cin >> first >> second;
    --first;
    --second;
    yk[first].push_back(second);
    yk[second].push_back(first);
  }
  for (std::size_t i = 0; i < n_len - 1; ++i) {
    auto first = city{};
    auto second = city{};
    std::cin >> first >> second;
    --first;
    --second;
    xk[first].push_back(second);
    xk[second].push_back(first);
  }
  solve(xk, yk);
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
