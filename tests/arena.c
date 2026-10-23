
#define ARK_NAMESPACE
#include "arklib.h"

/* Simple high‑resolution timer (Linux) */
int main (void) {
	enum { N_ALLOCS = 1000000, ALLOC_SIZE = 32 };

	/* --- Benchmark a --- */
	ark_arena* a = unwrap (ark_make_arena (64 * 1024, ARK_ARENA_DEFAULT));

	uint64_t start = ark_kron_rdtsc ();
	for (int i = 0; i < N_ALLOCS; ++i) {
		void* p = ark_arena_alloc (a, ALLOC_SIZE, __alignof__ (max_align_t));
		/* prevent optimisation */
		__asm__ volatile ("" : : "r"(p) : "memory");
	}
	uint64_t arena_cycles = ark_kron_rdtsc () - start;

	ark_arena_reset (a);

	start = ark_kron_rdtsc ();
	for (int i = 0; i < N_ALLOCS; ++i) {
		void* p = ark_arena_alloc (a, ALLOC_SIZE, __alignof__ (max_align_t));
		__asm__ volatile ("" : : "r"(p) : "memory");
	}
	uint64_t arena_reset_cycles = ark_kron_rdtsc () - start;

	ark_destroy_arena (a);

	/* --- Benchmark malloc/free --- */
	void** ptrs = malloc (N_ALLOCS * sizeof (void*));
	if (!ptrs) abort ();

	start = ark_kron_rdtsc ();
	for (int i = 0; i < N_ALLOCS; ++i) {
		ptrs[i] = malloc (ALLOC_SIZE);
		__asm__ volatile ("" : : "r"(ptrs[i]) : "memory");
	}
	uint64_t malloc_cycles = ark_kron_rdtsc () - start;

	for (int i = 0; i < N_ALLOCS; ++i)
		free (ptrs[i]);

	start = ark_kron_rdtsc ();
	for (int i = 0; i < N_ALLOCS; ++i) {
		ptrs[i] = malloc (ALLOC_SIZE);
		__asm__ volatile ("" : : "r"(ptrs[i]) : "memory");
	}
	uint64_t malloc2_cycles = ark_kron_rdtsc () - start;

	for (int i = 0; i < N_ALLOCS; ++i)
		free (ptrs[i]);
	free (ptrs);

	/* --- Report --- */
	printf ("Allocations: %d x %zu bytes\n", N_ALLOCS, (size_t)ALLOC_SIZE);
	printf (
		"Arena (first run)  : %10lu cycles  (%7.2f ns/alloc)\n",
		(unsigned long)arena_cycles,
		(double)arena_cycles / N_ALLOCS * (1e9 / 3.0e9)
	); /* assume 3 GHz */
	printf (
		"Arena (after reset): %10lu cycles  (%7.2f ns/alloc)\n",
		(unsigned long)arena_reset_cycles,
		(double)arena_reset_cycles / N_ALLOCS * (1e9 / 3.0e9)
	);
	printf (
		"malloc (first run) : %10lu cycles  (%7.2f ns/alloc)\n",
		(unsigned long)malloc_cycles,
		(double)malloc_cycles / N_ALLOCS * (1e9 / 3.0e9)
	);
	printf (
		"malloc (second run): %10lu cycles  (%7.2f ns/alloc)\n",
		(unsigned long)malloc2_cycles,
		(double)malloc2_cycles / N_ALLOCS * (1e9 / 3.0e9)
	);

	return 0;
}
