# Treap Height

Problem D: Treap Height.

| Time limit  | Memory limit |
| ----------- | ------------ |
| 0.5 seconds | 64 MB        |

**Input:** standard input or `input.txt`.
**Output:** standard output or `output.txt`.

A treap is a binary tree whose nodes store pairs $(x, y)$, where $x$ is a key and $y$ is a priority.
It is a binary search tree with respect to $x$ and a heap with respect to $y$.
Assume that all keys are distinct and all priorities are distinct.
If a node contains $(x_0, y_0)$, every node in its left subtree has $x < x_0$,
every node in its right subtree has $x > x_0$, and every node in either subtree has $y < y_0$.

Consider a random permutation of the integers from $1$ to $n$: $\pi = (i_1, i_2, \ldots, i_n)$.
All permutations are equally likely.
Build a treap from the pairs $(1, i_1), (2, i_2), \ldots, (n, i_n)$,
where the first component is the key and the second is the priority.

Determine the probability that the treap has height exactly $h$.
The height of a treap is the height of its tree.
A tree consisting of a single node has height zero.

## Input Format

The only input line contains two integers $n$ and $h$, where $1 \le n \le 100$ and $0 \le h \le 10^9$.

## Output Format

Output the required probability with an absolute or relative error of at most $10^{-6}$.
To avoid precision issues, print the answer with ten digits after the decimal point.

## Example 1

Input:

```text
1 0
```

Output:

```text
1
```

## Example 2

Input:

```text
1 1
```

Output:

```text
0
```

## Example 3

Input:

```text
3 2
```

Output:

```text
0.666667
```
