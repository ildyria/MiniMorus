#include <stdint.h>

typedef uint32_t S[5];

uint32_t rotate_left(uint32_t x, int bits)
{
  if (bits == 0) return x;
  return (x << bits) | (x >> (32 - bits));
}

void round(S state, uint32_t message) {
	state[0] ^= state[1] & state[2];
	state[0] ^= state[3];
	state[0] = rotate_left(state[0], 5);

	state[1] ^= message;
	state[1] ^= state[2] & state[3];
	state[1] ^= state[4];
	state[1] = rotate_left(state[1], 31);

	state[2] ^= message;
	state[2] ^= state[3] & state[4];
	state[2] ^= state[0];
	state[2] = rotate_left(state[2], 7);

	state[3] ^= message;
	state[3] ^= state[4] & state[0];
	state[3] ^= state[1];
	state[3] = rotate_left(state[3], 22);

	state[4] ^= message;
	state[4] ^= state[0] ^ state[1];
	state[4] ^= state[2];
	state[4] = rotate_left(state[4], 13);
}

void init(S state, uint32_t iv, uint32_t key, uint32_t const1, uint32_t const2){
	state[0] = iv;
	state[1] = key;
	state[2] = 0xffffffff;
	state[3] = const1;
	state[4] = const2;
	for (int i = 0; i < 16; ++i)
	{
		round(state,0);
	}
}

void encrypt(uint32_t message, S state)
{
	uint32_t mem = message;
	message ^= state[0];
	message ^= state[1];
	message ^= state[2] & state[3];
	round(state, mem);
	return message;
}




