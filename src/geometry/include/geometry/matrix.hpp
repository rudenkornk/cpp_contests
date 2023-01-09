#pragma once

#include <algorithm>
#include <array>
#include <cassert>
#include <compare>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <numeric>
#include <type_traits>

#include "utils/math.hpp"

namespace cpp_contests {

constexpr double EPSMIN = 1e-100;
constexpr double EPSMAX = 1e-6;

template <std::random_access_iterator Iterator, size_t X, size_t Y>
class TransposeIterator final {
public:
  using iterator_type = Iterator;
  using iterator_concept = std::random_access_iterator_tag;
  using iterator_category = std::random_access_iterator_tag;
  using difference_type = typename std::iter_difference_t<Iterator>;
  using value_type = typename std::iter_value_t<Iterator>;
  using pointer = typename std::iterator_traits<Iterator>::pointer;
  using reference = typename std::iter_reference_t<Iterator>;
  static_assert(std::is_signed_v<difference_type>);

private:
  Iterator begin_;
  difference_type i_;
  Iterator it_;

public:
  // clang-format off
  constexpr explicit TransposeIterator(Iterator begin, difference_type i) noexcept(
      std::is_nothrow_move_constructible_v<Iterator>)
      : begin_(std::move(begin)), i_(std::move(i)), it_(transform(begin, i)) {}
  // clang-format on
  constexpr explicit TransposeIterator(Iterator begin) noexcept(
      std::is_nothrow_copy_constructible_v<Iterator>)
      : begin_(begin), i_(0), it_(begin) {}
  constexpr explicit TransposeIterator() noexcept(
      std::is_nothrow_constructible_v<Iterator>)
      : begin_(Iterator{}), i_(0), it_(Iterator{}) {}

  constexpr reference operator*() const {
    assert(i_ >= 0);
    assert(static_cast<size_t>(i_) < X * Y);
    return *it_;
  }
  constexpr pointer operator->() const
    requires(std::is_pointer_v<Iterator> ||
             requires(Iterator const i) { i.operator->(); })
  {
    assert(i_ >= 0);
    assert(static_cast<size_t>(i_) < X * Y);
    return it_.operator->();
  }
  constexpr value_type const &operator[](difference_type n) const {
    assert(i_ + n >= 0);
    assert(static_cast<size_t>(i_ + n) < X * Y);
    return *transform(i_ + n);
  }

  constexpr TransposeIterator &operator+=(difference_type n) {
    it_ = transform(i_ + n);
    i_ += n;
    return *this;
  }
  constexpr TransposeIterator &operator-=(difference_type n) {
    return operator+=(-n);
  }
  constexpr TransposeIterator &operator++() { return operator+=(1); }
  constexpr TransposeIterator &operator--() { return operator-=(1); }
  constexpr TransposeIterator operator++(int) {
    auto tmp = *this;
    operator++();
    return tmp;
  }
  constexpr TransposeIterator operator--(int) {
    auto tmp = *this;
    operator--();
    return tmp;
  }

  friend constexpr difference_type operator-(TransposeIterator const &lhs,
                                             TransposeIterator const &rhs) {
    return lhs.i_ - rhs.i_;
  }
  friend constexpr bool operator==(TransposeIterator const &lhs,
                                   TransposeIterator const &rhs) {
    return lhs.it_ == rhs.it_;
  }
  friend constexpr std::partial_ordering
  operator<=>(TransposeIterator const &lhs, TransposeIterator const &rhs) {
    if (lhs.begin() != rhs.begin())
      return std::partial_ordering::unordered;
    return lhs.i_ <=> rhs.i_;
  }

private:
  constexpr Iterator transform(difference_type i_tr) const
      noexcept(noexcept(begin_ + i_tr)) {
    return transform(begin_, i_tr);
  }

  static constexpr Iterator
  transform(Iterator begin,
            difference_type i_tr) noexcept(noexcept(begin + i_tr)) {
    assert(i_tr >= 0);
    assert(static_cast<size_t>(i_tr) <= X * Y);
    if (i_tr == X * Y)
      return begin + i_tr;
    auto x_tr = i_tr / Y;
    auto y_tr = i_tr % Y;
    auto i = x_tr + y_tr * X;
    return begin + i;
  }
};
template <std::random_access_iterator Iterator, size_t X, size_t Y>
constexpr TransposeIterator<Iterator, X, Y>
operator+(TransposeIterator<Iterator, X, Y> const &it,
          typename TransposeIterator<Iterator, X, Y>::difference_type n) {
  auto tmp = it;
  tmp += n;
  return tmp;
}
template <std::random_access_iterator Iterator, size_t X, size_t Y>
constexpr TransposeIterator<Iterator, X, Y>
operator+(typename TransposeIterator<Iterator, X, Y>::difference_type n,
          TransposeIterator<Iterator, X, Y> const &it) {
  auto tmp = it;
  tmp += n;
  return tmp;
}
template <std::random_access_iterator Iterator, size_t X, size_t Y>
constexpr TransposeIterator<Iterator, X, Y>
operator-(TransposeIterator<Iterator, X, Y> const &it,
          typename TransposeIterator<Iterator, X, Y>::difference_type n) {
  auto tmp = it;
  tmp -= n;
  return tmp;
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
template <std::size_t X, std::size_t Y, typename T> class Matrix final {
public:
  using value_type = T;
  using Container = std::array<value_type, X * Y>;
  using iterator = typename Container::iterator;
  using const_iterator = typename Container::const_iterator;
  using transpose_iterator = TransposeIterator<iterator, X, Y>;
  using const_transpose_iterator = TransposeIterator<const_iterator, X, Y>;

private:
  Container data_;

public:
  constexpr Matrix() noexcept { data_.fill(value_type{}); }
  constexpr explicit Matrix(
      std::array<std::array<value_type, X>, Y> const &m) noexcept
      : data_(splat_(m)) {}
  constexpr explicit Matrix(std::array<value_type, X * Y> m) noexcept
      : data_(std::move(m)) {}
  constexpr explicit Matrix(std::initializer_list<value_type> m) noexcept {
    assert(m.size() == X * Y);
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
  constexpr Matrix(Iterator first, Iterator last) noexcept {
    std::copy(first, last, data_.begin());
  }

  // clang-format off
  template <typename... Matrices>
    requires(
            (std::is_same_v<
                std::remove_cvref_t<Matrices>,
                Matrix<
                  std::remove_cvref_t<Matrices>::size_x(),
                  std::remove_cvref_t<Matrices>::size_y(),
                  typename std::remove_cvref_t<Matrices>::value_type>
                  >
            && ...)
            && ((std::remove_cvref_t<Matrices>::size_x() == X) && ...)
            && ((std::remove_cvref_t<Matrices>::size_y() + ...) == Y)
          )
  // clang-format on
  constexpr explicit Matrix(Matrices &&...row_like) noexcept {
    unpack_row_<0, Matrices...>(std::forward<Matrices>(row_like)...);
  }
  // clang-format off
  template <typename... Matrices>
    requires(
            (std::is_same_v<
                std::remove_cvref_t<Matrices>,
                Matrix<
                  std::remove_cvref_t<Matrices>::size_x(),
                  std::remove_cvref_t<Matrices>::size_y(),
                  typename std::remove_cvref_t<Matrices>::value_type>
                  >
              && ...)
              && ((std::remove_cvref_t<Matrices>::size_x() + ...) == X)
              && ((std::remove_cvref_t<Matrices>::size_y() == Y) && ...)
              && sizeof...(Matrices) >= 2 // sizeof == 1 conflicts with row_like piecewise ctor
            )
  // clang-format on
  constexpr explicit Matrix(Matrices &&...col_like) noexcept {
    unpack_col_<0, Matrices...>(std::forward<Matrices>(col_like)...);
  }

private:
  template <std::size_t Unpacked, typename MatrixArg, typename... Matrices>
  // NOLINTNEXTLINE
  constexpr void unpack_row_(MatrixArg &&row_like,
                             Matrices &&...rest) noexcept {
    if constexpr (std::is_rvalue_reference_v<MatrixArg &&>)
      std::move(row_like.begin(), row_like.end(), begin() + (X * Unpacked));
    else
      std::copy(row_like.begin(), row_like.end(), begin() + (X * Unpacked));
    unpack_row_<Unpacked + std::remove_cvref_t<MatrixArg>::size_y(),
                Matrices...>(std::forward<Matrices>(rest)...);
  }
  template <std::size_t Unpacked> constexpr void unpack_row_() noexcept {}

  template <std::size_t Unpacked, typename MatrixArg, typename... Matrices>
  // NOLINTNEXTLINE
  constexpr void unpack_col_(MatrixArg &&col_like,
                             Matrices &&...rest) noexcept {
    if constexpr (std::is_rvalue_reference_v<MatrixArg &&>)
      std::move(col_like.tbegin(), col_like.tend(), tbegin() + (Y * Unpacked));
    else
      std::copy(col_like.tbegin(), col_like.tend(), tbegin() + (Y * Unpacked));
    unpack_col_<Unpacked + std::remove_cvref_t<MatrixArg>::size_x(),
                Matrices...>(std::forward<Matrices>(rest)...);
  }
  template <std::size_t Unpacked> constexpr void unpack_col_() noexcept {}

public:
  template <std::size_t X2 = X, std::size_t Y2 = Y>
    requires(X2 == X && Y2 == Y && X == 1 && Y == 1)
  constexpr operator value_type() const {
    return data_[0];
  }

  constexpr iterator begin() noexcept { return data_.begin(); }
  constexpr iterator end() noexcept { return data_.end(); }
  constexpr const_iterator begin() const noexcept { return data_.begin(); }
  constexpr const_iterator end() const noexcept { return data_.end(); }
  constexpr transpose_iterator tbegin() noexcept {
    return transpose_iterator(begin(), 0);
  }
  constexpr transpose_iterator tend() noexcept {
    return transpose_iterator(begin(), X * Y);
  }
  constexpr const_transpose_iterator tbegin() const noexcept {
    return const_transpose_iterator(begin(), 0);
  }
  constexpr const_transpose_iterator tend() const noexcept {
    return const_transpose_iterator(begin(), X * Y);
  }

  constexpr value_type operator[](std::size_t i) const noexcept {
    assert(i < X * Y);
    return data_[i];
  }
  constexpr value_type &operator[](std::size_t i) noexcept {
    assert(i < X * Y);
    return data_[i];
  }
  constexpr value_type operator[](std::size_t x, std::size_t y) const noexcept {
    assert(x < X && y < Y);
    return data_[y * X + x];
  }
  constexpr value_type &operator[](std::size_t x, std::size_t y) noexcept {
    assert(x < X && y < Y);
    return data_[y * X + x];
  }
  template <std::size_t SubX, std::size_t SubY>
  constexpr Matrix<SubX, SubY, T>
  operator[](Matrix<1, SubX, std::size_t> const &cols,
             Matrix<1, SubY, std::size_t> const &rows) const {
    static_assert(SubX <= X && SubY <= Y);
    Matrix<SubX, SubY, value_type> m{};
    for (std::size_t i = 0; i < SubX * SubY; ++i) {
      auto x = i % SubX;
      auto y = i / SubX;
      assert(cols[x] < X && rows[y] < Y);
      m[x, y] = operator[](cols[x], rows[y]);
    }
    return m;
  }
  constexpr Matrix<X, 1, value_type> row(std::size_t y) const noexcept {
    assert(y < Y);
    Matrix<X, 1, value_type> m{};
    std::copy(begin() + y * X, begin() + (y + 1) * X, m.begin());
    return m;
  }
  constexpr Matrix<1, Y, value_type> col(std::size_t x) const noexcept {
    assert(x < X);
    Matrix<1, Y, value_type> m;
    for (std::size_t i = 0; i < Y; ++i) {
      m[i] = data_[x + i * X];
    }
    return m;
  }
  template <std::convertible_to<value_type> MT>
  constexpr Matrix &set_row(Matrix<X, 1, MT> m, std::size_t y) noexcept {
    assert(y < Y);
    std::copy(m.begin(), m.end(), begin() + y * X);
    return *this;
  }
  template <std::convertible_to<value_type> MT>
  constexpr Matrix &set_col(Matrix<1, Y, MT> m, std::size_t x) noexcept {
    assert(x < X);
    std::copy(m.begin(), m.end(), tbegin() + x * Y);
    return *this;
  }

  [[nodiscard]] constexpr std::size_t size() const noexcept { return X * Y; }

  [[nodiscard]] static constexpr std::size_t size_x() noexcept { return X; }
  [[nodiscard]] static constexpr std::size_t size_y() noexcept { return Y; }

  template <std::invocable<value_type> F>
  constexpr Matrix &
  elementwise(F f) noexcept(noexcept(f(std::declval<value_type>()))) {
    std::transform(begin(), end(), begin(), f);
    return *this;
  }
  template <std::invocable<value_type, value_type> F>
  constexpr Matrix &elementwise(Matrix const &m, F f) noexcept(
      noexcept(f(std::declval<value_type>(), std::declval<value_type>()))) {
    std::transform(begin(), end(), m.begin(), begin(), f);
    return *this;
  }
  constexpr Matrix operator+() const noexcept { return *this; }
  constexpr Matrix operator-() const noexcept {
    Matrix m = *this;
    return m.elementwise([](value_type x) noexcept { return -x; });
  }
  template <std::convertible_to<value_type> MT>
  constexpr Matrix &operator+=(Matrix<X, Y, MT> const &m) noexcept {
    return elementwise(
        m, [](value_type a, value_type b) noexcept { return a + b; });
  }
  template <std::convertible_to<value_type> MT>
  constexpr Matrix &operator-=(Matrix<X, Y, MT> const &m) noexcept {
    return elementwise(
        m, [](value_type a, value_type b) noexcept { return a - b; });
  }
  constexpr Matrix &operator+=(value_type x) noexcept {
    return elementwise([x](value_type a) noexcept { return a + x; });
  }
  constexpr Matrix &operator-=(value_type x) noexcept { return operator+=(-x); }
  constexpr Matrix &operator*=(value_type x) noexcept {
    return elementwise([x](value_type a) noexcept { return a * x; });
  }
  constexpr Matrix &operator/=(value_type x) { return operator*=(1.0 / x); }
  constexpr Matrix &operator%=(value_type x) noexcept {
    return elementwise([x](value_type a) noexcept { return a % x; });
  }

private:
  static constexpr std::array<value_type, X * Y>
  splat_(std::array<std::array<value_type, X>, Y> const &m) noexcept {
    std::array<value_type, X * Y> m_;
    for (std::size_t i = 0; i < X * Y; ++i) {
      m_[i] = m[i / X][i % X];
    }
    return m_;
  }
};

template <std::size_t N, typename T> using Vector = Matrix<1, N, T>;
template <std::size_t N> using MInds = Vector<N, std::size_t>;

template <std::size_t X, std::size_t Y, typename T>
constexpr Matrix<X, Y, T> eye(T v = T{1}) noexcept {
  static_assert(X == Y);
  Matrix<X, Y, T> m{};
  for (std::size_t i = 0; i < std::min(X, Y); ++i) {
    m[i, i] = v;
  }
  return m;
}
template <std::size_t Y, typename T = std::size_t>
constexpr Matrix<1, Y, T> iota(T value = T{0}) noexcept {
  Matrix<1, Y, T> m{};
  std::iota(m.begin(), m.end(), value);
  return m;
}

template <std::size_t X, std::size_t Y, std::integral T>
constexpr bool operator==(Matrix<X, Y, T> const &a,
                          Matrix<X, Y, T> const &b) noexcept {
  auto &&it = std::mismatch(a.begin(), a.end(), b.begin());
  return it.first == a.end();
}
template <std::size_t X, std::size_t Y, typename T>
constexpr Matrix<X, Y, T> operator+(Matrix<X, Y, T> const &a,
                                    Matrix<X, Y, T> const &b) noexcept {
  Matrix<X, Y, T> m = a;
  m += b;
  return m;
}
template <std::size_t X, std::size_t Y, typename T>
constexpr Matrix<X, Y, T> operator-(Matrix<X, Y, T> const &a,
                                    Matrix<X, Y, T> const &b) noexcept {
  Matrix<X, Y, T> m = a;
  m -= b;
  return m;
}
template <std::size_t X, std::size_t Y, typename T1, std::convertible_to<T1> T2>
constexpr Matrix<X, Y, T1> operator+(Matrix<X, Y, T1> const &a, T2 v) noexcept {
  Matrix<X, Y, T1> m = a;
  m += v;
  return m;
}
template <std::size_t X, std::size_t Y, typename T1, std::convertible_to<T1> T2>
constexpr Matrix<X, Y, T1> operator+(T2 v, Matrix<X, Y, T1> const &a) noexcept {
  return a + v;
}
template <std::size_t X, std::size_t Y, typename T1, std::convertible_to<T1> T2>
constexpr Matrix<X, Y, T1> operator-(Matrix<X, Y, T1> const &a, T2 v) noexcept {
  return a + (-v);
}
template <std::size_t X, std::size_t Y, typename T1, std::convertible_to<T1> T2>
constexpr Matrix<X, Y, T1> operator-(T2 v, Matrix<X, Y, T1> const &a) noexcept {
  return (-a) + v;
}
template <std::size_t X, std::size_t Y, typename T1, std::convertible_to<T1> T2>
constexpr Matrix<X, Y, T1> operator*(Matrix<X, Y, T1> const &a, T2 v) noexcept {
  Matrix<X, Y, T1> m = a;
  m *= v;
  return m;
}
template <std::size_t X, std::size_t Y, typename T1, std::convertible_to<T1> T2>
constexpr Matrix<X, Y, T1> operator*(T2 v, Matrix<X, Y, T1> const &a) noexcept {
  return a * v;
}
template <std::size_t X, std::size_t Y, typename T1, std::convertible_to<T1> T2>
constexpr Matrix<X, Y, T1> operator/(Matrix<X, Y, T1> const &a, T2 v) {
  return a * (T1{1} / v);
}
template <std::size_t X, std::size_t Y, typename T1, std::convertible_to<T1> T2>
constexpr Matrix<X, Y, T1> operator%(Matrix<X, Y, T1> const &a, T2 v) {
  Matrix<X, Y, T1> m = a;
  m %= v;
  return m;
}

template <std::size_t X, std::size_t Y, typename T>
constexpr T norm(Matrix<X, Y, T> const &a) noexcept {
  return sqrt(std::transform_reduce(a.begin(), a.end(), a.begin(), T{}));
}
template <std::size_t X, std::size_t Y, typename T>
constexpr T n(Matrix<X, Y, T> const &a) noexcept {
  return norm(a);
}
template <std::size_t X, std::size_t Y, typename T>
constexpr Matrix<Y, X, T> transpose(Matrix<X, Y, T> const &a) noexcept {
  return Matrix<Y, X, T>(a.tbegin(), a.tend());
}
template <std::size_t X, std::size_t Y, typename T>
constexpr Matrix<Y, X, T> t(Matrix<X, Y, T> const &a) noexcept {
  return transpose(a);
}

template <std::size_t X, std::size_t Y, std::size_t Z, typename T>
constexpr Matrix<Z, Y, T> operator*(Matrix<X, Y, T> const &a,
                                    Matrix<Z, X, T> const &b) noexcept {
  Matrix<Z, Y, T> m{};
  for (std::size_t i = 0; i < Z * Y; ++i) {
    auto z = i % Z;
    auto y = i / Z;
    m[z, y] = std::transform_reduce(a.begin() + y * X, a.begin() + (y + 1) * X,
                                    b.tbegin() + z * X, T{});
  }
  return m;
}
template <std::size_t X, std::size_t Y, typename T>
constexpr T dot(Matrix<X, Y, T> const &a, Matrix<X, Y, T> const &b) noexcept {
  return std::transform_reduce(a.begin(), a.end(), b.begin(), T{});
}

template <typename T>
constexpr Vector<3, T> cross(Vector<3, T> const &a,
                             Vector<3, T> const &b) noexcept {
  // clang-format off
  return Vector<3, T>{a[1] * b[2] - a[2] * b[1],
                      a[2] * b[0] - a[0] * b[2],
                      a[0] * b[1] - a[1] * b[0]};
  // clang-format on
}

template <std::size_t N, typename T>
constexpr T det(Matrix<N, N, T> const &a) noexcept {
  T res{};
  MInds<N - 1> rows = iota<N - 1>() + 1;
  MInds<N - 1> cols = iota<N - 1>() + 1;
  for (std::size_t i = 0; i < N; ++i) {
    T sign = T{1} - 2 * (i % 2);
    res += sign * a[i, 0] * det(a[cols, rows]);
    if (i < N - 1)
      cols[i] -= 1;
  }
  return res;
}

template <typename T> constexpr T det(Matrix<3, 3, T> const &a) noexcept {
  return a[0, 0] * a[1, 1] * a[2, 2] + a[0, 1] * a[1, 2] * a[2, 0] +
         a[0, 2] * a[1, 0] * a[2, 1] - a[0, 2] * a[1, 1] * a[2, 0] -
         a[0, 1] * a[1, 0] * a[2, 2] - a[0, 0] * a[1, 2] * a[2, 1];
}

template <typename T> constexpr T det(Matrix<2, 2, T> const &a) noexcept {
  return a[0, 0] * a[1, 1] - a[0, 1] * a[1, 0];
}

template <typename T> constexpr T det(Matrix<1, 1, T> const &a) noexcept {
  return a[0, 0];
}

namespace details_ {

template <typename T, std::size_t N> constexpr T staticpow(T val) {
  if constexpr (N == 0)
    return T{1};
  else
    return val * staticpow<T, N - 1>(val);
}
} // namespace details_
template <std::size_t N, typename T>
constexpr bool invertible(Matrix<N, N, T> const &a,
                          T eps = T{EPSMIN}) noexcept {
  auto det_ = det(a);
  auto volume = details_::staticpow<T, N>(n(a));
  return std::abs(det_) > eps * volume;
}

template <std::size_t N, typename T>
constexpr Matrix<N, N, T> inv(Matrix<N, N, T> const &a) noexcept {
  assert(invertible(a));
  Matrix<N, N, T> m{};
  MInds<N - 1> cols = iota<N - 1>() + 1;
  for (std::size_t x = 0; x < N; ++x) {
    MInds<N - 1> rows = iota<N - 1>() + 1;
    for (std::size_t y = 0; y < N; ++y) {
      Matrix<N - 1, N - 1, T> minor = a[cols, rows];
      auto d = det(minor);
      auto sign = T{1} - 2 * ((x + y) % 2);
      auto adj = sign * d;
      m[y, x] = adj;
      if (y < N - 1)
        rows[y] -= 1;
    }
    if (x < N - 1)
      cols[x] -= 1;
  }
  return m / det(a);
}

template <typename T>
constexpr Matrix<3, 3, T> inv(Matrix<3, 3, T> const &a) noexcept {
  assert(invertible(a));
  // clang-format off
  Matrix<3, 3, T> adj {
    {a[1, 1] * a[2, 2] - a[2, 1] * a[1, 2],   a[2, 0] * a[1, 2] - a[1, 0] * a[2, 2],   a[1, 0] * a[2, 1] - a[2, 0] * a[1, 1]},
    {a[2, 1] * a[0, 2] - a[0, 1] * a[2, 2],   a[0, 0] * a[2, 2] - a[2, 0] * a[0, 2],   a[2, 0] * a[0, 1] - a[0, 0] * a[2, 1]},
    {a[0, 1] * a[1, 2] - a[1, 1] * a[0, 2],   a[1, 0] * a[0, 2] - a[0, 0] * a[1, 2],   a[0, 0] * a[1, 1] - a[1, 0] * a[0, 1]},
  };
  // clang-format on
  return adj / det(a);
}

template <typename T>
constexpr Matrix<2, 2, T> inv(Matrix<2, 2, T> const &a) noexcept {
  assert(invertible(a));
  Matrix<2, 2, T> adj{{a[1, 1], -a[1, 0]}, {-a[0, 1], a[0, 0]}};
  return adj / det(a);
}

template <typename T>
constexpr Matrix<1, 1, T> inv(Matrix<1, 1, T> const &a) noexcept {
  assert(invertible(a));
  return Matrix<1, 1, T>{T{1} / a[0, 0]};
}

template <std::size_t X, std::size_t Y, typename T>
std::ostream &operator<<(std::ostream &os, Matrix<X, Y, T> const &m) {
  for (std::size_t y = 0; y < Y; ++y) {
    for (std::size_t x = 0; x < X; ++x) {
      os << m[x, y] << " ";
    }
    os << "\n";
  }
  return os;
}

} // namespace cpp_contests
