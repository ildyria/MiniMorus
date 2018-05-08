#include "full_morus_640.h"

#ifdef _MSC_VER
#define inline __inline
// #define _mm_store_si128 _mm_storeu_si128
// #define _mm_load_si128 _mm_loadu_si128
#endif

#define n1 5
#define n2 31
#define n3 7
#define n4 22
#define n5 13

#define ROTL_128(x,n)     XOR_128(_mm_slli_epi32((x), (n)),  _mm_srli_epi32((x),(32-n)))  /*Rotate 4 32-bit unsigned integers in x to the left by n-bit positions*/
#define ROTL8(x)       		_mm_shuffle_epi8((x), _mm_set_epi8(14,13,12,11,10,9,8,7,6,5,4,3,2,1,0,15) )  /*Rotate 4 32-bit unsigned integers in x to the left by 8-bit positions*/

#define ROTL32(x)      		_mm_shuffle_epi32((x),_MM_SHUFFLE(2,1,0,3))   /*Rotate x by 32-bit positions to the left*/
#define ROTL64(x)      		_mm_shuffle_epi32((x),_MM_SHUFFLE(1,0,3,2))   /*Rotate x by 64-bit positions to the left*/
#define ROTL96(x)      		_mm_shuffle_epi32((x),_MM_SHUFFLE(0,3,2,1))   /*Rotate x by 96-bit positions to the left*/

// #define STORE_128(x,p)     _mm_store_si128((__m128i *)(p), (x))         /*store the 128-bit word x into memeory address p, where p is the multile of 16 bytes*/
// #define LOAD_128(p)        _mm_load_si128((__m128i *)(p))               /*load 16 bytes from the memory address p, return a 128-bit word, where p is the multile of 16 bytes*/

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
	// to[0].f32[0] = from[0].f32[0];
	// to[0].f32[1] = from[0].f32[1];
	// to[0].f32[2] = from[0].f32[2];
	// to[0].f32[3] = from[0].f32[3];
	//
	// to[1].f32[0] = from[1].f32[0];
	// to[1].f32[1] = from[1].f32[1];
	// to[1].f32[2] = from[1].f32[2];
	// to[1].f32[3] = from[1].f32[3];
	//
	// to[2].f32[0] = from[2].f32[0];
	// to[2].f32[1] = from[2].f32[1];
	// to[2].f32[2] = from[2].f32[2];
	// to[2].f32[3] = from[2].f32[3];
	//
	// to[3].f32[0] = from[3].f32[0];
	// to[3].f32[1] = from[3].f32[1];
	// to[3].f32[2] = from[3].f32[2];
	// to[3].f32[3] = from[3].f32[3];
	//
	// to[4].f32[0] = from[4].f32[0];
	// to[4].f32[1] = from[4].f32[1];
	// to[4].f32[2] = from[4].f32[2];
	// to[4].f32[3] = from[4].f32[3];
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

// static inline void rotate_left_word(union Register *word, int bits) {
// 	word->f32[0] = rotate_left_32(word->f32[0], bits);
// 	word->f32[1] = rotate_left_32(word->f32[1], bits);
// 	word->f32[2] = rotate_left_32(word->f32[2], bits);
// 	word->f32[3] = rotate_left_32(word->f32[3], bits);
// }
//
// inline void and_xor_to_word(union Register *o, const union Register *s1, const union Register *s2) {
// 	o->f32[0] ^= s1->f32[0] & s2->f32[0];
// 	o->f32[1] ^= s1->f32[1] & s2->f32[1];
// 	o->f32[2] ^= s1->f32[2] & s2->f32[2];
// 	o->f32[3] ^= s1->f32[3] & s2->f32[3];
// }
//
// inline void xor_to_word(union Register *o, const union Register *s) {
// 	o->f32[0] ^= s->f32[0];
// 	o->f32[1] ^= s->f32[1];
// 	o->f32[2] ^= s->f32[2];
// 	o->f32[3] ^= s->f32[3];
// }
//
// inline void permute_words(union Register *s, int bits) {
// 	uint32_t t;
// 	if (bits == 96) {
// 		t = s->f32[0];
// 		s->f32[0] = s->f32[1];
// 		s->f32[1] = s->f32[2];
// 		s->f32[2] = s->f32[3];
// 		s->f32[3] = t;
// 	}
// 	if (bits == 64) {
// 		t = s->f32[0];
// 		s->f32[0] = s->f32[2];
// 		s->f32[2] = t;
// 		t = s->f32[1];
// 		s->f32[1] = s->f32[3];
// 		s->f32[3] = t;
// 	}
// 	if (bits == 32) {
// 		t = s->f32[3];
// 		s->f32[3] = s->f32[2];
// 		s->f32[2] = s->f32[1];
// 		s->f32[1] = s->f32[0];
// 		s->f32[0] = t;
// 	}
// }

void morus_640_rand_init(state st, struct RNG_state *seed) {

  st[0].f32 = aesrand_m128i_r(seed);
  st[1].f32 = aesrand_m128i_r(seed);
  st[2].f32 = aesrand_m128i_r(seed);
  st[3].f32 = aesrand_m128i_r(seed);
	st[4].f32 = aesrand_m128i_r(seed);
	//
  // st[0].f32[0] = aesrand_int32_r(seed);
	// st[0].f32[1] = aesrand_int32_r(seed);
  // st[0].f32[2] = aesrand_int32_r(seed);
  // st[0].f32[3] = aesrand_int32_r(seed);
	//
  // st[1].f32[0] = aesrand_int32_r(seed);
	// st[1].f32[1] = aesrand_int32_r(seed);
  // st[1].f32[2] = aesrand_int32_r(seed);
  // st[1].f32[3] = aesrand_int32_r(seed);
	//
  // st[2].f32[0] = aesrand_int32_r(seed);
  // st[2].f32[1] = aesrand_int32_r(seed);
  // st[2].f32[2] = aesrand_int32_r(seed);
  // st[2].f32[3] = aesrand_int32_r(seed);
	//
  // st[3].f32[0] = aesrand_int32_r(seed);
  // st[3].f32[1] = aesrand_int32_r(seed);
  // st[3].f32[2] = aesrand_int32_r(seed);
  // st[3].f32[3] = aesrand_int32_r(seed);
	//
	// st[4].f32[0] = aesrand_int32_r(seed);
	// st[4].f32[1] = aesrand_int32_r(seed);
	// st[4].f32[2] = aesrand_int32_r(seed);
	// st[4].f32[3] = aesrand_int32_r(seed);
}

void morus_640_iterate(state st) {

		st[0].f32 = XOR_128(st[0].f32, st[3].f32);
		st[0].f32 = XOR_128(st[0].f32, AND_128(st[1].f32, st[2].f32));
		st[0].f32 = ROTL_128(st[0].f32, n1);
		st[3].f32 = ROTL32(st[3].f32);

		// st[1].f32 = XOR_128(st[1].f32, msgblk);
		st[1].f32 = XOR_128(st[1].f32, st[4].f32);
		st[1].f32 = XOR_128(st[1].f32, AND_128(st[2].f32, st[3].f32));
		st[1].f32 = ROTL_128(st[1].f32, n2);
		st[4].f32 = ROTL64(st[4].f32);

		// st[2].f32 = XOR_128(st[2].f32, msgblk);
		st[2].f32 = XOR_128(st[2].f32, st[0].f32);
		st[2].f32 = XOR_128(st[2].f32, AND_128(st[3].f32, st[4].f32));
		st[2].f32 = ROTL_128(st[2].f32, n3);
		st[0].f32 = ROTL96(st[0].f32);

		// st[3].f32 = XOR_128(st[3].f32, msgblk);
		st[3].f32 = XOR_128(st[3].f32, st[1].f32);
		st[3].f32 = XOR_128(st[3].f32, AND_128(st[4].f32, st[0].f32));
		st[3].f32 = ROTL_128(st[3].f32, n4);
		st[1].f32 = ROTL64(st[1].f32);

		// st[4].f32 = XOR_128(st[4].f32, msgblk);
		st[4].f32 = XOR_128(st[4].f32, st[2].f32);
		st[4].f32 = XOR_128(st[4].f32, AND_128(st[0].f32, st[1].f32));
		st[4].f32 = ROTL_128(st[4].f32, n5);
		st[2].f32 = ROTL32(st[2].f32);

	// morus_640_print_state(st);
	// xor_to_word(&(st[0]), &(st[3]));
	// and_xor_to_word(&(st[0]), &(st[1]), &(st[2]));
	// rotate_left_word(&(st[0]), 5);
	//
	// permute_words(&(st[3]), 32);
	//
	// xor_to_word(&(st[1]), &(st[4]));
	// and_xor_to_word(&(st[1]), &(st[2]), &(st[3]));
	// rotate_left_word(&(st[1]), 31);
	//
	// permute_words(&(st[4]), 64);
	//
	// xor_to_word(&(st[2]), &(st[0]));
	// and_xor_to_word(&(st[2]), &(st[3]), &(st[4]));
	// rotate_left_word(&(st[2]), 7);
	//
	// permute_words(&(st[0]), 96);
	//
	// xor_to_word(&(st[3]), &(st[1]));
	// and_xor_to_word(&(st[3]), &(st[4]), &(st[0]));
	// rotate_left_word(&(st[3]), 22);
	//
	// permute_words(&(st[1]), 64);
	//
	// xor_to_word(&(st[4]), &(st[2]));
	// and_xor_to_word(&(st[4]), &(st[0]), &(st[1]));
	// rotate_left_word(&(st[4]), 13);
	//
	// permute_words(&(st[2]), 32);

	// morus_640_print_state(st);
}

void morus_640_encrypt(union Register *out, state st) {
	// printf("-------------------------------------------- Message\n");

	// out->f32 = st[1].f32;
	// out->f32[0] = st[1].f32[0];
	// out->f32[1] = st[1].f32[1];
	// out->f32[2] = st[1].f32[2];
	// out->f32[3] = st[1].f32[3];

	// xor_to_word(out, &(st[0]));
	//
	// permute_words(out, 96);
	//
	// and_xor_to_word(out, &(st[2]), &(st[3]));

	// __m128i keystream;
	//encryption
	out->f32 = XOR_128(st[0].f32, ROTL96(st[1].f32));
	out->f32 = XOR_128(out->f32, AND_128(st[2].f32, st[3].f32));

	morus_640_iterate(st);
	// printf("-------------------------------------------- CipherText\n");
	// print_word(*out);
}
