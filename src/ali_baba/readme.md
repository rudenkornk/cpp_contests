# Ali Baba

Contest 2, problem E: Ali Baba Collects Coins.

| Time limit  | Memory limit |
| ----------- | ------------ |
| 0.5 seconds | 64 MB        |

**Input:** standard input or `input.txt`.
**Output:** standard output or `output.txt`.

Gold coins are scattered at several places along a road.
Each coin's location is given by one integer: its distance in meters from the starting mark.
All coins lie to the right of that mark.

Ali Baba runs along the road and collects coins, starting at time $0$.
He runs exactly one meter per second.

Each coin has a deadline by which it must be collected, inclusive; otherwise, it disappears.

Ali Baba must collect all coins in the smallest possible time.
He may start at any point on the line and collect the coins in any order,
but he must collect all of them before their deadlines while minimizing the time spent.

## Input Format

The first line contains the number of coins $n$, where $1 \le n \le 1000$.
Each of the next $n$ lines contains two integers:
the coin's position and its collection deadline in seconds.
Coin coordinates range from $0$ to $10^6$.
The deadlines also do not exceed $10^6$.

## Output Format

Output one integer: the minimum time in which Ali Baba can collect all coins.
If collecting all coins before their deadlines is impossible, output `No solution`.

## Examples

### Example 1

Input:

```text
5
1 3
3 1
5 8
8 19
10 15
```

Output:

```text
11
```

### Example 2

Input:

```text
5
1 5
2 1
3 4
4 2
5 3
```

Output:

```text
No solution
```

## Restrictions

The following C++ classes are prohibited: `map`, `set`, `multimap`, `multiset`, `unordered_map`,
`unordered_set`, `unordered_multimap`, `unordered_multiset`, and `priority_queue`.
