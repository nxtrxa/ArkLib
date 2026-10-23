#if 0
/**
 * @file benchmark_ch.c
 * @brief Benchmark for the external‑chaining hashmap.
 *
 * Same workload and measurement methodology as benchmark_oa.c.
 */

	#include <stdint.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <time.h>

	#include "ark_xc_hashmap.h"

/* ------------------------------------------------------------------ */
/*  Simple PRNG (xorshift64*)                                          */
/* ------------------------------------------------------------------ */
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

static uint64_t int_hash (const void *key) {
	return (uint64_t)((const IntKey *)key)->value * 0x9E3779B97F4A7C15ULL;
}

static bool int_eq (const void *a, const void *b) {
	return ((const IntKey *)a)->value == ((const IntKey *)b)->value;
}

/* ------------------------------------------------------------------ */
/*  Helpers                                                            */
/* ------------------------------------------------------------------ */
static int64_t timespec_diff_ns (struct timespec end, struct timespec start) {
	return (end.tv_sec - start.tv_sec) * 1000000000L
		   + (end.tv_nsec - start.tv_nsec);
}

static void shuffle (void **arr, size_t n) {
	for (size_t i = n - 1; i > 0; i--) {
		size_t j   = (size_t)random_int ((int)(i + 1));
		void  *tmp = arr[i];
		arr[i]	   = arr[j];
		arr[j]	   = tmp;
	}
}

static void run_benchmark (size_t N) {
	printf ("  N = %zu\n", N);

	IntKey **keys = malloc (N * sizeof (IntKey *));
	for (size_t i = 0; i < N; i++) {
		keys[i]		   = malloc (sizeof (IntKey));
		keys[i]->value = (int)i;
	}
	shuffle ((void **)keys, N);

	double min_insert_ns = 1e18, min_lookup_ns = 1e18, min_mixed_ops = 0;
	double mixed_ops_count = (double)N;

	for (int trial = 0; trial < 3; trial++) {
		HashMap *map = hashmap_create (N / 2, int_hash, int_eq, NULL, NULL);
		if (!map) {
			fprintf (stderr, "hashmap_create failed\n");
			exit (1);
		}

		struct timespec t0, t1;

		/* ---------- Insert phase ---------- */
		clock_gettime (CLOCK_MONOTONIC, &t0);
		for (size_t i = 0; i < N; i++) {
			if (!hashmap_put (map, keys[i], keys[i])) {
				fprintf (stderr, "insert failed\n");
				exit (1);
			}
		}
		clock_gettime (CLOCK_MONOTONIC, &t1);
		double ins_ns = (double)timespec_diff_ns (t1, t0) / (double)N;
		if (ins_ns < min_insert_ns) min_insert_ns = ins_ns;

		/* ---------- Lookup phase ---------- */
		clock_gettime (CLOCK_MONOTONIC, &t0);
		for (size_t i = 0; i < N; i++) {
			if (hashmap_get (map, keys[i]) != keys[i]) {
				fprintf (stderr, "lookup failed\n");
				exit (1);
			}
		}
		clock_gettime (CLOCK_MONOTONIC, &t1);
		double lkup_ns = (double)timespec_diff_ns (t1, t0) / (double)N;
		if (lkup_ns < min_lookup_ns) min_lookup_ns = lkup_ns;

		/* ---------- Mixed phase ---------- */
		int *ops = malloc (N * sizeof (int));
		for (size_t i = 0; i < N / 2; i++) ops[i] = 0;
		for (size_t i = N / 2; i < N; i++) ops[i] = 1;
		for (size_t i = N - 1; i > 0; i--) {
			size_t j   = (size_t)random_int ((int)(i + 1));
			int	   tmp = ops[i];
			ops[i]	   = ops[j];
			ops[j]	   = tmp;
		}

		size_t lk_idx = 0, del_idx = N / 2;
		clock_gettime (CLOCK_MONOTONIC, &t0);
		for (size_t i = 0; i < N; i++) {
			if (ops[i] == 0) {
				if (hashmap_get (map, keys[lk_idx]) != keys[lk_idx]) {
					fprintf (stderr, "mixed lookup failed\n");
					exit (1);
				}
				lk_idx++;
			} else {
				if (!hashmap_remove (map, keys[del_idx])) {
					fprintf (stderr, "mixed delete failed\n");
					exit (1);
				}
				del_idx++;
			}
		}
		clock_gettime (CLOCK_MONOTONIC, &t1);
		double mix_time_s  = (double)timespec_diff_ns (t1, t0) * 1e-9;
		double mix_ops_sec = mixed_ops_count / mix_time_s;
		if (mix_ops_sec > min_mixed_ops) min_mixed_ops = mix_ops_sec;

		free (ops);
		hashmap_destroy (map);
	}

	printf ("    Insert : %.1f ns/op\n", min_insert_ns);
	printf ("    Lookup : %.1f ns/op\n", min_lookup_ns);
	printf ("    Mixed  : %.0f ops/s\n", min_mixed_ops);

	for (size_t i = 0; i < N; i++) free (keys[i]);
	free (keys);
}

int main (void) {
	/* Warm‑up */
	{
		HashMap *map = hashmap_create (0, int_hash, int_eq, NULL, NULL);
		IntKey	*keys[10000];
		for (int i = 0; i < 10000; i++) {
			keys[i]		   = malloc (sizeof (IntKey));
			keys[i]->value = i;
			hashmap_put (map, keys[i], keys[i]);
		}
		for (int i = 0; i < 10000; i++) hashmap_get (map, keys[i]);
		for (int i = 0; i < 10000; i++) hashmap_remove (map, keys[i]);
		hashmap_destroy (map);
		for (int i = 0; i < 10000; i++) free (keys[i]);
	}

	printf ("Chaining hashmap benchmark (min of 3 runs)\n");
	printf ("--------------------------------------------\n");

	run_benchmark (100000);
	run_benchmark (1000000);
	run_benchmark (10000000);

	return 0;
}
#endif
