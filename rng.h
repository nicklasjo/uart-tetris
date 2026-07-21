#ifndef RNG_H
#define RNG_H

#include <stdint.h>

void rng_seed(uint32_t seed);

// Uniform random value in [0, n). Avoids hardware/software division
// (this target has neither a divider nor a linked __aeabi_idivmod).
uint32_t rng_below(uint32_t n);

#endif
