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
#include <initializer_list>
#include <iterator>
#include <type_traits>

#include "utils/utils.hpp"

namespace cpp_contests {

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
      difference_type i = 0,
      bool transposed = false,
      std::size_t real_x = X,
      std::size_t real_y = Y,
      MatrixIndices<X> cols = get_indices<X>(),
      MatrixIndices<Y> rows = get_indices<Y>()
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

  constexpr iterator_type base() const
      noexcept(std::is_nothrow_copy_constructible_v<Iterator>) {
    return it_;
  }
  constexpr reference operator*() const {
    assert(i_ >= 0 && i_ < X * Y);
    return *it_;
  }
  constexpr pointer operator->() const
    requires(std::is_pointer_v<Iterator> ||
             requires(Iterator const i) { i.operator->(); })
  {
    assert(i_ >= 0 && i_ < X * Y);
    return it_.operator->();
  }
  constexpr value_type const &operator[](difference_type n) const {
    assert(i_ + n >= 0 && i_ + n < X * Y);
    return *transform(i_ + n);
  }

  constexpr MatrixIterator &operator+=(difference_type n) {
    it_ = transform(i_ + n);
    i_ += n;
    return *this;
  }
  constexpr MatrixIterator &operator-=(difference_type n) {
    return operator+=(-n);
  }
  constexpr MatrixIterator &operator++() { return operator+=(1); }
  constexpr MatrixIterator &operator--() { return operator-=(1); }
  constexpr MatrixIterator operator++(int) {
    auto tmp = *this;
    operator++();
    return tmp;
  }
  constexpr MatrixIterator operator--(int) {
    auto tmp = *this;
    operator--();
    return tmp;
  }

  constexpr MatrixIterator
  operator-(typename MatrixIterator::difference_type n) const {
    auto tmp = it_;
    tmp -= n;
    return tmp;
  }
  friend constexpr difference_type operator-(MatrixIterator const &lhs,
                                             MatrixIterator const &rhs) {
    return lhs.i_ - rhs.i_;
  }
  friend constexpr bool operator==(MatrixIterator const &lhs,
                                   MatrixIterator const &rhs) {
    return lhs.begin_ == rhs.begin_ && lhs.it_ == rhs.it_ &&
           lhs.transposed_ == rhs.transposed_ && lhs.rows_ == rhs.rows_ &&
           lhs.cols_ == rhs.cols_;
  }
  friend constexpr std::partial_ordering
  operator<=>(MatrixIterator const &lhs, MatrixIterator const &rhs) {
    auto ordered = lhs.begin_ == rhs.begin_ &&
                   lhs.transposed_ == rhs.transposed_ &&
                   lhs.rows_ == rhs.rows_ && lhs.cols_ == rhs.cols_;
    if (!ordered)
      return std::partial_ordering::unordered;
    return lhs.i_ <=> rhs.i_;
  }

  [[nodiscard]] constexpr MatrixIndices<Y> const &rows() const noexcept {
    return rows_;
  }
  [[nodiscard]] constexpr MatrixIndices<X> const &cols() const noexcept {
    return cols_;
  }
  constexpr Iterator begin() const noexcept { return begin_; }
  [[nodiscard]] constexpr std::size_t real_x() const noexcept {
    return real_x_;
  }
  [[nodiscard]] constexpr std::size_t real_y() const noexcept {
    return real_y_;
  }
  [[nodiscard]] constexpr bool transposed() const noexcept {
    return transposed_;
  }

private:
  constexpr Iterator transform(difference_type i) const noexcept {
    return transform(begin_, i, transposed_, real_x_, real_y_, cols_, rows_);
  }

  static constexpr Iterator transform(Iterator begin, difference_type i,
                                      bool transposed, std::size_t real_x,
                                      std::size_t real_y,
                                      MatrixIndices<X> const &cols,
                                      MatrixIndices<Y> const &rows) noexcept {
    assert(i >= 0 && i <= X * Y);

    if (i == X * Y)
      return begin + real_x * real_y;

    auto virt_row = i / X;
    auto virt_col = i % X;
    auto row = rows[virt_row];
    auto col = cols[virt_col];

    if (!transposed)
      return begin + (col + row * real_x);
    else
      return begin + (row + col * real_y);
  }
};
template <std::random_access_iterator Iterator, size_t X, size_t Y>
constexpr MatrixIterator<Iterator, X, Y>
operator+(MatrixIterator<Iterator, X, Y> const &it,
          typename MatrixIterator<Iterator, X, Y>::difference_type n) {
  auto tmp = it;
  tmp += n;
  return tmp;
}
template <std::random_access_iterator Iterator, size_t X, size_t Y>
constexpr MatrixIterator<Iterator, X, Y>
operator+(typename MatrixIterator<Iterator, X, Y>::difference_type n,
          MatrixIterator<Iterator, X, Y> const &it) {
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
  constexpr explicit Matrix(std::initializer_list<value_type> m) noexcept {
    std::move(m.begin(), m.end(), data_.begin());
  }
  constexpr explicit Matrix(
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
  static constexpr std::array<value_type, X * Y>
  splat(std::array<std::array<value_type, X>, Y> const &m) noexcept {
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
  constexpr MatrixView(Matrix<X, Y, value_type> &m) noexcept
      : begin_(m.begin()) {}

  template <std::size_t XR, std::size_t YR>
  constexpr MatrixView<XR, YR, T>
  operator()(MatrixIndices<XR> cols, MatrixIndices<XR> rows) const noexcept {
    return MatrixView<XR, YR, T>(begin_, std::move(cols), std::move(rows),
                                 false);
  }

  constexpr MatrixView(MatrixView const &other) noexcept
      : begin_(other.begin_) {}
  constexpr MatrixView(MatrixView &&other) noexcept
      : begin_(std::move(other.begin_)) {}

  constexpr MatrixView &operator=(MatrixView const &other) noexcept {
    std::copy(other.begin(), other.end(), begin());
    return *this;
  }
  constexpr MatrixView &operator=(MatrixView &&other) noexcept {
    std::move(other.begin(), other.end(), begin());
    return *this;
  }

  constexpr value_type &operator()(std::size_t x, std::size_t y) noexcept {
    return begin_[x + y * X];
  }
  constexpr value_type operator()(std::size_t x, std::size_t y) const noexcept {
    return begin_[x + y * X];
  }

  constexpr MatrixView<Y, X, T> transpose() const noexcept {
    return MatrixView<Y, X, T>(begin_, true);
  }

  constexpr iterator begin() const noexcept { return begin_; }
  constexpr iterator end() const noexcept { return begin_ + X * Y; }

  template <std::invocable<value_type> F>
  MatrixView &
  elementwise(F f) noexcept(noexcept(f(std::declval<value_type>()))) {
    std::transform(begin(), end(), begin(), f);
    return *this;
  }
  template <std::invocable<value_type, value_type> F,
            std::convertible_to<value_type> MT>
  MatrixView &elementwise(MatrixView<X, Y, MT> const &m, F f) noexcept(
      noexcept(f(std::declval<value_type>(), std::declval<value_type>()))) {
    std::transform(begin(), end(), m.begin(), begin(), f);
    return *this;
  }
  template <std::convertible_to<value_type> MT>
  MatrixView &operator+=(MatrixView<X, Y, MT> const &m) noexcept {
    return elementwise(
        m, [](value_type a, value_type b) noexcept { return a + b; });
  }
  template <std::convertible_to<value_type> MT>
  MatrixView &operator-=(MatrixView<X, Y, MT> const &m) noexcept {
    return elementwise(
        m, [](value_type a, value_type b) noexcept { return a - b; });
  }
  MatrixView &operator+=(value_type x) noexcept {
    return elementwise([x](value_type a) noexcept { return a + x; });
  }
  MatrixView &operator-=(value_type x) noexcept { return operator+=(-x); }
  MatrixView &operator*=(value_type x) noexcept {
    return elementwise([x](value_type a) noexcept { return a * x; });
  }
  MatrixView &operator/=(value_type x) { return operator*=(value_type{1} / x); }

  ~MatrixView() = default;

private:
  template <std::size_t XR, std::size_t YR>
  constexpr MatrixView(MatrixIterator<MI, XR, YR> const &mi,
                       MatrixIndices<X> &&cols,
                       MatrixIndices<Y> &&rows) noexcept
      : begin_(mi.begin(), 0, transpose, mi.real_x(), mi.real_y(),
               reindex(mi.cols(), std::move(cols)),
               reindex(mi.rows(), std::move(rows))) {}

  constexpr MatrixView(MatrixIterator<MI, Y, X> const &mi, bool) noexcept
      : begin_(mi.begin(), 0, !mi.transposed(), mi.real_y(), mi.real_x(),
               MatrixIndices<Y>(mi.cols()), MatrixIndices<X>(mi.rows())) {}

  template <std::size_t N, std::size_t M>
  static constexpr MatrixIndices<N>
  reindex(MatrixIndices<M> const &original,
          MatrixIndices<N> const &permutation) noexcept {
    MatrixIndices<N> indices;
    for (std::size_t i = 0; i < N; ++i) {
      indices[i] = original[permutation[i]];
    }
    return indices;
  }
};

} // namespace cpp_contests
