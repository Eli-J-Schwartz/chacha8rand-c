#include <emmintrin.h>
#include <immintrin.h>
#include "chacha8rand.h"

#define SEED_OFFSET 992

#define QUARTER_ROUND(a,b,c,d)\
a=_mm_add_epi32(a,b);\
d=_mm_xor_epi32(d,a);\
d=_mm_xor_epi32(_mm_slli_epi32(d,16),_mm_srli_epi32(d,16));\
c=_mm_add_epi32(c,d);\
b=_mm_xor_epi32(b,c);\
b=_mm_xor_epi32(_mm_slli_epi32(b,12),_mm_srli_epi32(b,20));\
a=_mm_add_epi32(a,b);\
d=_mm_xor_epi32(d,a);\
d=_mm_xor_epi32(_mm_slli_epi32(d,8),_mm_srli_epi32(d,24));\
c=_mm_add_epi32(c,d);\
b=_mm_xor_epi32(b,c);\
b=_mm_xor_epi32(_mm_slli_epi32(b,7),_mm_srli_epi32(b,25));\

#define FULL_ROUND(l)\
QUARTER_ROUND(l[0],l[4],l[8],l[12])\
QUARTER_ROUND(l[1],l[5],l[9],l[13])\
QUARTER_ROUND(l[2],l[6],l[10],l[14])\
QUARTER_ROUND(l[3],l[7],l[11],l[15])\
\
QUARTER_ROUND(l[0],l[5],l[10],l[15])\
QUARTER_ROUND(l[1],l[6],l[11],l[12])\
QUARTER_ROUND(l[2],l[7],l[8],l[13])\
QUARTER_ROUND(l[3],l[4],l[9],l[14])\


void chacha8rand_regenerate(chacha8rand_state* state) {
    state->position_in_state = 0;
    for (int i = 0; i < 4; i++) {
        __m128i chacha8_state[16];
        chacha8_state[0] = _mm_set1_epi32(0x61707865);
        chacha8_state[1] = _mm_set1_epi32(0x3320646e);
        chacha8_state[2] = _mm_set1_epi32(0x79622d32);
        chacha8_state[3] = _mm_set1_epi32(0x6b206574);
        for (int j = 0; j < 8; j++) {
            chacha8_state[j+4] = _mm_set1_epi32(*(uint32_t*)&state->internal_state[SEED_OFFSET+4*j]);
        }
        chacha8_state[12] = _mm_set_epi32(i*4+3, i*4+2, i*4+1, i*4+0);
        chacha8_state[13] = _mm_set1_epi32(0);
        chacha8_state[14] = _mm_set1_epi32(0);
        chacha8_state[15] = _mm_set1_epi32(0);
        
        FULL_ROUND(chacha8_state)
        FULL_ROUND(chacha8_state)
        FULL_ROUND(chacha8_state)
        FULL_ROUND(chacha8_state)

        for (int j = 0; j < 8; j++) {
            chacha8_state[j+4] = _mm_add_epi32(chacha8_state[j+4], _mm_set1_epi32(*(uint32_t*)&state->internal_state[SEED_OFFSET+4*j]));
        }

        for (int j = 0; j < 16; j++) {
            _mm_storeu_epi8(state->internal_state+(i*16+j)*16, chacha8_state[j]);
        }
    }
}

void chacha8rand_seed(chacha8rand_state* state, uint8_t seed[32]) {
    for (int i = 0; i < 32; i++) {
        state->internal_state[SEED_OFFSET+i] = seed[i];
    }
    chacha8rand_regenerate(state);
}

void chacha8rand_next(uint8_t* out, chacha8rand_state* state, uint64_t count) {
    for (uint64_t i = 0; i < count; i++) {
        out[i] = state->internal_state[state->position_in_state];
        state->position_in_state++;
        if (state->position_in_state == SEED_OFFSET) {
            chacha8rand_regenerate(state);
        }
    }
}
