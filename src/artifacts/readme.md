# Artifacts

For an exhibition, it is required to select $n$ artifacts from different epochs, such that each artifact comes from a distinct epoch.

In storage, there are $m$ artifacts available for each of the $n$ epochs
(that is, $m$ artifacts from the first epoch, $m$ artifacts from the second epoch, and so on, for a total of $n \cdot m$ artifacts).

An exhibition is considered successful if the selected artifacts can be arranged in a sequence
such that the sum of absolute differences between the values of adjacent artifacts is minimized:

$$
|a_2 - a_1| + |a_3 - a_2| + \dots + |a_n - a_{n-1}|
$$

Your task is to choose the artifacts and determine the order in which they should be arranged.

---

## Input format

The first line contains two integers $n$ and $m$ $(1 \le n \cdot m \le 10^5)$ — the number of epochs and the number of artifacts in each epoch.

Each of the next $n$ lines contains $m$ integers $a_i$ $(1 \le a_i \le 10^9)$, where $a_i$ is the value of the $i$-th artifact of that epoch.

---

## Output Format

Output a sequence of $n$ integers — the values of the selected artifacts.

If there are multiple valid answers, output the one with the minimum possible sum of all selected values.
