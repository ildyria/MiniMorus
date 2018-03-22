#include <stdint.h>

typedef uint32_t S[4];

uint32_t rotate_left(uint32_t x, int bits)
{
  if (bits == 0) return x;
  return (x << bits) | (x >> (32 - bits));
}

void rotate_left_state(S state, int bits)
{
	for (int i = 0; i < 4; ++i)
	{
		state[i] = rotate_left(state[i],bits);
	}
}

void rotate_right_state(S state, int bits)
{
	for (int i = 0; i < 4; ++i)
	{
		state[i] = rotate_left(state[i],32-bits);
	}
}

void and_state(S o, S s1 , S s2)
{
	for (int i = 0; i < 4; ++i)
	{
		o[i] = s1[i] ^ s2[i]
	}
}

void xor_state(S o, S s)
{
	for (int i = 0; i < 4; ++i)
	{
		o[i] ^= s[i]
	}
}

void permute_state(S s, int bits)
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
		t = s[1]
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





