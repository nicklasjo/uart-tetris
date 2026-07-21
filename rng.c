#include "rng.h"

static uint32_t state = 1;

void rng_seed(uint32_t seed) {
    state = seed ? seed : 1; // xorshift can't be seeded with 0
}

static uint32_t rng_next(void) {
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}

uint32_t rng_below(uint32_t n) {
    // Smallest (2^k - 1) mask that covers n, then reject draws that
    // land outside [0, n) so every value stays equally likely.
    uint32_t mask = 1;
    while (mask < n) {
        mask = (mask << 1) | 1;
    }

    uint32_t r;
    do {
        r = rng_next() & mask;
    } while (r >= n);
    return r;
}
