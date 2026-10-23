/*
 * Copyright (c) 2026 Murilo Gonzalez. All Rights Reserved.
 *
 *                      ARKLIB ARENA
 *
 * ARK_SYNC_ARENA have an internal linked-list of regions, allocated via MMAP.
 * it uses bump pointer until its region capacity is reached, once
 * reached, a new region is mapped and added to the linked-list.
 *
 * CONSTRAINTS:
 *
 *  GROWTH_FACTOR_PER_REGION = 2
 *  DEFAULT_REGION_SIZE      = 64Kb
 *  MAX_REGION_SIZE          = 1Gb
 *  HUGE_PAGE_SIZE           = 2Mb
 *  PAGE_SIZE                = 4Kb
 *
 */

#ifndef ARKLIB_SYNC_ARENA_H_
#define ARKLIB_SYNC_ARENA_H_

#import "core/arkexpect.h"
#import "interfaces/arkallocator_interface.h"

ARK_BEGIN

#ifdef ARK_NAMESPACE
	#define SYNC_ARENA_ALLOCATOR	 ARK_SYNC_ARENA_ALLOCATOR
	#define SYNC_ARENA_DEFAULT		 ARK_SYNC_ARENA_DEFAULT
	#define SYNC_ARENA_GUARD_PAGES	 ARK_SYNC_ARENA_GUARD_PAGES
	#define SYNC_ARENA_HUGE_PAGES	 ARK_SYNC_ARENA_HUGE_PAGES
	#define SYNC_ARENA_ZERO_ON_RESET ARK_SYNC_ARENA_ZERO_ON_RESET
	#define class_sync_arena		 ark_class_sync_arena
	#define class_sync_arena_unsafe	 ark_class_sync_arena_unsafe
	#define arena					 ark_sync_arena
	#define make_arena				 ark_make_sync_arena
	#define reset_arena				 ark_reset_sync_arena
	#define destroy_arena			 ark_destroy_sync_arena
	#define sync_arena_cleanup		 ark_sync_arena_cleanup
	#define sync_arena_alloc		 ark_sync_arena_alloc
#endif

#define ARK_SYNC_ARENA_ALLOCATOR(Arena_)                                                                               \
	({                                                                                                                 \
		$static_assert ($typematch ((Arena_), ark_sync_arena*), "invalid context");                                    \
		(ark_allocator) {                                                                                              \
			._ctx  = (Arena_),                                                                                         \
			._vptr = &ark_sync_arena_allocator_vtable,                                                                 \
		};                                                                                                             \
	})

#define ark_class_sync_arena(Exception_Handler_, Arena_, ...)                                                          \
	__attr_cleanup__ (ark_sync_arena_cleanup) ark_sync_arena* Arena_                                                   \
		= ark_catch (ark_make_sync_arena (__VA_ARGS__), ___except__, Exception_Handler_ (___except__))

#define ark_class_sync_arena_unsafe(Arena_, ...)                                                                       \
	__attr_cleanup__ (ark_sync_arena_cleanup) ark_sync_arena* Arena_ = ark_make_sync_arena (__VA_ARGS__).unwrap

enum __attr_flag_enum__ : flag_t {
	ARK_SYNC_ARENA_DEFAULT		 = 0,
	ARK_SYNC_ARENA_GUARD_PAGES	 = 1 << 0,
	ARK_SYNC_ARENA_HUGE_PAGES	 = 1 << 1,
	ARK_SYNC_ARENA_ZERO_ON_RESET = 1 << 2,
};

ARK_CLASS (ark_sync_arena);
struct region;

ARK_DECL_FAULTS (ARK_SYNC_ARENA, MALLOC, MUTEX_INIT);

ARKAPI const ark_allocator_vtable ark_sync_arena_allocator_vtable;

ARKAPI __decl_overloadable__ __decl_malloc__ __decl_nodiscard__
	ark_expect (ARK_SYNC_ARENA, ark_sync_arena_p) ark_make_sync_arena (sz cap_, flag_t flags_) __attr_noexcept__;

__decl_static_inline__ __decl_overloadable__ __decl_malloc__ __decl_nodiscard__
ark_expect (ARK_SYNC_ARENA, ark_sync_arena_p) ark_make_sync_arena (u0) __attr_noexcept__ {
	return ark_make_sync_arena (0, ARK_SYNC_ARENA_DEFAULT);
}

__decl_static_inline__ __decl_overloadable__ __decl_malloc__ __decl_nodiscard__
ark_expect (ARK_SYNC_ARENA, ark_sync_arena_p) ark_make_sync_arena (sz cap_) __attr_noexcept__ {
	return ark_make_sync_arena (cap_, ARK_SYNC_ARENA_DEFAULT);
}

__decl_static_inline__ __decl_overloadable__ __decl_malloc__ __decl_nodiscard__
ark_expect (ARK_SYNC_ARENA, ark_sync_arena_p) ark_make_sync_arena (flag_t flags_) __attr_noexcept__ {
	return ark_make_sync_arena (0, flags_);
}

ARKAPI u0 ark_reset_sync_arena (ark_sync_arena* __noalias__ arena_) __attr_noexcept__;

ARKAPI u0 ark_destroy_sync_arena (ark_sync_arena* __noalias__ arena_) __attr_noexcept__;

ARKAPI __decl_malloc__ __decl_returns_nonnull__ __decl_nodiscard__ u0* ark_sync_arena_alloc (
	ark_sync_arena* __noalias__ arena_, sz size_, sz alignment_
) __attr_alloc_params__ (2, 3) __attr_noexcept__;

__decl_static_inline__ u0 ark_sync_arena_cleanup (ark_sync_arena** __noalias__ arena_) {
	ark_destroy_sync_arena (*arena_);
}

ARK_END

#endif /* ARKLIB_SYNC_ARENA_H_ */
