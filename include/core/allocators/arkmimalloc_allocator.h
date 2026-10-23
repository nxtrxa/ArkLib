/*-----------------------------------------------------------------------
 * @Copyright (c) 2026 Notrya A. ad' Avlis. All Rights Reserved.
 *
 *                      ARKLIB MIMALLOC_ALLOCATOR
 *
 * Wrapper for Microsoft malloc implemention.
 *
 * mimalloc (pronounced "me-malloc") is a general purpose allocator with excellent performance characteristics.
 * Initially developed by Daan Leijen for the runtime systems of the Koka and Lean languages.
 *
 * https://github.com/microsoft/mimalloc
 *----------------------------------------------------------------------*/
#ifndef ARK_MIMALLOC_ALLOCATOR_H_
#define ARK_MIMALLOC_ALLOCATOR_H_

#include "interfaces/arkallocator.h"
#include "mimalloc/include/mimalloc.h"

#define ARK_MIMALLOC_ALLOCATOR                                                                                         \
	(&(ark_allocator) {                                                                                                \
		.alloc	 = (ark_alloc_fn_t)mi_malloc,                                                                          \
		.realloc = (ark_realloc_fn_t)mi_realloc,                                                                       \
		.dealloc = (ark_dealloc_fn_t)mi_free,                                                                          \
		.type	 = ARK_DYNAMIC_ALLOCATOR,                                                                              \
	})

#endif /* ARK_MIMALLOC_ALLOCATOR_H_ */
