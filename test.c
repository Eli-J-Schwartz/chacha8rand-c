#include <stdio.h>
#include "chacha8rand.h"

int main(int argc, char** argv) {
    uint8_t seed[32] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ123456";
    int output_size = 2976;

    chacha8rand_state state;
    chacha8rand_seed(&state, seed);
    
    uint8_t output[output_size];
    chacha8rand_next(output, &state, output_size);
    
    for (int i = 0; i < output_size; i++) {
        printf("%02x", output[i]);
        if (i%32 == 31) {
            printf("\n");
        }
    }
}
