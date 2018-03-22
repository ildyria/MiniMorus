#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <omp.h>

typedef uint32_t S[5];

// Helper function to generate the mask for Linear correlation

inline uint32_t mask (uint32_t* mask, unsigned int i);

// Compute the Hamming Weight
inline uint32_t HW(uint32_t x);

// Remark the XOR of the bits are equal to the parity of the Hamming Weight !

void print_state(S state);

inline uint32_t rotate_left(uint32_t x, int bits);

void mini_morus_round(S state, uint32_t message);

void init(S state, uint32_t iv, uint32_t key, uint32_t const1, uint32_t const2);

uint32_t encrypt(uint32_t message, S state);

long long minimorus_linearsample();

void minimorus_linearstats(unsigned long long num);
