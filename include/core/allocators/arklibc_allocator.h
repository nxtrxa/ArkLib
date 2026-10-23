/*
 * Copyright (c) 2026 Notrya A. ad' Avlis. All Rights Reserved.
 *
 *              ARKLIB LIBC_ALLOCATOR
 *
 *  ARK_LIBC_ALLOCATOR implements wrappers for standard MALLOC, REALLOC and FREE,
 *  or if supported, the bultin versions of these.
 *
 */

#ifndef ARK_LIBC_ALLOCATOR_H_
#define ARK_LIBC_ALLOCATOR_H_

#include "interfaces/arkallocator_interface.h"

ARK_BEGIN

#ifdef ARK_NAMESPACE
	#define LIBC_ALLOCATOR ARK_LIBC_ALLOCATOR
#endif

#ifndef ARK_LIBC_ALLOCATOR_ALLOC
	#define ARK_LIBC_ALLOCATOR_ALLOC ark_libc_alloc
#endif
#ifndef ARK_LIBC_ALLOCATOR_REALLOC
	#define ARK_LIBC_ALLOCATOR_REALLOC ark_libc_realloc
#endif
#ifndef ARK_LIBC_ALLOCATOR_DEALLOC
	#define ARK_LIBC_ALLOCATOR_DEALLOC ark_libc_dealloc
#endif

#ifndef ARK_LIBC_ALLOCATOR
	#define ARK_LIBC_ALLOCATOR                                                                                         \
		((ark_allocator) {                                                                                             \
			._vptr = &ark_libc_allocator_vtable,                                                                       \
		})
#endif

ARKAPI const ark_allocator_vtable ark_libc_allocator_vtable;

ARKAPI __decl_malloc__ __decl_nodiscard__ u0* __nullable__
ark_libc_alloc (u0* __restrict__ ctx_, sz size_, sz alignment_) __attr_alloc_params__ (2, 3);

ARKAPI __decl_realloc__ (ark_libc_alloc, 2) u0* __nullable__ ark_libc_realloc (
	u0* __restrict__ ctx_, u0* ptr_, sz old_sz_, sz new_sz_, sz alignment_
) __attr_alloc_params__ (4, 5);

ARKAPI u0 ark_libc_dealloc (u0* __restrict__ ctx_, u0* __restrict__ ptr_);

ARK_END

#endif /* ARK_LIBC_ALLOCATOR_H_ */
