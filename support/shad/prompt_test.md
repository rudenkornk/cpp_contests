Generate several DIFFERENT static test cases.
The tests must be diverse and GOOD, meaning they should cover different aspects of the problem.
The tests must be static, meaning they must be runnable as `cat test_1 | ./a.out`.
Each test must have a static expected output in a file named `test_1.answer`.

There must be between 4 and 16 tests, including test 0.
Test 0 must match the sample from the problem statement. If there are multiple samples, use the most complex.
First couple of tests should check LOWER limits, i.e. the minimum number of values, minimum ranges, etc.
All or the rest (and MOST) tests must push the specified limits (e.g., the maximum number of values, maximum ranges, etc.).

The tests will be checked using the `./run_tests.sh` script.
You may generate them using any method, such as Python.
Do not commit anything.
