#include <stdint.h>
#include <stdio.h>
#include <time.h>
#ifdef _OPENMP
#include <omp.h>
#else
#define omp_get_max_threads(x) 1
#define omp_get_thread_num(x) 1
#endif
#include <math.h>
#include "morus.h"
#include "aes-rng.h"
