# Two Pointers Maximum

Contest 2, problem A: Sliding Window Maximum.

| Time limit  | Memory limit |
| ----------- | ------------ |
| 0.6 seconds | 64 MB        |

**Input:** standard input or `input.txt`.
**Output:** standard output or `output.txt`.

You are given an array of $n$ integers.
Two pointers $l$ and $r$ move over this array, where $1 \le l, r \le n$.
Initially, both point to the first element: $l = r = 1$.

Both pointers may move only to the right, one position at a time.
The pointer $l$ never moves to the right of $r$, and neither pointer leaves the array.

After each pointer movement, determine the maximum of all elements from $l$ to $r$, inclusive.

**Requirement:** the amortized cost of processing each movement and computing the maximum must be $O(1)$.

## Input Format

The first line contains the array size $n$, where $1 \le n \le 10^5$.
The second line contains $n$ integers from $-10^9$ to $10^9$: the array elements.
The third line contains the number of movements $m$, where $0 \le m \le 2n - 2$.
The fourth line contains $m$ space-separated characters `L` or `R`:

- `L`: move the pointer $l$ one position to the right.
- `R`: move the pointer $r$ one position to the right.

## Output Format

Output exactly $m$ integers on one line.
The $i$-th integer is the maximum on the interval from $l$ to $r$ after the $i$-th operation.

## Example

Input:

```text
10
1 4 2 3 5 8 6 7 9 10
12
R R L R R R L L L R L L
```

Output:

```text
4 4 4 4 5 8 8 8 8 8 8 6
```

## Restrictions

The following C++ classes are prohibited: `map`, `set`, `multimap`, `multiset`, `unordered_map`,
`unordered_set`, `unordered_multimap`, `unordered_multiset`, and `priority_queue`.
