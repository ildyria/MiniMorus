#include "main_mini_morus.h"

inline void copy_word(state_words* to, const state_words* from)
{
  *to = *from;
}

inline void save_state(state* state_saved, state state, int i)
{
  copy_word(&state_saved[i][0], &state[0]);
  copy_word(&state_saved[i][1], &state[1]);
  copy_word(&state_saved[i][2], &state[2]);
  copy_word(&state_saved[i][3], &state[3]);
  copy_word(&state_saved[i][4], &state[4]);
}

inline void sample(state* saved_state, state_words* saved_cipher, int num, unsigned int* seed)
{
  state state;
  state_words null_words;
  // state_words output;
  int i;
  rand_init(state, seed);
  null_word(&null_words);
  for (i = 0; i < num; i++) {
    save_state(saved_state, state, i);
    encrypt(&saved_cipher[i], null_words, state);
  }
  save_state(saved_state, state, i);
}

inline int linear_sample(unsigned int* seed)
{
  state saved_state[6];
  state_words saved_cipher[5];

  sample(saved_state, saved_cipher, 5, seed);
  return linear(saved_state, saved_cipher);
}

void linear_stats(unsigned long long num) {
    long long res = 0;
    long long inbalance = 0;
    unsigned long long bias = 0;
    unsigned long long i = 0;

    int tid;
    unsigned int seed;
    printf("--------------------------\n");
    printf("num:  %llu\n", (num* omp_get_max_threads()));
    printf("--------------------------\n");

    // omp_set_num_threads(16);
    # pragma omp parallel private(i, tid, seed) reduction(+:bias,inbalance)
    {
      tid = omp_get_thread_num();
      seed = 123456789 ^ tid;
      printf ( "  %6d  %12d\n", tid, seed);

      // try using openmp to speed things up
      #pragma omp parallel for reduction(+:bias,inbalance)
      for(i = 0 ; i < num; ++i)
      {
        res = linear_sample(&seed);
        inbalance += 1 - 2*res;
        bias += res;
        if((i & 0x3ffffff) == 0)
        {
          printf("sampling, %llu -- thread %d\n", i, tid);
        }
      }
    }

    printf("--------------------------\n");
    printf("num:  %llu\n", (num* omp_get_max_threads()));
    printf("inba: %lld\n", inbalance);
    printf("bias: %llu\n", bias);
    printf("--------------------------\n");
}

int main(int argc, char const *argv[]) {
  long long int num = 1;
  num <<= 33;
  linear_stats(num);
  return 0;
}