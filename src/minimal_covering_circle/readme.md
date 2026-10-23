# Minimal Covering Circle

Contest 1, problem D.

| Time limit  | Memory limit |
| ----------- | ------------ |
| 0.5 seconds | 64 MB        |

**Input:** standard input or `input.txt`.
**Output:** standard output or `output.txt`.

You are given $n$ points on the plane with coordinates $(x_1, y_1), (x_2, y_2), \ldots, (x_n, y_n)$.
Cover at least $k$ of these points with a circle whose center lies on the $x$-axis.
Find the smallest possible radius of such a circle with an accuracy of $10^{-3}$.

## Input Format

The first line contains $n$ and $k$, where $1 \le k \le n \le 10000$.
Each of the next $n$ lines contains two integers: the coordinates of a point.
The absolute values of the coordinates do not exceed $1000$.

## Output Format

Output the radius with exactly six digits after the decimal point.
Your answer must differ from the correct answer by at most $10^{-3}$.
Six digits must still be printed to avoid rounding errors.

## Examples

### Example 1

Input:

```text
3 3
0 5
3 4
-4 -3
```

Output:

```text
5.000000
```

### Example 2

Input:

```text
3 2
0 1
2 1
1 100
```

Output:

```text
1.414246
```

## Restrictions

The following C++ classes are prohibited: `map`, `set`, `multimap`, `multiset`, `unordered_map`,
`unordered_set`, `unordered_multimap`, `unordered_multiset`, and `priority_queue`.
