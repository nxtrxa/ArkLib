/*
 * Copyright (c) 2026 Notrya A. ad' avlis. All Rights Reserved.
 *
 *                      ARKLIB MONOTONIC_BUFFER ALLOCATOR
 *
 * ARK_MONOTONIC_BUFFER implements a bump allocator that yields
 * a stack allocated buffer.
 *
 */

#ifndef ARK_MONOTONIC_BUFFER_H_
#define ARK_MONOTONIC_BUFFER_H_

#include "interfaces/arkallocator_interface.h"

ARK_BEGIN

#ifdef ARK_NAMESPACE
	#define MBA						   ARK_MBA
	#define MONOTONIC_BUFFER_ALLOCATOR ARK_MONOTONIC_BUFFER_ALLOCATOR
	#define monotonic_buffer		   ark_monotonic_buffer
	#define monotonic_buffer_alloc	   ark_monotonic_buffer_alloc
	#define monotonic_buffer_resert	   ark_monotonic_buffer_reset
	#define monotonic_buffer_rewind	   ark_monotonic_buffer_rewind
#endif

static constexpr bool ARK_MONOTONIC_BUFFER_ALLOCATOR_INVALID_CONTEXT = false;

#define ARK_MONOTONIC_BUFFER_ALLOCATOR(Buffer_)                                                                        \
	((ark_allocator) {                                                                                                 \
		._ctx  = ($typematch ((Buffer_), (ark_internal_symbol (monotonic_buffer)*)0))                                  \
					 ? (Buffer_)                                                                                       \
					 : $assert (ARK_MONOTONIC_BUFFER_ALLOCATOR_INVALID_CONTEXT),                                       \
		._vptr = &ark_monotonic_buffer_allocator_vtable,                                                               \
	})

#define ARK_MBA ARK_MONOTONIC_BUFFER_ALLOCATOR

#ifndef ARK_GL_MONOTONIC_BUFFER_INITIAL_CAPACITY
	#define ARK_GL_MONOTONIC_BUFFER_INITIAL_CAPACITY KiB (4)
#endif

ARK_INTERNAL_CLASS (monotonic_buffer) {
	u8* base;
	sz	capacity;
	sz	used;
};

ARKAPI const ark_allocator_vtable ark_monotonic_buffer_allocator_vtable;

#define ark_monotonic_buffer(...) ark_pp_concat (ark_monotonic_buffer_, ark_pp_nargs (__VA_ARGS__)) (__VA_ARGS__)

#define ark_monotonic_buffer_1(Buffer_) ark_monotonic_buffer_2 (Buffer_, ARK_GL_MONOTONIC_BUFFER_INITIAL_CAPACITY)

#define ark_monotonic_buffer_2(Buffer_, Cap_)                                                                          \
	alignas (max_align_t) u8 ark_pp_concat (Buffer_, _buffer)[Cap_] = {};                                              \
	ark_internal_symbol (monotonic_buffer)* Buffer_					= (&(ark_internal_symbol (monotonic_buffer)) {     \
		.base	  = ark_pp_concat (Buffer_, _buffer),                                                                  \
		.capacity = Cap_,                                                                                              \
	})

#define ark_monotonic_buffer_3(Buffer_, Cap_, Align_)                                                                  \
	alignas (Align_) u8 ark_pp_concat (Buffer_, _buffer)[Cap_] = {};                                                   \
	ark_internal_symbol (monotonic_buffer)* Buffer_			   = (&(ark_internal_symbol (monotonic_buffer)) {          \
		.base	  = ark_pp_concat (Buffer_, _buffer),                                                                  \
		.capacity = Cap_,                                                                                              \
	})

ARKAPI __decl_malloc__ __decl_nodiscard__ u0* __nullable__ ark_monotonic_buffer_alloc (
	ark_internal_symbol (monotonic_buffer) * __noalias__ ctx_, sz alloc_sz_, sz alignment_
) __attr_alloc_params__ (2, 3);

ARKAPI u0 ark_monotonic_buffer_reset (ark_internal_symbol (monotonic_buffer) * __noalias__ mbuff_);

ARKAPI u0 ark_monotonic_buffer_rewind (ark_internal_symbol (monotonic_buffer) * __noalias__ mbuff_, sz mark_);

ARK_END

#endif /* ARK_MONOTONIC_BUFFER_H_ */
