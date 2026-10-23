# Min-Max Arrays

Contest 1, problem C: Minimum Maximum.

| Time limit | Memory limit |
| ---------- | ------------ |
| 2 seconds  | 256 MB       |

**Input:** standard input or `input.txt`.
**Output:** standard output or `output.txt`.

You are given $n$ nondecreasing arrays $A_i$ and $m$ nonincreasing arrays $B_j$.
All arrays have the same length $l$.
Then $q$ queries of the form $(i, j)$ follow.
For each query, find an index $k$ minimizing $\max(A_{i,k}, B_{j,k})$.
If several such indices exist, any of them may be returned.

## Input Format

The first line contains $n$, $m$, and $l$, where $1 \le n, m \le 900$ and $1 \le l \le 3000$.
The next $n$ lines describe the arrays $A_i$, each as a list of $l$ elements.
Array elements are integers from $0$ to $10^5 - 1$.
The next $m$ lines describe the arrays $B_j$ in the same format.
Arrays and their elements are numbered starting from $1$.

The next line contains the number of queries $q$, where $1 \le q \le n \cdot m$.
Each of the next $q$ lines contains a pair $i$, $j$, where $1 \le i \le n$ and $1 \le j \le m$.

## Output Format

Output $q$ integers from $1$ to $l$: the answers to the queries.

## Example

Input:

```text
4 3 5
1 2 3 4 5
1 1 1 1 1
0 99999 99999 99999 99999
0 0 0 0 99999
5 4 3 2 1
99999 99999 99999 0 0
99999 99999 0 0 0
12
1 1
1 2
1 3
2 1
2 2
2 3
3 1
3 2
3 3
4 1
4 2
4 3
```

Output:

```text
3
4
3
5
4
3
1
4
3
4
4
4
```

## Restrictions

The following C++ classes are prohibited: `map`, `set`, `multimap`, `multiset`, `unordered_map`,
`unordered_set`, `unordered_multimap`, `unordered_multiset`, and `priority_queue`.
