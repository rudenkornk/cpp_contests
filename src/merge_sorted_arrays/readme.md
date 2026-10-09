# Merge Sorted Arrays

Contest 2, problem C: Merging Sorted Sequences.

| Time limit | Memory limit |
| ---------- | ------------ |
| 1 second   | 64 MB        |

**Input:** standard input or `input.txt`.
**Output:** standard output or `output.txt`.

You are given $n$ sequences of integers sorted in ascending order, each of length $m$.
Merge them into one sequence of $mn$ integers sorted in ascending order.

**Requirements:** the algorithm's time complexity must not exceed $O(mn \log n)$,
and its memory usage must be $O(mn)$.

## Input Format

The first line contains two integers $n$ and $m$, where $1 \le m, n \le 1000$.
Each of the next $n$ lines contains $m$ integers sorted in ascending order.
The absolute values of the integers do not exceed $10^9$.

## Output Format

Output all $mn$ integers in ascending order.

## Examples

### Example 1

Input:

```text
2 5
1 3 5 7 9
2 4 6 8 10
```

Output:

```text
1 2 3 4 5 6 7 8 9 10
```

### Example 2

Input:

```text
4 2
1 4
2 8
3 7
5 6
```

Output:

```text
1 2 3 4 5 6 7 8
```

## Restrictions

The following C++ classes are prohibited: `map`, `set`, `multimap`, `multiset`, `unordered_map`,
`unordered_set`, `unordered_multimap`, `unordered_multiset`, and `priority_queue`.
