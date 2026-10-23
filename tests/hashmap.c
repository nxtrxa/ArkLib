#include <time.h>

#include "ark_xchashmap.h"
#include "arklib.h"

static uint64_t prng_state = 123456789ULL;

static inline uint64_t prng_u64 (void) {
	uint64_t x = prng_state;
	x ^= x >> 12;
	x ^= x << 25;
	x ^= x >> 27;
	prng_state = x;
	return x * 0x2545F4914F6CDD1DULL;
}

static inline int random_int (int range) {
	return (int)(prng_u64 () % (uint64_t)range);
}

/* ------------------------------------------------------------------ */
/*  Key wrapper                                                        */
/* ------------------------------------------------------------------ */
typedef struct {
	int value;
} IntKey;

static u64 int_hash (const void *key) {
	return (u64)((const IntKey *)key)->value * 0x9E3779B97F4A7C15ULL;
}

static bool int_eq (const void *a, const void *b) {
	return ((const IntKey *)a)->value == ((const IntKey *)b)->value;
}

int main () {
	ark_xchashmap *map = ark_make_xchasmap ();
	ark_destroy_xchashmap (map);
}

