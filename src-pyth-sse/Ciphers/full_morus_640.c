#include "full_morus_640.h"

#ifdef _MSC_VER
#define inline __inline
#endif

#define n1 5
#define n2 31
#define n3 7
#define n4 22
#define n5 13


#define XOR_128(x,y)      _mm_xor_si128((x),(y))     /*XOR_128(x,y) = x ^ y, where x and y are two 128-bit word*/
#define AND_128(x,y)      _mm_and_si128((x),(y))     /*AND_128(x,y) = x & y, where x and y are two 128-bit word*/

#define ROTL_128(x,n)     XOR_128(_mm_slli_epi32((x), (n)),  _mm_srli_epi32((x),(32-n)))  /*Rotate 4 32-bit unsigned integers in x to the left by n-bit positions*/

#define ROTL_128_32(x)      		_mm_shuffle_epi32((x),_MM_SHUFFLE(2,1,0,3))   /*Rotate x by 32-bit positions to the left*/
#define ROTL_128_64(x)      		_mm_shuffle_epi32((x),_MM_SHUFFLE(1,0,3,2))   /*Rotate x by 64-bit positions to the left*/
#define ROTL_128_96(x)      		_mm_shuffle_epi32((x),_MM_SHUFFLE(0,3,2,1))   /*Rotate x by 96-bit positions to the left*/

void morus_640_print_state(state state) {
	// printf("--------------------------------------------\n");
	// printf("   S0   |   S1   |   S3   |   S3   |   S4   \n");
	// printf("%08x %08x %08x %08x %08x\n", state[0].f32[3], state[1].f32[3], state[2].f32[3], state[3].f32[3], state[4].f32[3]);
	// printf("%08x %08x %08x %08x %08x\n", state[0].f32[2], state[1].f32[2], state[2].f32[2], state[3].f32[2], state[4].f32[2]);
	// printf("%08x %08x %08x %08x %08x\n", state[0].f32[1], state[1].f32[1], state[2].f32[1], state[3].f32[1], state[4].f32[1]);
	// printf("%08x %08x %08x %08x %08x\n", state[0].f32[0], state[1].f32[0], state[2].f32[0], state[3].f32[0], state[4].f32[0]);
}

void morus_640_copy_state(state to, state from) {
	to[0].f32 = from[0].f32;
	to[1].f32 = from[1].f32;
	to[2].f32 = from[2].f32;
	to[3].f32 = from[3].f32;
	to[4].f32 = from[4].f32;
}

inline void print_word(union Register *word) {
	// printf("%08x %08x %08x %08x\n", word->f32[0], word->f32[1], word->f32[2], word->f32[3]);
}

void morus_640_print(state *saved_state, union Register *saved_cipher) {
	printf("-------------------------------------------- Ciphers\n");
	print_word(&(saved_cipher[0]));
	print_word(&(saved_cipher[1]));
	print_word(&(saved_cipher[2]));
	print_word(&(saved_cipher[3]));
	print_word(&(saved_cipher[4]));
	printf("-------------------------------------------- States\n");
	morus_640_print_state(saved_state[0]);
	morus_640_print_state(saved_state[1]);
	morus_640_print_state(saved_state[2]);
	morus_640_print_state(saved_state[3]);
	morus_640_print_state(saved_state[4]);
	morus_640_print_state(saved_state[5]);
}

void morus_640_rand_init(state st, struct RNG_state *seed) {

  st[0].f32 = aesrand_m128i_r(seed);
  st[1].f32 = aesrand_m128i_r(seed);
  st[2].f32 = aesrand_m128i_r(seed);
  st[3].f32 = aesrand_m128i_r(seed);
	st[4].f32 = aesrand_m128i_r(seed);
}

void morus_640_iterate(state st) {

		st[0].f32 = XOR_128(st[0].f32, st[3].f32);
		st[0].f32 = XOR_128(st[0].f32, AND_128(st[1].f32, st[2].f32));
		st[0].f32 = ROTL_128(st[0].f32, n1);
		st[3].f32 = ROTL_128_32(st[3].f32);

		st[1].f32 = XOR_128(st[1].f32, st[4].f32);
		st[1].f32 = XOR_128(st[1].f32, AND_128(st[2].f32, st[3].f32));
		st[1].f32 = ROTL_128(st[1].f32, n2);
		st[4].f32 = ROTL_128_64(st[4].f32);

		st[2].f32 = XOR_128(st[2].f32, st[0].f32);
		st[2].f32 = XOR_128(st[2].f32, AND_128(st[3].f32, st[4].f32));
		st[2].f32 = ROTL_128(st[2].f32, n3);
		st[0].f32 = ROTL_128_96(st[0].f32);

		st[3].f32 = XOR_128(st[3].f32, st[1].f32);
		st[3].f32 = XOR_128(st[3].f32, AND_128(st[4].f32, st[0].f32));
		st[3].f32 = ROTL_128(st[3].f32, n4);
		st[1].f32 = ROTL_128_64(st[1].f32);

		st[4].f32 = XOR_128(st[4].f32, st[2].f32);
		st[4].f32 = XOR_128(st[4].f32, AND_128(st[0].f32, st[1].f32));
		st[4].f32 = ROTL_128(st[4].f32, n5);
		st[2].f32 = ROTL_128_32(st[2].f32);
}

void morus_640_encrypt(union Register *out, state st) {
	// printf("-------------------------------------------- Message\n");

	out->f32 = XOR_128(st[0].f32, ROTL_128_96(st[1].f32));
	out->f32 = XOR_128(out->f32, AND_128(st[2].f32, st[3].f32));

	morus_640_iterate(st);
	// printf("-------------------------------------------- CipherText\n");
	// print_word(*out);
}
