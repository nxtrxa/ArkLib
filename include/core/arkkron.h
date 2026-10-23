#ifndef ARK_KRON_H_
#define ARK_KRON_H_

#include "internals/arkdefs.h"

/*
 *   Simple high‑resolution timer (Linux)
 */
__decl_static_inline__ uint64_t ark_kron_rdtsc (void) {
	uint32_t lo, hi;
	__asm__ volatile ("rdtsc" : "=a"(lo), "=d"(hi));
	return ((uint64_t)hi << 32) | lo;
}

#endif /* ARK_KRON_H_ */
