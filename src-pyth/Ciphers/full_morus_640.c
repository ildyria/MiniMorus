#include "full_morus_640.h"

void morus_640_print_state(state state) {
	printf("--------------------------------------------\n");
	printf("   S0   |   S1   |   S3   |   S3   |   S4   \n");
	printf("%08x %08x %08x %08x %08x\n", state[0].f32[3], state[1].f32[3], state[2].f32[3], state[3].f32[3], state[4].f32[3]);
	printf("%08x %08x %08x %08x %08x\n", state[0].f32[2], state[1].f32[2], state[2].f32[2], state[3].f32[2], state[4].f32[2]);
	printf("%08x %08x %08x %08x %08x\n", state[0].f32[1], state[1].f32[1], state[2].f32[1], state[3].f32[1], state[4].f32[1]);
	printf("%08x %08x %08x %08x %08x\n", state[0].f32[0], state[1].f32[0], state[2].f32[0], state[3].f32[0], state[4].f32[0]);
}

void morus_640_copy_state(state to, state from) {
	to[0].f32[0] = from[0].f32[0];
	to[0].f32[1] = from[0].f32[1];
	to[0].f32[2] = from[0].f32[2];
	to[0].f32[3] = from[0].f32[3];

	to[1].f32[0] = from[1].f32[0];
	to[1].f32[1] = from[1].f32[1];
	to[1].f32[2] = from[1].f32[2];
	to[1].f32[3] = from[1].f32[3];

	to[2].f32[0] = from[2].f32[0];
	to[2].f32[1] = from[2].f32[1];
	to[2].f32[2] = from[2].f32[2];
	to[2].f32[3] = from[2].f32[3];

	to[3].f32[0] = from[3].f32[0];
	to[3].f32[1] = from[3].f32[1];
	to[3].f32[2] = from[3].f32[2];
	to[3].f32[3] = from[3].f32[3];

	to[4].f32[0] = from[4].f32[0];
	to[4].f32[1] = from[4].f32[1];
	to[4].f32[2] = from[4].f32[2];
	to[4].f32[3] = from[4].f32[3];
}

inline void print_word(union Register *word) {
	printf("%08x %08x %08x %08x\n", word->f32[0], word->f32[1], word->f32[2], word->f32[3]);
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

static inline void rotate_left_word(union Register *word, int bits) {
	word->f32[0] = rotate_left_32(word->f32[0], bits);
	word->f32[1] = rotate_left_32(word->f32[1], bits);
	word->f32[2] = rotate_left_32(word->f32[2], bits);
	word->f32[3] = rotate_left_32(word->f32[3], bits);
}

inline void and_xor_to_word(union Register *o, const union Register *s1, const union Register *s2) {
	o->f32[0] ^= s1->f32[0] & s2->f32[0];
	o->f32[1] ^= s1->f32[1] & s2->f32[1];
	o->f32[2] ^= s1->f32[2] & s2->f32[2];
	o->f32[3] ^= s1->f32[3] & s2->f32[3];
}

inline void xor_to_word(union Register *o, const union Register *s) {
	o->f32[0] ^= s->f32[0];
	o->f32[1] ^= s->f32[1];
	o->f32[2] ^= s->f32[2];
	o->f32[3] ^= s->f32[3];
}

inline void permute_words(union Register *s, int bits) {
	uint32_t t;
	if (bits == 96) {
		t = s->f32[0];
		s->f32[0] = s->f32[1];
		s->f32[1] = s->f32[2];
		s->f32[2] = s->f32[3];
		s->f32[3] = t;
	}
	if (bits == 64) {
		t = s->f32[0];
		s->f32[0] = s->f32[2];
		s->f32[2] = t;
		t = s->f32[1];
		s->f32[1] = s->f32[3];
		s->f32[3] = t;
	}
	if (bits == 32) {
		t = s->f32[3];
		s->f32[3] = s->f32[2];
		s->f32[2] = s->f32[1];
		s->f32[1] = s->f32[0];
		s->f32[0] = t;
	}
}

void morus_640_rand_init(state st, struct RNG_state *seed) {

  st[0].f32[0] = aesrand_int32_r(seed);
	st[0].f32[1] = aesrand_int32_r(seed);
  st[0].f32[2] = aesrand_int32_r(seed);
  st[0].f32[3] = aesrand_int32_r(seed);

  st[1].f32[0] = aesrand_int32_r(seed);
	st[1].f32[1] = aesrand_int32_r(seed);
  st[1].f32[2] = aesrand_int32_r(seed);
  st[1].f32[3] = aesrand_int32_r(seed);

  st[2].f32[0] = aesrand_int32_r(seed);
  st[2].f32[1] = aesrand_int32_r(seed);
  st[2].f32[2] = aesrand_int32_r(seed);
  st[2].f32[3] = aesrand_int32_r(seed);

  st[3].f32[0] = aesrand_int32_r(seed);
  st[3].f32[1] = aesrand_int32_r(seed);
  st[3].f32[2] = aesrand_int32_r(seed);
  st[3].f32[3] = aesrand_int32_r(seed);

	st[4].f32[0] = aesrand_int32_r(seed);
	st[4].f32[1] = aesrand_int32_r(seed);
	st[4].f32[2] = aesrand_int32_r(seed);
	st[4].f32[3] = aesrand_int32_r(seed);
}

void morus_640_iterate(state st) {
	// morus_640_print_state(st);
	xor_to_word(&(st[0]), &(st[3]));
	and_xor_to_word(&(st[0]), &(st[1]), &(st[2]));
	rotate_left_word(&(st[0]), 5);

	permute_words(&(st[3]), 32);

	xor_to_word(&(st[1]), &(st[4]));
	and_xor_to_word(&(st[1]), &(st[2]), &(st[3]));
	rotate_left_word(&(st[1]), 31);

	permute_words(&(st[4]), 64);

	xor_to_word(&(st[2]), &(st[0]));
	and_xor_to_word(&(st[2]), &(st[3]), &(st[4]));
	rotate_left_word(&(st[2]), 7);

	permute_words(&(st[0]), 96);

	xor_to_word(&(st[3]), &(st[1]));
	and_xor_to_word(&(st[3]), &(st[4]), &(st[0]));
	rotate_left_word(&(st[3]), 22);

	permute_words(&(st[1]), 64);

	xor_to_word(&(st[4]), &(st[2]));
	and_xor_to_word(&(st[4]), &(st[0]), &(st[1]));
	rotate_left_word(&(st[4]), 13);

	permute_words(&(st[2]), 32);

	// morus_640_print_state(st);
}

void morus_640_encrypt(union Register *out, state st) {
	// printf("-------------------------------------------- Message\n");

	out->f32[0] = st[1].f32[0];
	out->f32[1] = st[1].f32[1];
	out->f32[2] = st[1].f32[2];
	out->f32[3] = st[1].f32[3];

	xor_to_word(out, &(st[0]));

	permute_words(out, 96);

	and_xor_to_word(out, &(st[2]), &(st[3]));
	morus_640_iterate(st);
	// printf("-------------------------------------------- CipherText\n");
	// print_word(*out);
}
