// Use AES-based RNG
#include "aes-rng.h"
#ifdef __AES__

// __m128i key, count;
// int next;
//
// void init_aesrand(unsigned long long seed) {
//   srand(seed);
//   key   = _mm_setr_epi32(rand(), rand(), rand(), rand());
//   count = _mm_setr_epi32(rand(), rand(), rand(), rand());
//   next = 0;
// }
//
// unsigned int aesrand_int32() {
//   static __m128i t;
//   switch(next++) {
//   case 0:
//     // Increment counter
//     count = _mm_add_epi64(count, _mm_setr_epi32(0,0,1,0));
//
//     // 6 AES rounds
//     t = count;
//     t = _mm_aesenc_si128(t, key);
//     t = _mm_aesenc_si128(t, key);
//     t = _mm_aesenc_si128(t, key);
//     t = _mm_aesenc_si128(t, key);
//     t = _mm_aesenc_si128(t, key);
//     t = _mm_aesenc_si128(t, key);
//
//     return _mm_extract_epi32(t,0);
//   case 1:
//     return _mm_extract_epi32(t,1);
//   case 2:
//     return _mm_extract_epi32(t,2);
//   case 3:
//     next = 0;
//     return _mm_extract_epi32(t,3);
//   default:
//     assert(0);
//   }
// }


struct RNG_state* init_aesrand_r(uint32_t seed1, uint32_t seed2) {
  // Internal state: count + t
  struct RNG_state *st = malloc(sizeof(struct RNG_state));
  assert(st);
  st->count = _mm_setr_epi32(seed1, seed2, 0, 0);
  st->k = _mm_setr_epi32(0, 0, seed1, seed2);
  st->next = 0;
  return st;
}

uint32_t aesrand_int32_r(struct RNG_state* rng) {
  switch(rng->next++) {
  case 0:
    // Increment counter
    rng->count = _mm_add_epi64(rng->count, _mm_setr_epi32(0,0,1,0));

    // 6 AES rounds
    rng->t = rng->count;
    rng->t = _mm_aesenc_si128(rng->t, rng->k);
    rng->t = _mm_aesenc_si128(rng->t, rng->k);
    rng->t = _mm_aesenc_si128(rng->t, rng->k);
    rng->t = _mm_aesenc_si128(rng->t, rng->k);
    rng->t = _mm_aesenc_si128(rng->t, rng->k);
    rng->t = _mm_aesenc_si128(rng->t, rng->k);

    return _mm_extract_epi32(rng->t,0);
  case 1:
    return _mm_extract_epi32(rng->t,1);
  case 2:
    return _mm_extract_epi32(rng->t,2);
  case 3:
    rng->next = 0;
    return _mm_extract_epi32(rng->t,3);
  default:
    assert(0);
  }
}

// #else
//
// #include "util.h"
//
// void init_aesrand(unsigned long long seed) {
//   die ("AES-NI support not enabled!\n");
// }
//
// unsigned int aesrand_int32(void){
//   die ("AES-NI support not enabled!\n");
// }
//
// void* init_aesrand_r(uint32_t seed1, uint32_t seed2) {
//   die ("AES-NI support not enabled!\n");
// }
//
// unsigned int aesrand_int32_r(void* st) {
//   die ("AES-NI support not enabled!\n");
// }
//

#endif
