# Number of Binary Search Trees

Problem A: Number of Binary Search Trees.

| Time limit  | Memory limit |
| ----------- | ------------ |
| 1.5 seconds | 64 MB        |

**Input:** standard input or `input.txt`.
**Output:** standard output or `output.txt`.

Count the number of distinct binary search trees consisting of $n$ vertices with keys $a_1, a_2, \ldots, a_n$.
Only trees in which all keys in the left subtree are strictly smaller than the root key,
and all keys in the right subtree are greater than or equal to the root key, are considered.
This condition must hold at every vertex.

Two trees are considered identical if their drawings look identical.
Formally, two trees are identical if their preorder traversals produce the same sequence of numbers.
The answer can be very large, so calculate it modulo $123\,456\,789$.
Key values range from $-2^{31}$ to $2^{31} - 1$.

## Input Format

The first line contains $n$, where $1 \le n \le 400$.
The second line contains $n$ integers $a_1, a_2, \ldots, a_n$.

## Output Format

Output the number of distinct binary search trees modulo $123\,456\,789$.

## Example 1

Input:

```text
2
2 1
```

Output:

```text
2
```

## Example 2

Input:

```text
3
10 10 10
```

Output:

```text
1
```

## Example 3

Input:

```text
3
1 2 3
```

Output:

```text
5
```
