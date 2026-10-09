# Postman

Find the minimum sum of distances from a point on a line to `n` house coordinates.
The 32-bit state starts at zero and is updated as `cur = cur * a + b` with unsigned overflow.
Each `nextRand24()` updates the state and returns `cur >> 8`.
A coordinate is `(first << 8) ^ second` for two consecutive `nextRand24()` results.

Input contains `n` (`1 ≤ n ≤ 10⁷`), followed by `a` and `b` (`1 ≤ a, b ≤ 10⁹`).
Output the minimum total distance.

Sorting algorithms from the C++ standard library (`std`, including `std::ranges`) must not be used.
