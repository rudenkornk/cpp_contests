# Memory Manager

Contest 3, problem C.

| Limit        | All languages | Scala 2.9.1 | Haskell 7.10.2 |
| ------------ | ------------- | ----------- | -------------- |
| Time limit   | 0.5 seconds   | 4 seconds   | 2 seconds      |
| Memory limit | 64 MB         | 128 MB      | 256 MB         |

**Input:** standard input or `input.txt`.
**Output:** standard output or `output.txt`.

Petya has been asked to write a memory manager for the new standard library of the H++ language.
The manager has an array of $N$ consecutive memory cells numbered from $1$ to $N$.
Its job is to process applications' requests to allocate and release memory.

An allocation request has one parameter $K$ and asks for $K$ consecutive memory cells.
If at least one free contiguous block of $K$ cells exists, the manager must allocate such a block.
The manager allocates memory from the longest free block.
If several blocks have the maximum length, it chooses the one with the smallest first-cell number.
The allocated cells then become occupied and cannot be allocated again until they are released.
If no free block of $K$ consecutive cells exists, the request is rejected.

A release request has one parameter $T$.
It asks the manager to release the memory allocated when processing request number $T$.
Requests are numbered starting from $1$.
It is guaranteed that request $T$ is an allocation request and that its memory has not been released before.
Released cells may be allocated again.
If request $T$ was rejected, the release request is ignored.

Simulate a memory manager satisfying these rules.

## Input Format

The first line contains $N$ and $M$: the number of memory cells and the number of requests,
where $1 \le N \le 2^{31} - 1$ and $1 \le M \le 10^5$.
Each of the next $M$ lines contains one integer.
The $(i + 1)$-st input line contains:

- A positive integer $K$ if request $i$ allocates $K$ cells, where $1 \le K \le N$.
- A negative integer $-T$ if request $i$ releases memory allocated by request $T$, where $1 \le T < i$.

## Output Format

For each allocation request, output one integer on its own line.
If memory was allocated, output the number of the first cell in the allocated block.
Otherwise, output `-1`.

## Example

Input:

```text
6 8
2
3
-1
3
3
-5
2
2
```

Output:

```text
1
3
-1
-1
1
-1
```

## Restrictions

The following C++ classes are prohibited: `map`, `set`, `multimap`, `multiset`, `unordered_map`,
`unordered_set`, `unordered_multimap`, and `unordered_multiset`.
