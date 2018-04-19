#include "mini_morus_1280.h"

void mini_morus_1280_print_state(state state) {
	printf("%016lx %016lx %016lx %016lx %016lx\n", state[0].m64, state[1].m64, state[2].m64, state[3].m64,
	       state[4].m64);
}

void mini_morus_1280_copy_state(state to, state from) {
	to[0].m64 = from[0].m64;
	to[1].m64 = from[1].m64;
	to[2].m64 = from[2].m64;
	to[3].m64 = from[3].m64;
	to[4].m64 = from[4].m64;
}

void mini_morus_1280_rand_init(state st, struct RNG_state *seed) {
	st[0].m64 = aesrand_int64_r(seed);
	st[1].m64 = aesrand_int64_r(seed);
	st[2].m64 = aesrand_int64_r(seed);
	st[3].m64 = aesrand_int64_r(seed);
	st[4].m64 = aesrand_int64_r(seed);
}

void mini_morus_1280_iterate(state st) {
	st[0].m64 ^= st[1].m64 & st[2].m64;
	st[0].m64 ^= st[3].m64;
	st[0].m64 = rotate_left_64(st[0].m64, 13);

//    st[1].m64 ^= message;
	st[1].m64 ^= st[2].m64 & st[3].m64;
	st[1].m64 ^= st[4].m64;
	st[1].m64 = rotate_left_64(st[1].m64, 46);

//    st[2].m64 ^= message;
	st[2].m64 ^= st[3].m64 & st[4].m64;
	st[2].m64 ^= st[0].m64;
	st[2].m64 = rotate_left_64(st[2].m64, 38);

//    st[3].m64 ^= message;
	st[3].m64 ^= st[4].m64 & st[0].m64;
	st[3].m64 ^= st[1].m64;
	st[3].m64 = rotate_left_64(st[3].m64, 7);

//    st[4].m64 ^= message;
	st[4].m64 ^= st[0].m64 & st[1].m64;
	st[4].m64 ^= st[2].m64;
	st[4].m64 = rotate_left_64(st[4].m64, 4);
}

void mini_morus_1280_encrypt(union Register *out, state st) {
	out->m64 = 0;
	out->m64 ^= st[0].m64;
	out->m64 ^= st[1].m64;
	out->m64 ^= st[2].m64 & st[3].m64;
	mini_morus_1280_iterate(st);
}