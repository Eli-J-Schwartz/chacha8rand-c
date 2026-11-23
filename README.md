# chacha8rand-c
C implementation of the ChaCha8Rand Random Number Generator.

## Instructions for testing:
Compile with `clang test.c chacha8rand.c -mavx512vl -mavx512bw -g`.
Run tests with `./a.out > actual.txt`.
Compare result with `diff actual.txt expected.txt`.
