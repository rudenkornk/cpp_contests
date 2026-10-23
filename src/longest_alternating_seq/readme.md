# Longest Alternating Subsequence

Contest 1, problem A.

| Time limit  | Memory limit |
| ----------- | ------------ |
| 0.5 seconds | 64 MB        |

**Input:** standard input or `input.txt`.
**Output:** standard output or `output.txt`.

Find a longest alternating subsequence $a_{i_1}, a_{i_2}, \ldots, a_{i_k}$ of the sequence
$a_1, a_2, \ldots, a_n$.
The indices must satisfy $i_1 < i_2 < \cdots < i_k$, any two consecutive elements must be different,
and any three consecutive elements must satisfy either
$a_{i_{j-1}} < a_{i_j} > a_{i_{j+1}}$ or $a_{i_{j-1}} > a_{i_j} < a_{i_{j+1}}$.
The length $k$ must be as large as possible.

## Input Format

The first line contains the number $n$.
The second line contains $n$ integers $a_1, a_2, \ldots, a_n$,
where $1 \le n \le 1000$ and $|a_i| \le 10^9$.

## Output Format

Output the subsequence $a_{i_1}, a_{i_2}, \ldots, a_{i_k}$ itself.
If several longest subsequences exist, choose one with the smallest $i_1$.
Among those with the same $i_1$, choose one with the smallest $i_2$, and so on.

## Examples

### Example 1

Input:

```text
10
1 4 2 3 5 8 6 7 9 10
```

Output:

```text
1 4 2 8 6 7
```

### Example 2

Input:

```text
5
1 2 3 4 5
```

Output:

```text
1 2
```

### Example 3

Input:

```text
1
100
```

Output:

```text
100
```

## Restrictions

The following C++ classes are prohibited: `map`, `set`, `multimap`, `multiset`, `unordered_map`,
`unordered_set`, `unordered_multimap`, `unordered_multiset`, and `priority_queue`.
