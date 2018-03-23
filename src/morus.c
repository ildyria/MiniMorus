// #include "morus.h"
//
// inline void print_state(state state) {
//   printf("%08x %08x %08x %08x %08x\n", state[0][0], state[1][0], state[2][0], state[3][0], state[4][0]);
//   printf("%08x %08x %08x %08x %08x\n", state[0][1], state[1][1], state[2][1], state[3][1], state[4][1]);
//   printf("%08x %08x %08x %08x %08x\n", state[0][2], state[1][2], state[2][2], state[3][2], state[4][2]);
//   printf("%08x %08x %08x %08x %08x\n", state[0][3], state[1][3], state[2][3], state[3][3], state[4][3]);
// }
//
// uint32_t rotate_left(uint32_t x, int bits)
// {
//   if (bits == 0) return x;
//   return (x << bits) | (x >> (32 - bits));
// }
//
// void rotate_left_state(state_words word, int bits)
// {
// 	for (int i = 0; i < 4; ++i)
// 	{
// 		word[i] = rotate_left(word[i],bits);
// 	}
// }
//
// void rotate_right_state(state_words words, int bits)
// {
// 	for (int i = 0; i < 4; ++i)
// 	{
// 		words[i] = rotate_left(words[i],32-bits);
// 	}
// }
//
// void and_xor_to_state(state_words o, const state_words s1 , const state_words s2)
// {
// 	for (int i = 0; i < 4; ++i)
// 	{
// 		o[i] ^= s1[i] & s2[i];
// 	}
// }
//
// void xor_to_state(state_words o, const state_words s)
// {
// 	for (int i = 0; i < 4; ++i)
// 	{
// 		o[i] ^= s[i];
// 	}
// }
//
// void permute_state(state_words s, int bits)
// {
// 	uint32_t t;
// 	if (bits == 96)
// 	{
// 		t = s[0];
// 		s[0] = s[1];
// 		s[1] = s[2];
// 		s[2] = s[3];
// 		s[3] = t;
// 	}
// 	if (bits == 64)
// 	{
// 		t = s[0];
// 		s[0] = s[2];
// 		s[2] = t;
// 		t = s[1];
// 		s[1] = s[3];
// 		s[3] = t;
// 	}
// 	if (bits == 32)
// 	{
// 		t = s[3];
// 		s[3] = s[2];
// 		s[2] = s[1];
// 		s[1] = s[0];
// 		s[0] = t;
// 	}
// }
//
// void rand_init(state st, unsigned int* seed){
// 	st[0][0] = (rand_r(seed) & 0xffff) | (rand_r(seed) & 0xffff) << 16;
//   st[1][0] = (rand_r(seed) & 0xffff) | (rand_r(seed) & 0xffff) << 16;
//   st[2][0] = (rand_r(seed) & 0xffff) | (rand_r(seed) & 0xffff) << 16;
//   st[3][0] = (rand_r(seed) & 0xffff) | (rand_r(seed) & 0xffff) << 16;
//   st[4][0] = (rand_r(seed) & 0xffff) | (rand_r(seed) & 0xffff) << 16;
//
//   st[0][1] = (rand_r(seed) & 0xffff) | (rand_r(seed) & 0xffff) << 16;
//   st[1][1] = (rand_r(seed) & 0xffff) | (rand_r(seed) & 0xffff) << 16;
//   st[2][1] = (rand_r(seed) & 0xffff) | (rand_r(seed) & 0xffff) << 16;
//   st[3][1] = (rand_r(seed) & 0xffff) | (rand_r(seed) & 0xffff) << 16;
//   st[4][1] = (rand_r(seed) & 0xffff) | (rand_r(seed) & 0xffff) << 16;
//
//   st[0][2] = (rand_r(seed) & 0xffff) | (rand_r(seed) & 0xffff) << 16;
//   st[1][2] = (rand_r(seed) & 0xffff) | (rand_r(seed) & 0xffff) << 16;
//   st[2][2] = (rand_r(seed) & 0xffff) | (rand_r(seed) & 0xffff) << 16;
//   st[3][2] = (rand_r(seed) & 0xffff) | (rand_r(seed) & 0xffff) << 16;
//   st[4][2] = (rand_r(seed) & 0xffff) | (rand_r(seed) & 0xffff) << 16;
//
//   st[0][3] = (rand_r(seed) & 0xffff) | (rand_r(seed) & 0xffff) << 16;
//   st[1][3] = (rand_r(seed) & 0xffff) | (rand_r(seed) & 0xffff) << 16;
//   st[2][3] = (rand_r(seed) & 0xffff) | (rand_r(seed) & 0xffff) << 16;
//   st[3][3] = (rand_r(seed) & 0xffff) | (rand_r(seed) & 0xffff) << 16;
//   st[4][3] = (rand_r(seed) & 0xffff) | (rand_r(seed) & 0xffff) << 16;
// }
//
// void iterate(state st, state_words message) {
// 	st[0] ^= st[1] & st[2];
// 	st[0] ^= st[3];
// 	st[0] = rotate_left(st[0], 5);
//
// 	st[1] ^= message;
// 	st[1] ^= st[2] & st[3];
// 	st[1] ^= st[4];
// 	st[1] = rotate_left(st[1], 31);
//
// 	st[2] ^= message;
// 	st[2] ^= st[3] & st[4];
// 	st[2] ^= st[0];
// 	st[2] = rotate_left(st[2], 7);
//
// 	st[3] ^= message;
// 	st[3] ^= st[4] & st[0];
// 	st[3] ^= st[1];
// 	st[3] = rotate_left(st[3], 22);
//
// 	st[4] ^= message;
// 	st[4] ^= st[0] & st[1];
// 	st[4] ^= st[2];
// 	st[4] = rotate_left(st[4], 13);
// }
//
// state_words encrypt(state_words message, state st)
// {
// 	state_words mem = message;
// 	message ^= st[0];
// 	message ^= st[1];
// 	message ^= st[2] & st[3];
// 	iterate(st, mem);
// 	return message;
// }
