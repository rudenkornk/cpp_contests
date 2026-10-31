# External Rectangles

Problem B: External Rectangles.

| Time limit | Memory limit |
| ---------- | ------------ |
| 1 second   | 64 MB        |

**Input:** standard input or `input.txt`.
**Output:** standard output or `output.txt`.

There are $n$ rectangles drawn in the plane, with vertices at integer coordinates and sides parallel to the coordinate axes.
The boundaries of any two rectangles have no points in common.
In other words, any two rectangles are either separate from each other, or one lies strictly inside the other.
Some rectangles are external: they do not lie inside any other rectangle.
The remaining rectangles are internal.
Count the external rectangles.

## Input Format

The first line contains the number of rectangles $n$, where $1 \le n \le 10^5$.
Each of the next $n$ lines contains four integers $x_1$, $y_1$, $x_2$, $y_2$,
where $-10^9 \le x_1, y_1, x_2, y_2 \le 10^9$.
They specify the coordinates of two opposite vertices of the corresponding rectangle.

## Output Format

Output a single integer: the number of external rectangles.

## Example 1

Input:

```text
3
-3 -3 3 3
-2 2 2 -2
-1 -1 1 1
```

Output:

```text
1
```

## Example 2

Input:

```text
4
0 0 3 3
1 1 2 2
100 100 101 101
200 200 201 201
```

Output:

```text
3
```

## Restrictions

The C++ class `std::set` is prohibited.
