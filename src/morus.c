#include "morus.h"

inline void print_state(state state) {
  printf("   S0   |   S1   |   S3   |   S3   |   S4   \n");
  printf("%08x %08x %08x %08x %08x\n", state[0][3], state[1][3], state[2][3], state[3][3], state[4][3]);
  printf("%08x %08x %08x %08x %08x\n", state[0][2], state[1][2], state[2][2], state[3][2], state[4][2]);
  printf("%08x %08x %08x %08x %08x\n", state[0][1], state[1][1], state[2][1], state[3][1], state[4][1]);
  printf("%08x %08x %08x %08x %08x\n", state[0][0], state[1][0], state[2][0], state[3][0], state[4][0]);
}

inline uint32_t rotate_left(uint32_t x, int bits)
{
  if (bits == 0) return x;
  return (x << bits) | (x >> (32 - bits));
}

inline void copy_state_words(state_words out, const state_words word)
{
  out[0] = word[0];
  out[1] = word[1];
  out[2] = word[2];
  out[3] = word[3];
}

inline void rotate_left_word(state_words word, int bits)
{
  word[0] = rotate_left(word[0],bits);
  word[1] = rotate_left(word[1],bits);
  word[2] = rotate_left(word[2],bits);
  word[3] = rotate_left(word[3],bits);
}

inline void and_xor_to_word(state_words o, const state_words s1 , const state_words s2)
{
  o[0] ^= s1[0] & s2[0];
  o[1] ^= s1[1] & s2[1];
  o[2] ^= s1[2] & s2[2];
  o[3] ^= s1[3] & s2[3];
}

inline void xor_to_word(state_words o, const state_words s)
{
		o[0] ^= s[0];
    o[1] ^= s[1];
    o[2] ^= s[2];
    o[3] ^= s[3];
}

inline void permute_words(state_words s, int bits)
{
	uint32_t t;
	if (bits == 96)
	{
		t = s[0];
		s[0] = s[1];
		s[1] = s[2];
		s[2] = s[3];
		s[3] = t;
	}
	if (bits == 64)
	{
		t = s[0];
		s[0] = s[2];
		s[2] = t;
		t = s[1];
		s[1] = s[3];
		s[3] = t;
	}
	if (bits == 32)
	{
		t = s[3];
		s[3] = s[2];
		s[2] = s[1];
		s[1] = s[0];
		s[0] = t;
	}
}

void null_word(state_words* w){
  (*w)[0] = 0;
  (*w)[1] = 0;
  (*w)[2] = 0;
  (*w)[3] = 0;
}

void rand_init(state st, unsigned int* seed){
  // need to be faster !!!
	st[0][0] = (rand_r(seed) & 0xffff) | (rand_r(seed) & 0xffff) << 16;
  st[1][0] = (rand_r(seed) & 0xffff) | (rand_r(seed) & 0xffff) << 16;
  st[2][0] = (rand_r(seed) & 0xffff) | (rand_r(seed) & 0xffff) << 16;
  st[3][0] = (rand_r(seed) & 0xffff) | (rand_r(seed) & 0xffff) << 16;
  st[4][0] = (rand_r(seed) & 0xffff) | (rand_r(seed) & 0xffff) << 16;

  st[0][1] = (rand_r(seed) & 0xffff) | (rand_r(seed) & 0xffff) << 16;
  st[1][1] = (rand_r(seed) & 0xffff) | (rand_r(seed) & 0xffff) << 16;
  st[2][1] = (rand_r(seed) & 0xffff) | (rand_r(seed) & 0xffff) << 16;
  st[3][1] = (rand_r(seed) & 0xffff) | (rand_r(seed) & 0xffff) << 16;
  st[4][1] = (rand_r(seed) & 0xffff) | (rand_r(seed) & 0xffff) << 16;

  st[0][2] = (rand_r(seed) & 0xffff) | (rand_r(seed) & 0xffff) << 16;
  st[1][2] = (rand_r(seed) & 0xffff) | (rand_r(seed) & 0xffff) << 16;
  st[2][2] = (rand_r(seed) & 0xffff) | (rand_r(seed) & 0xffff) << 16;
  st[3][2] = (rand_r(seed) & 0xffff) | (rand_r(seed) & 0xffff) << 16;
  st[4][2] = (rand_r(seed) & 0xffff) | (rand_r(seed) & 0xffff) << 16;

  st[0][3] = (rand_r(seed) & 0xffff) | (rand_r(seed) & 0xffff) << 16;
  st[1][3] = (rand_r(seed) & 0xffff) | (rand_r(seed) & 0xffff) << 16;
  st[2][3] = (rand_r(seed) & 0xffff) | (rand_r(seed) & 0xffff) << 16;
  st[3][3] = (rand_r(seed) & 0xffff) | (rand_r(seed) & 0xffff) << 16;
  st[4][3] = (rand_r(seed) & 0xffff) | (rand_r(seed) & 0xffff) << 16;
}

void iterate(state st, state_words message) {

    and_xor_to_word(st[0], st[1], st[2]);
    xor_to_word(st[0],st[3]);
    rotate_left_word(st[0],5);

    permute_words(st[3],32);

    xor_to_word(st[1], message);
    and_xor_to_word(st[1], st[2], st[3]);
    xor_to_word(st[1],st[4]);
    rotate_left_word(st[1],31);

    permute_words(st[4],64);

    xor_to_word(st[2], message);
    and_xor_to_word(st[2], st[3], st[4]);
    xor_to_word(st[2],st[0]);
    rotate_left_word(st[2],7);

    permute_words(st[0],96);

    xor_to_word(st[3], message);
    and_xor_to_word(st[3], st[4], st[0]);
    xor_to_word(st[3],st[1]);
    rotate_left_word(st[3],22);

    permute_words(st[1],64);

    xor_to_word(st[4], message);
    and_xor_to_word(st[4], st[0], st[1]);
    xor_to_word(st[4],st[2]);
    rotate_left_word(st[4],13);

    permute_words(st[2],32);

}

void encrypt(state_words* out, state_words message, state st)
{
	state_words mem, st1;
  copy_state_words(mem,message);
  copy_state_words(st1,st[1]);
  permute_words(st1,96);
  xor_to_word(st1,message);
  and_xor_to_word(st1,st[2],st[3]);
	iterate(st, mem);
  (*out)[0] = st1[0];
  (*out)[1] = st1[1];
  (*out)[2] = st1[2];
  (*out)[3] = st1[3];
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

// return xor([mask(S[1][0], [0]),
//             mask(S[1][4], [0]),
//             mask(C[1], [0,8,26]),
//             mask(C[2], [13,31]),
//             # 2^-3 version
//             mask(S[2][4], [13]),
//             mask(S[3][1], [12]),
//             # 2^-4 version
//             #mask(S[2][1], [13]),
//             ]) # 2^-3

int linear(state* saved_state, state_words* saved_cipher) {
  uint32_t mask = 0;
  uint32_t res = 0;

  mask = 0;
  gen_mask(&mask,0);
  res ^= saved_state[1][0][0] & mask;
  res ^= saved_state[1][0][1] & mask;
  res ^= saved_state[1][0][2] & mask;
  res ^= saved_state[1][0][3] & mask;

  res ^= saved_state[1][4][0] & mask;
  res ^= saved_state[1][4][1] & mask;
  res ^= saved_state[1][4][2] & mask;
  res ^= saved_state[1][4][3] & mask;

  mask = 0;
  gen_mask(&mask,0);
  gen_mask(&mask,8);
  gen_mask(&mask,26);
  res ^= saved_cipher[1][0] & mask;
  res ^= saved_cipher[1][1] & mask;
  res ^= saved_cipher[1][2] & mask;
  res ^= saved_cipher[1][3] & mask;

  mask = 0;
  gen_mask(&mask,13);
  gen_mask(&mask,31);
  res ^= saved_cipher[1][0] & mask;
  res ^= saved_cipher[1][1] & mask;
  res ^= saved_cipher[1][2] & mask;
  res ^= saved_cipher[1][3] & mask;

  mask = 0;
  gen_mask(&mask,13);
  res ^= saved_state[2][4][0] & mask;
  res ^= saved_state[2][4][1] & mask;
  res ^= saved_state[2][4][2] & mask;
  res ^= saved_state[2][4][3] & mask;

  mask = 0;
  gen_mask(&mask,13);
  res ^= saved_state[2][1][0] & mask;
  res ^= saved_state[2][1][1] & mask;
  res ^= saved_state[2][1][2] & mask;
  res ^= saved_state[2][1][3] & mask;

  mask = 0;
  gen_mask(&mask,12);
  res ^= saved_state[3][1][0] & mask;
  res ^= saved_state[3][1][1] & mask;
  res ^= saved_state[3][1][2] & mask;
  res ^= saved_state[3][1][3] & mask;

  return 1 & HW(res);
}
