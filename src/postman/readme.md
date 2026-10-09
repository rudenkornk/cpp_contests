# Postman

Contest 2, problem D: One-Dimensional Postman.

| Time limit | Memory limit |
| ---------- | ------------ |
| 1 second   | 256 MB       |

**Input:** standard input or `input.txt`.
**Output:** standard output or `output.txt`.

There are $n$ people living in the village of Pechalka.
Their houses lie on the $x$-axis: the house of person $i$ is located at $x_i$.

A postman has arrived in the village and wants to choose the coordinate $y$ of his house
so that the total distance to all residents is minimized:

$$
\sum_{i=1}^{n} |y - x_i| \longrightarrow \min.
$$

You are given an array $x$ of $n$ random integers.
Find the smallest possible total distance.

## Input Format

The first line contains $n$, where $1 \le n \le 10^7$.
The second line contains two integers $a$ and $b$, where $1 \le a, b \le 10^9$:
the parameters of the random number generator.

### Coordinate Generator

```cpp
unsigned int cur = 0; // An unsigned 32-bit integer.

unsigned int nextRand24() {
  cur = cur * a + b; // Computed with unsigned overflow.
  return cur >> 8;  // A number from 0 to 2**24 - 1.
}

unsigned int nextRand32() {
  unsigned int a = nextRand24();
  unsigned int b = nextRand24();
  return (a << 8) ^ b; // A number from 0 to 2**32 - 1.
}
```

The elements $x_1, x_2, \ldots, x_n$ are generated consecutively.
Each element is the result of the next call to `nextRand32()`.

## Output Format

Output one integer: the minimum total distance from $y$ to all houses.

## Example

Input:

```text
6
239 13
```

Output:

```text
8510257371
```

## Restrictions

Using standard-library sorting functions and `std::nth_element` is prohibited.

The following C++ classes are prohibited: `map`, `set`, `multimap`, `multiset`, `unordered_map`,
`unordered_set`, `unordered_multimap`, `unordered_multiset`, and `priority_queue`.
