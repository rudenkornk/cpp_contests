# Tree Isomorphism

Contest 3, problem B: Two Kingdoms.

| Limit        | All languages | Haskell 7.10.2 |
| ------------ | ------------- | -------------- |
| Time limit   | 1 second      | 10 seconds     |
| Memory limit | 42 MB         | 256 MB         |

**Input:** standard input or `input.txt`.
**Output:** standard output or `output.txt`.

This morning, Bob boasted to Joe that he had recently visited an amazing kingdom $X$.
Listening to his story, Joe suddenly realized that Bob's kingdom sounded very much like one
that Joe himself had visited about fifty years ago.
Joe has fond memories of that trip and has even kept an old map of kingdom $Y$ showing all its cities.

The map shows $N$ cities, each with a distinct integer number from $1$ to $N$.
Some pairs of cities are connected by roads that can be traveled in either direction.
The kingdom is arranged so that there is exactly one way to travel from any city to any other,
possibly passing through other cities along the way.

Bob, being a true traveler, visited every city in kingdom $X$ and drew a complete map of it.
Surprisingly, this kingdom also has $N$ cities numbered from $1$ to $N$.
Bob's map also shows that there is exactly one way to travel between any two cities,
possibly passing through other cities.

Over fifty years, the numbering of the cities has changed completely.
Joe will be very grateful if you determine whether Bob really visited the same kingdom that Joe once did.
If so, Joe wants to know the new number of each city.

## Input Format

The first line contains the number of cities $N$, where $1 \le N \le 10^5$.
The next $N - 1$ lines describe kingdom $Y$.
Each line contains the numbers of two cities connected by a road.
The following $N - 1$ lines describe kingdom $X$ in the same format.

## Output Format

If Joe is mistaken and the kingdoms are different, output `-1`.
Otherwise, output $N$ integers, each on its own line.
The $i$-th integer must be the new number of city $i$: its corresponding number in kingdom $X$.
If several solutions exist, output any of them.

## Example

Input:

```text
4
3 1
2 3
4 3
2 4
4 1
3 4
```

Output:

```text
1
2
4
3
```

## Restrictions

The following C++ classes are prohibited: `map`, `set`, `multimap`, `multiset`, `unordered_map`,
`unordered_set`, `unordered_multimap`, and `unordered_multiset`.
