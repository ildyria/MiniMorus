#include <immintrin.h>

#define XOR_128(x,y)      _mm_xor_si128((x),(y))     /*XOR_128(x,y) = x ^ y, where x and y are two 128-bit word*/
#define AND_128(x,y)      _mm_and_si128((x),(y))     /*AND_128(x,y) = x & y, where x and y are two 128-bit word*/
