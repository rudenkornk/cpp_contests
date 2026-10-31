# Binary Search Tree Traversal

Problem C: Binary Search Tree Traversal.

| Time limit  | Memory limit |
| ----------- | ------------ |
| 0.5 seconds | 64 MB        |

**Input:** standard input or `input.txt`.
**Output:** standard output or `output.txt`.

A binary search tree is given in the input.
For every node, all keys in its left subtree are strictly smaller than its key,
and all keys in its right subtree are greater than or equal to its key.
The tree contains at most $100\,000$ nodes.
Key values range from $0$ to $1\,000\,000\,000$.

## Input Format

The first line contains the number of nodes in the tree.
The second line contains the keys of the tree in preorder.

## Output Format

Output two lines.
The first line must contain the keys of the tree in postorder,
and the second line must contain the keys of the tree in inorder.

## Example 1

Input:

```text
7
4 2 1 3 6 5 7
```

Output:

```text
1 3 2 5 7 6 4
1 2 3 4 5 6 7
```

## Example 2

Input:

```text
6
5 3 2 3 5 6
```

Output:

```text
2 3 3 6 5 5
2 3 3 5 5 6
```
