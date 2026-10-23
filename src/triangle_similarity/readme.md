# Triangle Similarity Classes

Problem E: Triangle Similarity Classes.

| Time limit | Memory limit |
| ---------- | ------------ |
| 2 seconds  | 256 MB       |

**Input:** standard input or `input.txt`.
**Output:** standard output or `output.txt`.

You are given a collection of $n$ triangles, where $1 \le n \le 10^6$.
Each triangle is specified by three integers: the lengths of its sides.
Side lengths are positive integers not exceeding $10^9$.

Some pairs of triangles are similar.
The properties of similarity imply that the triangles can be partitioned into classes,
with all triangles in each class similar to one another.
For example, the triangles with sides $(6, 6, 10)$, $(15, 25, 15)$, and $(35, 21, 21)$ are pairwise similar
and therefore form one class.
Find the number of similarity classes into which the given $n$ triangles are partitioned.

The algorithm's complexity must be linear in $n$ on average, though it may also depend on the bounds on side lengths.
In particular, sorting long arrays is not allowed in the solution.

## Input Format

The first line contains the number $n$.
Each of the following $n$ lines contains three integers specifying the side lengths of one triangle.

## Output Format

Output the number of similarity classes.

## Example 1

Input:

```text
3
6 6 10
15 25 15
35 21 21
```

Output:

```text
1
```

## Example 2

Input:

```text
4
3 4 5
10 11 12
6 7 8
6 8 10
```

Output:

```text
3
```
