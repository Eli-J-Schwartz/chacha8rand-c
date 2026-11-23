# chacha8rand-c
C implementation of the ChaCha8Rand Random Number Generator.

## Instructions for testing:
1. Compile with `clang test.c chacha8rand.c -mavx512vl -mavx512bw -g`.
2. Run tests with `./a.out > actual.txt`.
3. Compare result with `diff actual.txt expected.txt`.
