#include "mini_morus_640.h"

inline void mini_morus_640_print_state(state state) {
	printf("%08x %08x %08x %08x %08x\n", state[0].m32, state[1].m32, state[2].m32, state[3].m32, state[4].m32);
}

void mini_morus_640_copy_state(state to, state from) {
	to[0].m32 = from[0].m32;
	to[1].m32 = from[1].m32;
	to[2].m32 = from[2].m32;
	to[3].m32 = from[3].m32;
	to[4].m32 = from[4].m32;
}

void mini_morus_640_rand_init(state st, struct RNG_state *seed) {
	st[0].m32 = aesrand_int32_r(seed);
	st[1].m32 = aesrand_int32_r(seed);
	st[2].m32 = aesrand_int32_r(seed);
	st[3].m32 = aesrand_int32_r(seed);
	st[4].m32 = aesrand_int32_r(seed);
}

void mini_morus_640_iterate(state st) {
	st[0].m32 ^= st[1].m32 & st[2].m32;
	st[0].m32 ^= st[3].m32;
	st[0].m32 = rotate_left_32(st[0].m32, 5);

//    st[1].m32 ^= message;
	st[1].m32 ^= st[2].m32 & st[3].m32;
	st[1].m32 ^= st[4].m32;
	st[1].m32 = rotate_left_32(st[1].m32, 31);

//    st[2].m32 ^= message;
	st[2].m32 ^= st[3].m32 & st[4].m32;
	st[2].m32 ^= st[0].m32;
	st[2].m32 = rotate_left_32(st[2].m32, 7);

//    st[3].m32 ^= message;
	st[3].m32 ^= st[4].m32 & st[0].m32;
	st[3].m32 ^= st[1].m32;
	st[3].m32 = rotate_left_32(st[3].m32, 22);

//    st[4].m32 ^= message;
	st[4].m32 ^= st[0].m32 & st[1].m32;
	st[4].m32 ^= st[2].m32;
	st[4].m32 = rotate_left_32(st[4].m32, 13);
}

void mini_morus_640_encrypt(union Register *out, state st) {
	out->m32 = 0;
	out->m32 ^= st[0].m32;
	out->m32 ^= st[1].m32;
	out->m32 ^= st[2].m32 & st[3].m32;
	mini_morus_640_iterate(st);
}
