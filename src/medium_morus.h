#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "aes-rng.h"

typedef uint64_t state_words;
typedef state_words state[5];

void print_state(state st);

void rand_init(state st, struct RNG_state* rng);

void iterate(state st, state_words message);

void encrypt(state_words* out, state_words message, state state);

int linear(state* saved_state, state_words* saved_cipher);

void null_word(state_words* w);
