#include "morus.h"

void print_state(state state) {
  printf("--------------------------------------------\n");
  printf("   S0   |   S1   |   S3   |   S3   |   S4   \n");
  printf("%08x %08x %08x %08x %08x\n", state[0][3], state[1][3], state[2][3], state[3][3], state[4][3]);
  printf("%08x %08x %08x %08x %08x\n", state[0][2], state[1][2], state[2][2], state[3][2], state[4][2]);
  printf("%08x %08x %08x %08x %08x\n", state[0][1], state[1][1], state[2][1], state[3][1], state[4][1]);
  printf("%08x %08x %08x %08x %08x\n", state[0][0], state[1][0], state[2][0], state[3][0], state[4][0]);
}

void print_word(state_words word) {
  printf("%08x %08x %08x %08x\n", word[0], word[1], word[2], word[3]);
}

void print(state* saved_state, state_words* saved_cipher)
{
  int i;
  printf("-------------------------------------------- Ciphers\n");
  for ( i = 0; i < 5; i++) {
    print_word(saved_cipher[i]);
  }
  printf("-------------------------------------------- States\n");
  for ( i = 0; i < 6; i++) {
    print_state(saved_state[i]);
  }
}
uint32_t rotate_left(uint32_t x, int bits)
{
  if (bits == 0) return x;
  return (x << bits) | (x >> (32 - bits));
}

void copy_state_words(state_words out, const state_words word)
{
  out[0] = word[0];
  out[1] = word[1];
  out[2] = word[2];
  out[3] = word[3];
}

void rotate_left_word(state_words word, int bits)
{
  word[0] = rotate_left(word[0], bits);
  word[1] = rotate_left(word[1], bits);
  word[2] = rotate_left(word[2], bits);
  word[3] = rotate_left(word[3], bits);
}

void and_xor_to_word(state_words o, const state_words s1 , const state_words s2)
{
  o[0] ^= s1[0] & s2[0];
  o[1] ^= s1[1] & s2[1];
  o[2] ^= s1[2] & s2[2];
  o[3] ^= s1[3] & s2[3];
}

void xor_to_word(state_words o, const state_words s)
{
		o[0] ^= s[0];
    o[1] ^= s[1];
    o[2] ^= s[2];
    o[3] ^= s[3];
}

void permute_words(state_words s, int bits)
{
	// return;
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
		t = s[0];s[0] = s[2];s[2] = t;
		t = s[1];s[1] = s[3];s[3] = t;
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

void rand_init(state st, struct RNG_state* seed){
  st[0][0] = aesrand_int32_r(seed);
  st[1][0] = aesrand_int32_r(seed);
  st[2][0] = aesrand_int32_r(seed);
  st[3][0] = aesrand_int32_r(seed);
  st[4][0] = aesrand_int32_r(seed);

  st[0][1] = aesrand_int32_r(seed);
  st[1][1] = aesrand_int32_r(seed);
  st[2][1] = aesrand_int32_r(seed);
  st[3][1] = aesrand_int32_r(seed);
  st[4][1] = aesrand_int32_r(seed);

  st[0][2] = aesrand_int32_r(seed);
  st[1][2] = aesrand_int32_r(seed);
  st[2][2] = aesrand_int32_r(seed);
  st[3][2] = aesrand_int32_r(seed);
  st[4][2] = aesrand_int32_r(seed);

  st[0][3] = aesrand_int32_r(seed);
  st[1][3] = aesrand_int32_r(seed);
  st[2][3] = aesrand_int32_r(seed);
  st[3][3] = aesrand_int32_r(seed);
  st[4][3] = aesrand_int32_r(seed);
}

void iterate(state st, state_words message) {
    // print_state(st);
    xor_to_word(st[0],st[3]);
    and_xor_to_word(st[0], st[1], st[2]);
    rotate_left_word(st[0],5);

    permute_words(st[3],32);

    xor_to_word(st[1], message);
    xor_to_word(st[1],st[4]);
    and_xor_to_word(st[1], st[2], st[3]);
    rotate_left_word(st[1],31);

    permute_words(st[4],64);

    xor_to_word(st[2], message);
    xor_to_word(st[2],st[0]);
    and_xor_to_word(st[2], st[3], st[4]);
    rotate_left_word(st[2],7);

    permute_words(st[0],96);

    xor_to_word(st[3], message);
    xor_to_word(st[3],st[1]);
    and_xor_to_word(st[3], st[4], st[0]);
    rotate_left_word(st[3],22);

    permute_words(st[1],64);

    xor_to_word(st[4], message);
    xor_to_word(st[4],st[2]);
    and_xor_to_word(st[4], st[0], st[1]);
    rotate_left_word(st[4],13);

    permute_words(st[2],32);

    // print_state(st);
}

void encrypt(state_words* out, state_words message, state st)
{
	state_words mem, st1;
  // printf("-------------------------------------------- Message\n");
  // print_word(message);
  copy_state_words(mem,message);
  copy_state_words(st1,st[1]);

  permute_words(st1,96);

  xor_to_word(st1,message);
  xor_to_word(st1,st[0]);
  and_xor_to_word(st1,st[2],st[3]);
	iterate(st, mem);
  copy_state_words(*out,st1);
  // (*out)[0] = st1[0];
  // (*out)[1] = st1[1];
  // (*out)[2] = st1[2];
  // (*out)[3] = st1[3];
  // printf("-------------------------------------------- CipherText\n");
  // print_word(st1);
  // print_word(*out);
}

uint32_t gen_mask(uint32_t* mask, unsigned int i)
{
  *mask |= (1 << i);
  return 0;
}

// Compute the Hamming Weight
uint32_t HW(uint32_t x)
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

#if 0
  // print(saved_state,saved_cipher);

  mask = 0;
  gen_mask(&mask,0);
  res ^= saved_state[1][0][0] & mask;
  // res ^= saved_state[1][0][1] & mask;
  // res ^= saved_state[1][0][2] & mask;
  // res ^= saved_state[1][0][3] & mask;

  res ^= saved_state[1][4][0] & mask;
  // res ^= saved_state[1][4][1] & mask;
  // res ^= saved_state[1][4][2] & mask;
  // res ^= saved_state[1][4][3] & mask;

  mask = 0;
  gen_mask(&mask,0);
  gen_mask(&mask,8);
  gen_mask(&mask,26);
  res ^= saved_cipher[1][0] & mask;
  // res ^= saved_cipher[1][1] & mask;
  // res ^= saved_cipher[1][2] & mask;
  // res ^= saved_cipher[1][3] & mask;

  mask = 0;
  gen_mask(&mask,13);
  gen_mask(&mask,31);
  res ^= saved_cipher[1][0] & mask;
  // res ^= saved_cipher[1][1] & mask;
  // res ^= saved_cipher[1][2] & mask;
  // res ^= saved_cipher[1][3] & mask;

  mask = 0;
  gen_mask(&mask,13);
  res ^= saved_state[2][4][0] & mask;
  // res ^= saved_state[2][4][1] & mask;
  // res ^= saved_state[2][4][2] & mask;
  // res ^= saved_state[2][4][3] & mask;

  mask = 0;
  gen_mask(&mask,13);
  res ^= saved_state[2][1][0] & mask;
  // res ^= saved_state[2][1][1] & mask;
  // res ^= saved_state[2][1][2] & mask;
  // res ^= saved_state[2][1][3] & mask;

  mask = 0;
  gen_mask(&mask,12);
  res ^= saved_state[3][1][0] & mask;
  // res ^= saved_state[3][1][1] & mask;
  // res ^= saved_state[3][1][2] & mask;
  // res ^= saved_state[3][1][3] & mask;

#else

  /* res ^= saved_state[0][1][0] & (1<<0); */
  /* res ^= saved_state[0][1][1] & (1<<0); */
  /* res ^= saved_state[1][0][3] & (1<<5); */
  /* res ^= saved_cipher[0][0] & (1<<0); */

  /* res ^= saved_state[0][1][1] & (1<<0); */
  /* res ^= saved_state[0][0][0] & (1<<0); */
  /* res ^= saved_cipher[0][0] & (1<<0); */

  /* res ^= saved_state[0][1][0] & (1<<0); */
  /* res ^= saved_state[1][0][3] & (1<<5); */
  /* res ^= saved_state[0][0][0] & (1<<0); */
  /* res ^= saved_state[0][3][0] & (1<<0); */
  
  /* res ^= saved_state[1][1][0] & (1<<5); */
  /* res ^= saved_state[1][0][3] & (1<<5); */
  /* res ^= saved_cipher[1][3] & (1<<5); */


  /* res ^= saved_state[0][1][0] & (1<<0); */
  /* res ^= saved_state[0][1][1] & (1<<0); */
  /* res ^= saved_state[1][1][0] & (1<<5); */
  /* res ^= saved_cipher[0][0] & (1<<0); */
  /* res ^= saved_cipher[1][3] & (1<<5); */

  /* res ^= saved_state[0][1][1] & (1<<0); */
  /* res ^= saved_state[0][1][2] & (1<<0); */
  /* res ^= saved_state[1][1][1] & (1<<5); */
  /* res ^= saved_cipher[0][1] & (1<<0); */
  /* res ^= saved_cipher[1][0] & (1<<5); */

  /* res ^= saved_state[1][1][0] & (1<<5); */
  /* res ^= saved_state[1][1][1] & (1<<5); */
  /* res ^= saved_state[2][1][0] & (1<<10); */
  /* res ^= saved_cipher[1][0] & (1<<5); */
  /* res ^= saved_cipher[2][3] & (1<<10); */


  res ^= saved_cipher[0][0] & (1<<27);
  res ^= saved_cipher[0][1] & (1<<27);
  res ^= saved_cipher[0][2] & (1<<27);
  res ^= saved_cipher[0][3] & (1<<27);
  /* res ^= saved_state[1][0][0] & (1<<0); */
  /* res ^= saved_state[1][0][1] & (1<<0); */
  /* res ^= saved_state[1][0][2] & (1<<0); */
  /* res ^= saved_state[1][0][3] & (1<<0); */

  res ^= saved_cipher[1][0] & (1<<0);
  res ^= saved_cipher[1][1] & (1<<0);
  res ^= saved_cipher[1][2] & (1<<0);
  res ^= saved_cipher[1][3] & (1<<0);
  /* res ^= saved_state[2][0][0] & (1<<5); */
  /* res ^= saved_state[2][0][1] & (1<<5); */
  /* res ^= saved_state[2][0][2] & (1<<5); */
  /* res ^= saved_state[2][0][3] & (1<<5); */

  res ^= saved_state[1][3][0] & (1<<0);
  res ^= saved_state[1][3][1] & (1<<0);
  res ^= saved_state[1][3][2] & (1<<0);
  res ^= saved_state[1][3][3] & (1<<0);
  /* res ^= saved_state[1][0][0] & (1<<0); */
  /* res ^= saved_state[1][0][1] & (1<<0); */
  /* res ^= saved_state[1][0][2] & (1<<0); */
  /* res ^= saved_state[1][0][3] & (1<<0); */
  /* res ^= saved_state[2][0][0] & (1<<5); */
  /* res ^= saved_state[2][0][1] & (1<<5); */
  /* res ^= saved_state[2][0][2] & (1<<5); */
  /* res ^= saved_state[2][0][3] & (1<<5); */
  
  res ^= saved_state[1][1][0] & (1<<0);
  res ^= saved_state[1][1][1] & (1<<0);
  res ^= saved_state[1][1][2] & (1<<0);
  res ^= saved_state[1][1][3] & (1<<0);
  
#endif

  
  return 1 & HW(res);
}
