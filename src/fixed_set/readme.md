# Fixed Set

Contest 3, problem A.

| Time limit  | Memory limit |
| ----------- | ------------ |
| 1.5 seconds | 64 MB        |

**Input:** standard input or `input.txt`.
**Output:** standard output or `output.txt`.

Implement the following class for storing a set of integers:

```cpp
class FixedSet {
public:
  FixedSet();
  void Initialize(const vector<int>& numbers);
  bool Contains(int number) const;
};
```

When `Initialize` is called, `FixedSet` receives the integers that it will subsequently store.
The set does not change until the next call to `Initialize`.
The `Contains` operation returns `true` if `number` belongs to the set.

The expected running time of `Initialize` must be $O(n)$, where $n$ is the number of integers in `numbers`.
Memory usage must be $O(n)$ in the worst case.
The `Contains` operation must run in $O(1)$ time in the worst case.

Use this class to solve the following model problem.
The input contains a set of distinct integers followed by integer queries.
For each query, determine whether its integer belongs to the set.

## Input Format

Input, output, and `main` are already provided in the
[official wrapper](https://gist.github.com/astiunov/810433327e6b4c9f6ebd60ddb2b00430).
The same program is run on the server.
The submitted file must be a header implementing `FixedSet`.
The wrapper also contains a small number of unit tests that run on the server.
The implementation must pass those tests as well as the contest tests.
To run the wrapper's unit tests, pass the `--testing` flag to the program.

The problem constructs a set of $n$ distinct integers and then performs $q$ queries,
where $1 \le n \le 100000$ and $1 \le q \le 1000000$.
The absolute values of all integers do not exceed $10^9$.

## Output Format

For each query, output `Yes` if its integer belongs to the set, or `No` otherwise, each on its own line.

## Examples

### Example 1

Input:

```text
3
1 2 3
4
1 2 3 4
```

Output:

```text
Yes
Yes
Yes
No
```

### Example 2

Input:

```text
3
3 1 2
4
10 1 4 2
```

Output:

```text
No
Yes
No
Yes
```

## Restrictions

The following C++ classes are prohibited: `map`, `set`, `multimap`, `multiset`, `unordered_map`,
`unordered_set`, `unordered_multimap`, and `unordered_multiset`.
