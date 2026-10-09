# Football Team

Contest 2, problem B.

| Time limit  | Memory limit |
| ----------- | ------------ |
| 0.5 seconds | 64 MB        |

**Input:** standard input or `input.txt`.
**Output:** standard output or `output.txt`.
**Problem type:** practical, with code review.

You want to assemble a football team.
Each player's efficiency is described by a single integer.
The larger the number, the more effective the player.

Every team must be cohesive.
If one player is much better than all the others, the others will dislike that player and the team will fall apart.
Therefore, the efficiency of any player on the team must not exceed the sum of the efficiencies
of any two other players on the team.

Choose a team satisfying this cohesion condition and having the largest possible total efficiency.

## Input Format

The first line contains $n$, where $1 \le n \le 10^5$.
The second line contains the efficiencies of the $n$ players:
positive integers not exceeding $2^{31} - 1$.

## Output Format

Output two lines:

- The first line contains the largest possible total efficiency of the team.
- The second line contains the indices of all selected players in increasing order.

## Examples

### Example 1

Input:

```text
5
3 2 5 4 1
```

Output:

```text
14
1 2 3 4
```

### Example 2

Input:

```text
5
1 2 4 8 16
```

Output:

```text
24
4 5
```

## Notes

Players are numbered from $1$ to $n$ in the order in which their efficiencies appear in the input.
If several teams have the maximum total efficiency, output any of them.

## Restrictions

The following C++ classes are prohibited: `map`, `set`, `multimap`, `multiset`, `unordered_map`,
`unordered_set`, `unordered_multimap`, `unordered_multiset`, and `priority_queue`.
