# Sliding k-th Order Statistic

Problem D: On the Sliding k-th Order Statistic.

| Time limit | Memory limit |
| ---------- | ------------ |
| 1 second   | 64 MB        |

**Input:** standard input or `input.txt`.
**Output:** standard output or `output.txt`.

Two pointers, l and r, move along an array, initially both pointing to its first element.
There are two operations: L moves l one element to the right, and R moves r one element to the right.
After each operation, output the k-th number in ascending order among the elements from l through r, inclusive,
or -1 if this interval contains fewer than k elements.

## Input Format

The first line contains three integers n, m, and k.
The second line contains n integers specifying the array along which l and r move.
The third line contains m characters, R or L, without spaces, specifying the sequence of operations.

It is guaranteed that l never moves past r and that r never moves outside the array.
The constraints are 1 ≤ n, k ≤ 100000 and 0 ≤ m ≤ 2n - 2.
All array elements are nonnegative integers not exceeding 10⁹.

## Output Format

Output exactly m lines, each containing exactly one integer.
After each operation, output the k-th number in ascending order among the elements from l through r, inclusive,
or -1 if this interval contains fewer than k elements.

## Example 1

Input:

```text
7 4 2
4 2 1 3 6 5 7
RRLL
```

Output:

```text
4
2
2
-1
```

## Example 2

Input:

```text
4 6 1
1 2 3 4
RLRLRL
```

Output:

```text
1
2
2
3
3
4
```
