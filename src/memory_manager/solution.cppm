module;

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <iostream>
#include <iterator>
#include <list>
#include <optional>
#include <print>
#include <queue>
#include <span>
#include <tuple>
#include <utility>
#include <vector>

export module memory_manager;

namespace cpp_contests {
namespace {

using size_type = uint32_t;

struct ListBlock {
  size_type end;
  size_type size;
  size_type id;
  bool free;
};

using iterator_type = std::list<ListBlock>::iterator;

struct FreeBlock {
  // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
  size_type end;
  size_type size;
  size_type id;
  iterator_type iterator;
  // NOLINTEND(misc-non-private-member-variables-in-classes)

  auto operator<(FreeBlock const &rhs) const noexcept -> bool {
    return std::tie(size, rhs.end) < std::tie(rhs.size, end);
  }

  auto static from_iterator(iterator_type const &iter) -> FreeBlock {
    return FreeBlock{.end = iter->end, .size = iter->size, .id = iter->id, .iterator = iter};
  }
};

auto pop(std::priority_queue<FreeBlock> &free_blocks, std::vector<bool> const &dead, int request)
    -> std::optional<FreeBlock> {
  while (true) {
    if (free_blocks.empty()) {
      return std::nullopt;
    }
    auto top = free_blocks.top();
    if (dead[top.id]) {
      free_blocks.pop();
      continue;
    }
    if (std::cmp_less(top.size, request)) {
      return std::nullopt;
    }
    free_blocks.pop();
    return top;
  }
}

auto decrease(std::list<ListBlock> &list, iterator_type iterator, int request)
    -> std::pair<iterator_type, std::optional<iterator_type>> {
  auto &&element = *iterator;
  element.size -= request;
  auto used_block =
      ListBlock{.end = element.end - element.size, .size = static_cast<size_type>(request), .id = 0, .free = false};
  auto history = list.insert(iterator, used_block);
  if (element.size > 0) {
    return {history, iterator};
  }
  list.erase(iterator);
  return {history, std::nullopt};
}

// NOLINTNEXTLINE(readability-function-size)
auto return_block(std::priority_queue<FreeBlock> &free_blocks, std::vector<bool> &dead, std::list<ListBlock> &list,
                  iterator_type iterator, size_type &next_id) -> void {
  auto prev = std::optional<iterator_type>{};
  auto next = std::optional<iterator_type>{};
  if (iterator != list.begin() && std::prev(iterator)->free) {
    prev = std::prev(iterator);
    dead[prev.value()->id] = true;
  }
  if (iterator != std::prev(list.end()) && std::next(iterator)->free) {
    next = std::next(iterator);
    dead[next.value()->id] = true;
  }
  if (prev.has_value()) {
    prev.value()->end = iterator->end;
    prev.value()->size += iterator->size;
    list.erase(iterator);
    iterator = prev.value();
  }
  if (next.has_value()) {
    iterator->end = next.value()->end;
    iterator->size += next.value()->size;
    list.erase(next.value());
  }
  iterator->id = next_id++;
  iterator->free = true;
  free_blocks.push(FreeBlock::from_iterator(iterator));
}

// NOLINTNEXTLINE(readability-function-size)
auto solve(std::vector<int> const &requests, size_type n_len) -> void {
  auto free_blocks = std::priority_queue<FreeBlock>{};
  auto list_blocks = std::list<ListBlock>{};
  auto history = std::vector<std::optional<iterator_type>>(requests.size());
  auto dead = std::vector<bool>(requests.size(), false);

  size_type id = 0;
  list_blocks.push_back(ListBlock{.end = n_len, .size = n_len, .id = id++, .free = true});
  free_blocks.push(FreeBlock::from_iterator(list_blocks.begin()));

  for (size_type idx = 0; idx < requests.size(); ++idx) {
    auto request = requests[idx];

    if (request > 0) {
      auto free_block = pop(free_blocks, dead, request);
      if (!free_block.has_value()) {
        std::println("-1");
        continue;
      }
      auto [history_block, new_block] = decrease(list_blocks, free_block->iterator, request);
      history[idx] = history_block;
      std::println("{}", history_block->end - history_block->size + 1);
      if (new_block.has_value()) {
        free_blocks.push(FreeBlock::from_iterator(new_block.value()));
      }
    } else {
      auto const history_idx = static_cast<size_type>(-request);
      auto const &allocated_block = history[history_idx];
      if (!allocated_block.has_value()) {
        continue;
      }
      return_block(free_blocks, dead, list_blocks, allocated_block.value(), id);
    }
  }
}

auto run([[maybe_unused]] std::span<char *> args) -> void {
  size_type n_len = 0;
  size_type m_len = 0;

  std::cin >> n_len >> m_len;

  auto requests = std::vector<int>(m_len);

  for (size_type i = 0; i < m_len; ++i) {
    std::cin >> requests[i];
    if (requests[i] < 0) {
      ++requests[i];
    }
  }
  solve(requests, n_len);
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
