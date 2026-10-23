/*
 * Copyright (c) 2026 Murilo Gonzalez. All Rights Reserved.
 *
 *                      ARKLIB ARENA (async)
 *
 * Lock-free bump allocator with generation-counter rollback safety.
 * The fast path never takes a lock; the slow path (region growth) and
 * rollback operations (reset / temp_end) take a per-arena mutex.
 *
 */

#ifndef ARKLIB_ASYNC_ARENA_H_
#define ARKLIB_ASYNC_ARENA_H_

#import "core/arkexpect.h"
#import "interfaces/arkallocator_interface.h"

ARK_BEGIN

#ifdef ARK_NAMESPACE
	#define ASYNC_ARENA_ALLOCATOR	 ARK_ASYNC_ARENA_ALLOCATOR
	#define ASYNC_ARENA_DEFAULT		 ARK_ASYNC_ARENA_DEFAULT
	#define ASYNC_ARENA_GUARD_PAGES	 ARK_ASYNC_ARENA_DEFAULT
	#define ASYNC_ARENA_HUGE_PAGES	 ARK_ASYNC_ARENA_HUGE_PAGES
	#define ARENA_ZERO_ON_RESET		 ARK_ASYNC_ARENA_ZERO_ON_RESET
	#define class_async_arena		 ark_class_async_arena
	#define class_async_arena_unsafe ark_class_async_arena_unsafe
	#define scoped_arena_resource	 ark_scoped_arena_resource
	#define async_arena				 ark_async_arena
	#define make_async_arena		 ark_make_async_arena
	#define reset_async_arena		 ark_reset_async_arena
	#define destroy_async_arena		 ark_destroy_async_arena
	#define async_arena_cleanup		 ark_async_arena_cleanup
	#define async_arena_alloc		 ark_async_arena_alloc
#endif

#define ARK_ASYNC_ARENA_ALLOCATOR(Arena_)                                                                              \
	({                                                                                                                 \
		$static_assert ($typematch ((Arena_), ark_async_arena*), "invalid context");                                   \
		(ark_allocator) {                                                                                              \
			._ctx  = (Arena_),                                                                                         \
			._vptr = &ark_async_arena_allocator_vtable,                                                                \
		};                                                                                                             \
	})

#define ark_class_async_arena(Exception_Handler_, Arena_, ...)                                                         \
	__attr_cleanup__ (ark_async_arena_cleanup) ark_async_arena* Arena_                                                 \
		= ark_catch (ark_make_async_arena (__VA_ARGS__), ___except__, Exception_Handler_ (___except__))

#define ark_class_async_arena_unsafe(Arena_, ...)                                                                      \
	__attr_cleanup__ (ark_async_arena_cleanup) ark_async_arena* Arena_ = ark_make_async_arena (__VA_ARGS__).unwrap

#define ark_scoped_arena_resource(Arena_)                                                                              \
	$static_assert (                                                                                                   \
		$typematch ((Arena_), ark_async_arena*), "a scoped temporary arena resource must be asynchronous"              \
	);                                                                                                                 \
	bool ark_pp_concat (___once__, __LINE__) = true;                                                                   \
	for (__attr_cleanup__ (ark_async_arena_temp_end)                                                                   \
			 ark_temp_async_arena temp_arena_obj = ark_async_arena_temp_begin (Arena_);                                \
		 ark_pp_concat (___once__, __LINE__);                                                                          \
		 ark_pp_concat (___once__, __LINE__) = false)

enum __attr_flag_enum__ : flag_t {
	ARK_ASYNC_ARENA_DEFAULT		  = 0,
	ARK_ASYNC_ARENA_GUARD_PAGES	  = 1 << 0,
	ARK_ASYNC_ARENA_HUGE_PAGES	  = 1 << 1,
	ARK_ASYNC_ARENA_ZERO_ON_RESET = 1 << 2,
};

ARK_CLASS (ark_async_arena);
struct region;

ARK_CLASS (ark_temp_async_arena) {
	ark_async_arena* _arena;
	struct region*	 _region;
	sz				 _used;
};

#define ARK_EXPECT_CONTEXT ARK_ASYNC_ARENA

ark_faultdefs (ARK_ASYNC_ARENA, MALLOC, MUTEX_INIT);

ARKAPI const ark_allocator_vtable ark_async_arena_allocator_vtable;

ARKAPI __decl_overloadable__ __decl_malloc__ __decl_nodiscard__
	ark_expect (ark_async_arena_p) ark_make_async_arena (sz cap_, flag_t flags_) __attr_noexcept__;

__decl_static_inline__ __decl_overloadable__ __decl_malloc__ __decl_nodiscard__
ark_expect (ark_async_arena_p) ark_make_async_arena (u0) __attr_noexcept__ {
	return ark_make_async_arena (0, ARK_ASYNC_ARENA_DEFAULT);
}

__decl_static_inline__ __decl_overloadable__ __decl_malloc__ __decl_nodiscard__
ark_expect (ark_async_arena_p) ark_make_async_arena (sz cap_) __attr_noexcept__ {
	return ark_make_async_arena (cap_, ARK_ASYNC_ARENA_DEFAULT);
}

__decl_static_inline__ __decl_overloadable__ __decl_malloc__ __decl_nodiscard__
ark_expect (ark_async_arena_p) ark_make_async_arena (flag_t flags_) __attr_noexcept__ {
	return ark_make_async_arena (0, flags_);
}

ARKAPI u0 ark_reset_async_arena (ark_async_arena* __noalias__ arena_) __attr_noexcept__;

ARKAPI u0 ark_destroy_async_arena (ark_async_arena* __noalias__ arena_) __attr_noexcept__;

ARKAPI __decl_malloc__ __decl_returns_nonnull__ __decl_nodiscard__ u0* ark_async_arena_alloc (
	ark_async_arena* __noalias__ arena_, sz size_, sz alignment_
) __attr_alloc_params__ (2, 3) __attr_noexcept__;

ARKAPI ark_temp_async_arena ark_async_arena_temp_begin (ark_async_arena* __noalias__ arena_) __attr_noexcept__;

ARKAPI u0 ark_async_arena_temp_end (ark_temp_async_arena* __noalias__ temp_) __attr_noexcept__;

__decl_static_inline__ u0 ark_async_arena_cleanup (ark_async_arena** __noalias__ arena_) {
	ark_destroy_async_arena (*arena_);
}

#undef ARK_EXPECT_CONTEXT

ARK_END

#endif /* ARKLIB_ASYNC_ARENA_H_ */
