#pragma once

// An alternative draft implementation of a matrix.
// The core idea is to use Matrix View to perform operations on the matrix.
// This allows to get arbitrary views on the matrix including transpose, slices,
// permutes and make operations on them, affects the original matrix. The
// downside is that we need to actually store two arrays of permuted indices in
// the iterator, which is quite expensive and makes the whole point of static
// matrices irrelevant.

#include <algorithm>
#include <array>
#include <cassert>
#include <compare>
#include <concepts>
#include <cstddef>
#include <initializer_list>
#include <iterator>
#include <type_traits>

#include "utils/utils.hpp"

namespace cpp_contests {
// NOLINTBEGIN(readability-identifier-length)

template <std::size_t N> using MatrixIndices = std::array<std::size_t, N>;

template <std::random_access_iterator Iterator, size_t X, size_t Y>
class MatrixIterator final {
public:
  using iterator_type = Iterator;
  using iterator_concept = std::random_access_iterator_tag;
  using iterator_category = std::random_access_iterator_tag;
  using difference_type = typename std::iter_difference_t<Iterator>;
  using value_type = typename std::iter_value_t<Iterator>;
  using pointer = typename std::iterator_traits<Iterator>::pointer;
  using reference = typename std::iter_reference_t<Iterator>;
  static_assert(std::is_signed_v<difference_type>);

  static constexpr std::size_t size_x = X;
  static constexpr std::size_t size_y = Y;

private:
  Iterator begin_;
  difference_type i_ = 0;
  Iterator it_;
  bool transposed_ = false;
  std::size_t real_x_ = X;
  std::size_t real_y_ = Y;
  MatrixIndices<X> cols_ = get_indices<X>();
  MatrixIndices<Y> rows_ = get_indices<Y>();

public:
  // clang-format off
  constexpr explicit MatrixIterator(
      Iterator begin,
      difference_type i,
      bool transposed,
      std::size_t real_x,
      std::size_t real_y,
      MatrixIndices<X> cols,
      MatrixIndices<Y> rows
    ) noexcept(std::is_nothrow_move_constructible_v<Iterator>)
      : begin_(std::move(begin)),
        i_(i),
        it_(transform(begin, i, transposed, real_x, real_y, cols, rows)),
        transposed_(transposed),
        real_x_(real_x),
        real_y_(real_y),
        cols_(std::move(cols)),
        rows_(std::move(rows)) {
    #ifndef NDEBUG
      // rows and cols must be unique
      auto debug_cols = cols_;
      auto debug_rows = rows_;
      std::sort(debug_cols.begin(), debug_cols.end());
      std::sort(debug_rows.begin(), debug_rows.end());
      assert(std::unique(debug_cols.begin(), debug_cols.end()) == debug_cols.end());
      assert(std::unique(debug_rows.begin(), debug_rows.end()) == debug_rows.end());
    #endif
  }
  // clang-format on
  constexpr explicit MatrixIterator() noexcept(
      std::is_nothrow_constructible_v<Iterator>) = default;

  constexpr auto base() const
      noexcept(std::is_nothrow_copy_constructible_v<Iterator>)
          -> iterator_type {
    return it_;
  }
  constexpr auto operator*() const -> reference {
    assert(i_ >= 0 && i_ < X * Y);
    return *it_;
  }
  constexpr auto operator->() const -> pointer
    requires(std::is_pointer_v<Iterator> ||
             requires(Iterator const i) { i.operator->(); })
  {
    assert(i_ >= 0 && i_ < X * Y);
    return it_.operator->();
  }
  constexpr auto operator[](difference_type n) const -> value_type const & {
    assert(i_ + n >= 0 && i_ + n < X * Y);
    return *transform(i_ + n);
  }

  constexpr auto operator+=(difference_type n) -> MatrixIterator & {
    it_ = transform(i_ + n);
    i_ += n;
    return *this;
  }
  constexpr auto operator-=(difference_type n) -> MatrixIterator & {
    return operator+=(-n);
  }
  constexpr auto operator++() -> MatrixIterator & { return operator+=(1); }
  constexpr auto operator--() -> MatrixIterator & { return operator-=(1); }
  constexpr auto operator++(int) -> MatrixIterator {
    auto tmp = *this;
    operator++();
    return tmp;
  }
  constexpr auto operator--(int) -> MatrixIterator {
    auto tmp = *this;
    operator--();
    return tmp;
  }

  constexpr auto operator-(typename MatrixIterator::difference_type n) const
      -> MatrixIterator {
    auto tmp = it_;
    tmp -= n;
    return tmp;
  }
  friend constexpr auto operator-(MatrixIterator const &lhs,
                                  MatrixIterator const &rhs)
      -> difference_type {
    return lhs.i_ - rhs.i_;
  }
  friend constexpr auto operator==(MatrixIterator const &lhs,
                                   MatrixIterator const &rhs) -> bool {
    return lhs.begin_ == rhs.begin_ && lhs.it_ == rhs.it_ &&
           lhs.transposed_ == rhs.transposed_ && lhs.rows_ == rhs.rows_ &&
           lhs.cols_ == rhs.cols_;
  }
  friend constexpr auto operator<=>(MatrixIterator const &lhs,
                                    MatrixIterator const &rhs)
      -> std::partial_ordering {
    auto ordered = lhs.begin_ == rhs.begin_ &&
                   lhs.transposed_ == rhs.transposed_ &&
                   lhs.rows_ == rhs.rows_ && lhs.cols_ == rhs.cols_;
    if (!ordered) {
      return std::partial_ordering::unordered;
    }
    return lhs.i_ <=> rhs.i_;
  }

  [[nodiscard]] constexpr auto rows() const noexcept
      -> MatrixIndices<Y> const & {
    return rows_;
  }
  [[nodiscard]] constexpr auto cols() const noexcept
      -> MatrixIndices<X> const & {
    return cols_;
  }
  constexpr auto begin() const noexcept -> Iterator { return begin_; }
  [[nodiscard]] constexpr auto real_x() const noexcept -> std::size_t {
    return real_x_;
  }
  [[nodiscard]] constexpr auto real_y() const noexcept -> std::size_t {
    return real_y_;
  }
  [[nodiscard]] constexpr auto transposed() const noexcept -> bool {
    return transposed_;
  }

private:
  constexpr auto transform(difference_type i) const noexcept -> Iterator {
    return transform(begin_, i, transposed_, real_x_, real_y_, cols_, rows_);
  }

  static constexpr auto
  transform(Iterator begin, difference_type i, bool transposed,
            std::size_t real_x, std::size_t real_y,
            MatrixIndices<X> const &cols, MatrixIndices<Y> const &rows) noexcept
      -> Iterator {
    assert(i >= 0 && i <= X * Y);

    if (i == X * Y) {
      return begin + (real_x * real_y);
    }

    auto virt_row = i / X;
    auto virt_col = i % X;
    auto row = rows[virt_row];
    auto col = cols[virt_col];

    if (!transposed) {
      return begin + (col + row * real_x);
    }
    return begin + (row + col * real_y);
  }
};
template <std::random_access_iterator Iterator, size_t X, size_t Y>
constexpr auto
operator+(MatrixIterator<Iterator, X, Y> const &it,
          typename MatrixIterator<Iterator, X, Y>::difference_type n)
    -> MatrixIterator<Iterator, X, Y> {
  auto tmp = it;
  tmp += n;
  return tmp;
}
template <std::random_access_iterator Iterator, size_t X, size_t Y>
constexpr auto
operator+(typename MatrixIterator<Iterator, X, Y>::difference_type n,
          MatrixIterator<Iterator, X, Y> const &it)
    -> MatrixIterator<Iterator, X, Y> {
  return it + n;
}

//    Memory, X axis
//    ───────────────►
//  Y│
//   │
//  a│
//  x│
//  i│
//  s│
//   ▼
template <std::size_t X, std::size_t Y, typename T>
  requires(X > 0 && Y > 0)
class Matrix final {
public:
  using value_type = T;
  using Container = std::array<value_type, X * Y>;
  using iterator = typename Container::iterator;
  using const_iterator = typename Container::const_iterator;

  static constexpr std::size_t size_x = X;
  static constexpr std::size_t size_y = Y;
  static constexpr std::size_t size = X * Y;

private:
  Container data_;

public:
  constexpr Matrix() noexcept { data_.fill(value_type{}); }
  constexpr explicit Matrix(
      std::array<std::array<value_type, X>, Y> const &m) noexcept
      : data_(splat(m)) {}
  constexpr explicit Matrix(std::array<value_type, X * Y> m) noexcept
      : data_(std::move(m)) {}
  constexpr Matrix(std::initializer_list<value_type> m) noexcept {
    std::move(m.begin(), m.end(), data_.begin());
  }
  constexpr Matrix(
      std::initializer_list<std::initializer_list<value_type>> m) noexcept {
    assert(m.size() == Y);
    std::size_t y = 0;
    for (auto &&l : m) {
      assert(l.size() == X);
      std::move(l.begin(), l.end(), data_.begin() + y * X);
      ++y;
    }
  }
  template <std::input_iterator Iterator>
  constexpr explicit Matrix(Iterator first, Iterator last) noexcept {
    std::copy(first, last, data_.begin());
  }

private:
  static constexpr auto
  splat(std::array<std::array<value_type, X>, Y> const &m) noexcept
      -> std::array<value_type, X * Y> {
    std::array<value_type, X * Y> m_;
    for (std::size_t i = 0; i < X * Y; ++i) {
      m_[i] = m[i / X][i % X];
    }
    return m_;
  }
};

template <std::size_t X, std::size_t Y, typename T>
  requires(X > 0 && Y > 0)
class MatrixView final {
private:
  using MI = typename Matrix<X, Y, T>::iterator;

public:
  using value_type = T;
  using iterator = MatrixIterator<MI, X, Y>;

private:
  iterator begin_;

public:
  constexpr explicit MatrixView(Matrix<X, Y, value_type> &m) noexcept
      : begin_(m.begin()) {}

  template <std::size_t XR, std::size_t YR>
  constexpr auto operator()(MatrixIndices<XR> cols,
                            MatrixIndices<XR> rows) const noexcept
      -> MatrixView<XR, YR, T> {
    return MatrixView<XR, YR, T>(begin_, std::move(cols), std::move(rows),
                                 false);
  }

  constexpr MatrixView(MatrixView const &other) noexcept
      : begin_(other.begin_) {}
  constexpr MatrixView(MatrixView &&other) noexcept
      : begin_(std::move(other.begin_)) {}

  constexpr auto operator=(MatrixView const &other) noexcept -> MatrixView & {
    if (this == &other) {
      return *this;
    }
    std::copy(other.begin(), other.end(), begin());
    return *this;
  }
  constexpr auto operator=(MatrixView &&other) noexcept -> MatrixView & {
    if (this == &other) {
      return *this;
    }
    std::move(other.begin(), other.end(), begin());
    return *this;
  }

  constexpr auto operator()(std::size_t x, std::size_t y) noexcept
      -> value_type & {
    return begin_[x + (y * X)];
  }
  constexpr auto operator()(std::size_t x, std::size_t y) const noexcept
      -> value_type {
    return begin_[x + (y * X)];
  }

  constexpr auto transpose() const noexcept -> MatrixView<Y, X, T> {
    return MatrixView<Y, X, T>(begin_, true);
  }

  constexpr auto begin() const noexcept -> iterator { return begin_; }
  constexpr auto end() const noexcept -> iterator { return begin_ + X * Y; }

  template <std::invocable<value_type> F>
  auto elementwise(F f) noexcept(noexcept(f(std::declval<value_type>())))
      -> MatrixView & {
    std::transform(begin(), end(), begin(), f);
    return *this;
  }
  template <std::invocable<value_type, value_type> F,
            std::convertible_to<value_type> MT>
  auto elementwise(MatrixView<X, Y, MT> const &m,
                   F f) noexcept(noexcept(f(std::declval<value_type>(),
                                            std::declval<value_type>())))
      -> MatrixView & {
    std::transform(begin(), end(), m.begin(), begin(), f);
    return *this;
  }
  template <std::convertible_to<value_type> MT>
  auto operator+=(MatrixView<X, Y, MT> const &m) noexcept -> MatrixView & {
    return elementwise(
        m, [](value_type a, value_type b) noexcept -> auto { return a + b; });
  }
  template <std::convertible_to<value_type> MT>
  auto operator-=(MatrixView<X, Y, MT> const &m) noexcept -> MatrixView & {
    return elementwise(
        m, [](value_type a, value_type b) noexcept -> auto { return a - b; });
  }
  auto operator+=(value_type x) noexcept -> MatrixView & {
    return elementwise([x](value_type a) noexcept -> auto { return a + x; });
  }
  auto operator-=(value_type x) noexcept -> MatrixView & {
    return operator+=(-x);
  }
  auto operator*=(value_type x) noexcept -> MatrixView & {
    return elementwise([x](value_type a) noexcept -> auto { return a * x; });
  }
  auto operator/=(value_type x) -> MatrixView & {
    return operator*=(value_type{1} / x);
  }

  ~MatrixView() = default;

private:
  template <std::size_t XR, std::size_t YR>
  constexpr MatrixView(MatrixIterator<MI, XR, YR> const &mi,
                       MatrixIndices<X> &&cols,
                       MatrixIndices<Y> &&rows) noexcept
      : begin_(mi.begin(), 0, transpose, mi.real_x(), mi.real_y(),
               reindex(mi.cols(), std::move(cols)),
               reindex(mi.rows(), std::move(rows))) {}

  constexpr MatrixView(MatrixIterator<MI, Y, X> const &mi,
                       bool /*unused*/) noexcept
      : begin_(mi.begin(), 0, !mi.transposed(), mi.real_y(), mi.real_x(),
               MatrixIndices<Y>(mi.cols()), MatrixIndices<X>(mi.rows())) {}

  template <std::size_t N, std::size_t M>
  static constexpr auto reindex(MatrixIndices<M> const &original,
                                MatrixIndices<N> const &permutation) noexcept
      -> MatrixIndices<N> {
    MatrixIndices<N> indices;
    for (std::size_t i = 0; i < N; ++i) {
      indices[i] = original[permutation[i]];
    }
    return indices;
  }
};

// NOLINTEND(readability-identifier-length)
} // namespace cpp_contests
