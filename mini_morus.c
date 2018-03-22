#include "mini_morus.h"

// int log2_fast(double d) {
//     int result;
//     frexp(d, &result);
//     return result-1;
// }

inline uint32_t mask (uint32_t* mask, unsigned int i)
{
  *mask |= (1 << i);
  // return mask | (1 << i);
  return 0;
}

// Compute the Hamming Weight
inline uint32_t HW(uint32_t x)
{
    x = x - ((x >> 1) & 0x55555555);
    x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
    x = (x + (x >> 4)) & 0x0F0F0F0F;
    x = x + (x >> 8);
    x = x + (x >> 16);
    return x & 0x0000003F;
}

// Remark the XOR of the bits are equal to the parity of the Hamming Weight !

inline void save_state(S* state_saved, S state, int i)
{
  state_saved[i][0] = state[0];
  state_saved[i][1] = state[1];
  state_saved[i][2] = state[2];
  state_saved[i][3] = state[3];
  state_saved[i][4] = state[4];
}

inline void print_state(S state){
  printf("%08x %08x %08x %08x %08x\n", state[0], state[1], state[2], state[3], state[4]);
}

inline uint32_t rotate_left(uint32_t x, int bits)
{
  if (bits == 0) return x;
  return (x << bits) | (x >> (32 - bits));
}

void mini_morus_round(S state, uint32_t message) {
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
		mini_morus_round(state, 0);
	}
}

void rand_init(S state){
  // FILE * f = fopen("/dev/urandom", "rb");
  // fread(&state[0], 5*sizeof(uint32_t), 1, f);
  // fread(&state[1], sizeof(uint32_t), 1, f);
  // fread(&state[2], sizeof(uint32_t), 1, f);
  // fread(&state[3], sizeof(uint32_t), 1, f);
  // fread(&state[4], sizeof(uint32_t), 1, f);
	state[0] = rand();
	state[1] = rand();
	state[2] = rand();
	state[3] = rand();
	state[4] = rand();
}

uint32_t encrypt(uint32_t message, S state)
{
	uint32_t mem = message;
	message ^= state[0];
	message ^= state[1];
	message ^= state[2] & state[3];
	mini_morus_round(state, mem);
	return message;
}


inline void minimorus_sample(S* saved_state, uint32_t* saved_cipher, int num)
{
  S state;
  int i;
  rand_init(state);
  for (i = 0; i < num; i++) {
    save_state(saved_state,state,i);
    saved_cipher[i] = encrypt(0, state);
  }
}

long long minimorus_linearsample()
{
  S saved_state[5];
  uint32_t saved_cipher[5];

  uint32_t mask1 = 0;
  uint32_t mask2 = 0;
  uint32_t mask3 = 0;
  uint32_t mask4 = 0;
  uint32_t mask5 = 0;

  uint32_t res = 0;
  minimorus_sample(saved_state, saved_cipher, 5);

  // # Approx 2 for S2
  // # TESTED WITH 2^-22
  // return xor([mask(C[1], [2]),
  //             mask(C[2], [1,7,15,27]),
  //             mask(C[3], [6,20,14]),
  //             mask(C[4], [19]),
  //             mask(S[2][-1][2], [0])]) # 2^-9


  mask(&mask1,2);
  res ^= saved_cipher[0] & mask1;

  mask(&mask2,1);
  mask(&mask2,7);
  mask(&mask2,15);
  mask(&mask2,27);
  res ^= saved_cipher[1] & mask2;

  mask(&mask3,6);
  mask(&mask3,20);
  mask(&mask3,14);
  res ^= saved_cipher[2] & mask3;

  mask(&mask4,19);
  res ^= saved_cipher[3] & mask4;

  mask(&mask5,0);
  res ^= saved_state[2][2] & mask5;

  return 1 & HW(res);
}


void minimorus_linearstats(unsigned long long num) {
    long long res = 0;
    long long inbalance = 0;
    unsigned long long bias = 0;
    unsigned long long i = 0;
    long double probability;
    long double correlation;
    long double potential;
    long double biases;
    long double biases_cost;
    int tid;

    printf("num:  %llu\n", num);
    printf("--------------------------\n");

    // try using openmp to speed things up
    #pragma omp parallel for private(res, tid) reduction(+:bias,inbalance)
    for(i = 0 ; i < num; ++i)
    {
      res = minimorus_linearsample();
      // inbalance += (-1)^res // this is -1 if odd 1 if even
      // printf("1 -> -1 = %i\n",(~(1-1) << 1) + 1);
      // printf("0 ->  1 = %i\n",(~(0-1) << 1) + 1);
      inbalance += (~((res &1) -1) << 1) + 1;
      bias += res;
      if((i & 0x3fffff) == 0)
      {
        tid = omp_get_thread_num();
        // printf("sampling, %llu\n", i);
        printf("sampling, %llu -- thread %d\n", i, tid);
      }
    }

    // probability = float(bias)/num
    // correlation = imbalance2 / float(num)
    // bias = (bias - num/2)/float(num)
    // potential = correlation**2

    // needs to be verified...
    probability = (long double) bias / num;
    correlation = (long double) inbalance / num;
    biases = (long double) (bias - (num/2)) / num;
    potential = correlation * correlation;
    printf("--------------------------\n");
    printf("num:  %llu\n", num);
    printf("inba: %lld\n", inbalance);
    printf("bias: %llu\n", bias);
    printf("--------------------------\n");
    printf("prob: %Lf\n", probability);
    printf("corr: %Lf\n", correlation);
    printf("bias: %Lf\n", biases);
    printf("ptnt: %Lf\n", potential);
    printf("logD: %Lf\n", log2l(1/(biases*biases)));

    biases_cost = fabsl(log2(2*fabsl(biases)));
    printf("bias: +- 2^-%Lf\n", biases_cost);

}

int main(int argc, char const *argv[]) {

  /* Intializes random number generator */
  long long int num = 1;
  num <<= 25;
  srand((unsigned) time(NULL));
  minimorus_linearstats(num);
  return 0;
}
