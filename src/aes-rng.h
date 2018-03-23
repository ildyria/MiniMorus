#pragma once
#ifdef __AES__

#include <smmintrin.h>
#include <wmmintrin.h>
#include <stdint.h>
#include <assert.h>

struct RNG_state {
  __m128i count;
  __m128i t;
  __m128i k;
  int next;
};

struct RNG_state* init_aesrand_r(uint32_t seed1, uint32_t seed2);
uint32_t aesrand_int32_r(struct RNG_state* rng);


#endif
