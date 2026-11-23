#include <stdint.h>

typedef struct {
    uint8_t internal_state[1024];
    uint16_t position_in_state;
} chacha8rand_state;

void chacha8rand_seed(chacha8rand_state* state, uint8_t seed[32]);

void chacha8rand_next(uint8_t* out, chacha8rand_state* state, uint64_t count);
