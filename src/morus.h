#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef uint32_t state_words[4];
typedef state_words state[5];

void print_state(state st);

void rand_init(state st, unsigned int* seed);

void iterate(state st, state_words message);

state_words encrypt(state_words message, state state);

int linear(state* saved_state, state_words* saved_cipher);
