#include "mini_morus.h"

void null_word(state_words* w) {
   *w = 0;
}

inline void print_state(state state) {
  printf("%08x %08x %08x %08x %08x\n", state[0], state[1], state[2], state[3], state[4]);
}

inline uint32_t rotate_left(uint32_t x, int bits)
{
  if (bits == 0) return x;
  return (x << bits) | (x >> (32 - bits));
}

void rand_init(state st, struct RNG_state* seed){
  st[0] = aesrand_int32_r(seed);
  st[1] = aesrand_int32_r(seed);
  st[2] = aesrand_int32_r(seed);
  st[3] = aesrand_int32_r(seed);
  st[4] = aesrand_int32_r(seed);
}

void iterate(state st, state_words message) {
	st[0] ^= st[1] & st[2];
	st[0] ^= st[3];
	st[0] = rotate_left(st[0], 5);

	st[1] ^= message;
	st[1] ^= st[2] & st[3];
	st[1] ^= st[4];
	st[1] = rotate_left(st[1], 31);

	st[2] ^= message;
	st[2] ^= st[3] & st[4];
	st[2] ^= st[0];
	st[2] = rotate_left(st[2], 7);

	st[3] ^= message;
	st[3] ^= st[4] & st[0];
	st[3] ^= st[1];
	st[3] = rotate_left(st[3], 22);

	st[4] ^= message;
	st[4] ^= st[0] & st[1];
	st[4] ^= st[2];
	st[4] = rotate_left(st[4], 13);
}

void encrypt(state_words* out, state_words message, state st)
{
	state_words mem = message;
	message ^= st[0];
	message ^= st[1];
	message ^= st[2] & st[3];
	iterate(st, mem);
  *out = message;
}

inline uint32_t gen_mask(uint32_t* mask, unsigned int i)
{
  *mask |= (1 << i);
  return 0;
}

// Compute the Hamming Weight
inline uint32_t HW(uint32_t x)
{
    x = x - ((x >> 1) & 0x55555555);
    x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
    x = (x + (x >> 4)) & 0x0F0F0F0F;
    x = x + (x >> 8);
    x = x + (x >> 16);
    return x & 0x0000003F;
}



int linear(state* saved_state, state_words* saved_cipher) {
  uint32_t mask = 0;
  uint32_t res = 0;

  gen_mask(&mask,27);
  res ^= saved_cipher[0] & mask;

  mask = 0;
  gen_mask(&mask,0);
  gen_mask(&mask,26);
  gen_mask(&mask,8);
  gen_mask(&mask,2);
  res ^= saved_cipher[1] & mask;

  mask = 0;
  gen_mask(&mask,31);
  gen_mask(&mask,13);
  gen_mask(&mask,1);
  gen_mask(&mask,15);
  gen_mask(&mask,27);
  res ^= saved_cipher[2] & mask;

  mask = 0;
  gen_mask(&mask,6);
  gen_mask(&mask,12);
  gen_mask(&mask,20);
  gen_mask(&mask,14);
  res ^= saved_cipher[3] & mask;

  mask = 0;
  gen_mask(&mask,19);
  res ^= saved_cipher[4] & mask;

  return 1 & HW(res);
}
