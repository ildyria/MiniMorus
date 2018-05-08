#include "full_morus_1280.h"

#ifdef __AVX2__

/*define data alignment for different C compilers*/
#if defined(__GNUC__)
#define DATA_ALIGN32(x) x __attribute__ ((aligned(32)))
#else
#define DATA_ALIGN32(x) __declspec(align(32)) x
#endif

#ifdef _MSC_VER
#define  inline __inline
#endif

#define n1 13
#define n2 46
#define n3 38
#define n4 7
#define n5 4

#define XOR_256(x,y)      _mm256_xor_si256((x),(y))        /*XOR_256(x,y) = x ^ y, where x and y are two 256-bit word*/
#define AND_256(x,y)      _mm256_and_si256((x),(y))        /*AND(x,y) = x & y, where x and y are two 256-bit word*/

#define ROTL_256(x,n)      XOR_256( _mm256_slli_epi64((x), (n)), _mm256_srli_epi64((x),(64-n)) )   /*Rotate 4 64-bit unsigned integers in x to the left by n-bit positions*/

#define ROTL_256_64(x)     _mm256_permute4x64_epi64((x), _MM_SHUFFLE(2,1,0,3))  /*Rotate x by 64-bit  positions to the left*/
#define ROTL_256_128(x)    _mm256_permute4x64_epi64((x), _MM_SHUFFLE(1,0,3,2))  /*Rotate x by 128-bit positions to the left*/
#define ROTL_256_192(x)    _mm256_permute4x64_epi64((x), _MM_SHUFFLE(0,3,2,1))  /*Rotate x by 192-bit positions to the left*/

void morus_1280_print_state(state state) {
	// printf("--------------------------------------------\n");
	// printf("   S0   |   S1   |   S3   |   S3   |   S4   \n");
	// printf("%08x %08x %08x %08x %08x\n", state[0].f64[3], state[1].f64[3], state[2].f64[3], state[3].f64[3], state[4].f64[3]);
	// printf("%08x %08x %08x %08x %08x\n", state[0].f64[2], state[1].f64[2], state[2].f64[2], state[3].f64[2], state[4].f64[2]);
	// printf("%08x %08x %08x %08x %08x\n", state[0].f64[1], state[1].f64[1], state[2].f64[1], state[3].f64[1], state[4].f64[1]);
	// printf("%08x %08x %08x %08x %08x\n", state[0].f64[0], state[1].f64[0], state[2].f64[0], state[3].f64[0], state[4].f64[0]);
}

void morus_1280_copy_state(state to, state from) {
	to[0].f64 = from[0].f64;
	to[1].f64 = from[1].f64;
	to[2].f64 = from[2].f64;
	to[3].f64 = from[3].f64;
	to[4].f64 = from[4].f64;
}

inline void print_word(union Register *word) {
	// printf("%08x %08x %08x %08x\n", word->f64[0], word->f64[1], word->f64[2], word->f64[3]);
}

void morus_1280_print(state *saved_state, union Register *saved_cipher) {
	printf("-------------------------------------------- Ciphers\n");
	print_word(&(saved_cipher[0]));
	print_word(&(saved_cipher[1]));
	print_word(&(saved_cipher[2]));
	print_word(&(saved_cipher[3]));
	print_word(&(saved_cipher[4]));
	printf("-------------------------------------------- States\n");
	morus_1280_print_state(saved_state[0]);
	morus_1280_print_state(saved_state[1]);
	morus_1280_print_state(saved_state[2]);
	morus_1280_print_state(saved_state[3]);
	morus_1280_print_state(saved_state[4]);
	morus_1280_print_state(saved_state[5]);
}

void morus_1280_rand_init(state st, struct RNG_state *seed) {

  st[0].f64 = aesrand_m256i_r(seed);
  st[1].f64 = aesrand_m256i_r(seed);
  st[2].f64 = aesrand_m256i_r(seed);
  st[3].f64 = aesrand_m256i_r(seed);
	st[4].f64 = aesrand_m256i_r(seed);
}

void morus_1280_iterate(state st) {

	st[0].f64 = XOR_256(st[0].f64, st[3].f64);
	st[0].f64 = XOR_256(st[0].f64, AND_256(st[1].f64, st[2].f64));
	st[0].f64 = ROTL_256(st[0].f64, n1);
	st[3].f64 = ROTL_256_64(st[3].f64);

	st[1].f64 = XOR_256(st[1].f64, st[4].f64);
	st[1].f64 = XOR_256(st[1].f64, AND_256(st[2].f64, st[3].f64));
	st[1].f64 = ROTL_256(st[1].f64, n2);
	st[4].f64 = ROTL_256_128(st[4].f64);

	st[2].f64 = XOR_256(st[2].f64, st[0].f64);
	st[2].f64 = XOR_256(st[2].f64, AND_256(st[3].f64, st[4].f64));
	st[2].f64 = ROTL_256(st[2].f64, n3);
	st[0].f64 = ROTL_256_192(st[0].f64);

	st[3].f64 = XOR_256(st[3].f64, st[1].f64);
	st[3].f64 = XOR_256(st[3].f64, AND_256(st[4].f64, st[0].f64));
	st[3].f64 = ROTL_256(st[3].f64, n4);
	st[1].f64 = ROTL_256_128(st[1].f64);

	st[4].f64 = XOR_256(st[4].f64, st[2].f64);
	st[4].f64 = XOR_256(st[4].f64, AND_256(st[0].f64, st[1].f64));
	st[4].f64 = ROTL_256(st[4].f64, n5);
	st[2].f64 = ROTL_256_64(st[2].f64);
}

void morus_1280_encrypt(union Register *out, state st) {
	// printf("-------------------------------------------- Message\n");

	out->f64 = XOR_256(st[0].f64, ROTL_256_192(st[1].f64));
	out->f64 = XOR_256(out->f64, AND_256(st[2].f64, st[3].f64));

	morus_1280_iterate(st);
	// printf("-------------------------------------------- CipherText\n");
	// print_word(*out);
}

#else

void morus_1280_print_state(state st)
{
	die ("AVX2 support not enabled!\n");
}

void morus_1280_print(state *saved_state, union Register *saved_cipher)
{
	die ("AVX2 support not enabled!\n");
}

void morus_1280_copy_state(state to, state from)
{
	die ("AVX2 support not enabled!\n");
}

void morus_1280_rand_init(state st, struct RNG_state *rng)
{
	die ("AVX2 support not enabled!\n");
}

void morus_1280_iterate(state st)
{
	die ("AVX2 support not enabled!\n");
}

void morus_1280_encrypt(union Register *out, state state)
{
	die ("AVX2 support not enabled!\n");
}

#endif
