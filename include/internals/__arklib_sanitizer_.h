#ifndef ARK_INTERNALS_SANITIZER_H_
#define ARK_INTERNALS_SANITIZER_H_

#import "__arklib_attributes_.h"
#import "__arklib_builtins_.h"
#import "__arklib_types_.h"

ARK_BEGIN

/* Module Un-prefixed Aliases */
#ifdef ARK_NAMESPACE
	#define asan_poison_mem	  ark_asan_poison_mem
	#define asan_unpoison_mem ark_asan_unpoison_mem
	#define get_objsz		  ark_get_objsz
	#define get_dynamic_objsz ark_get_dynamic_objsz
#endif

/* Sanitizer Configuration Detection */
#if ARKLIB_HAS_FEATURE(address_sanitizer) || defined(__SANITIZE_ADDRESS__)
	#define ARK_ASAN_ENABLE 1
#else
	#define ARK_ASAN_ENABLE 0
#endif

#if (ARKLIB_HAS_FEATURE(memory_sanitizer) && !defined(__native_client__)) || defined(__SANITIZE_MEMORY__)
	#define ARK_MSAN_ENABLE 1
#else
	#define ARK_MSAN_ENABLE 0
#endif

#if ARK_ASAN_ENABLE
	#import <sanitizer/asan_interface.h>
#endif

#if ARK_MSAN_ENABLE
	#import <sanitizer/msan_interface.h>
#endif

/* Pointer Qualifiers/Bound Annotations */
#undef __nonnull__
#define __nonnull__ _Nonnull

#undef __nullable__
#define __nullable__ _Nullable

#undef __noalias__
#define __noalias__ __restrict__ __nonnull__

#undef __obj_size__
#define __obj_size__(T_) __attr_pass_objsz__ (T_) const

#undef __dyn_obj_size__
#define __dyn_obj_size__(T_) __attr_pass_dyn_objsz__ (T_) const

/* Dynamic Object Size Retrieval */
#define ark_get_objsz(Obj_, Type_)                                                                                     \
	({                                                                                                                 \
		auto ___obj_sz__ = ARK_OBJECT_SIZE (Obj_, Type_);                                                              \
		if (___obj_sz__ == (size_t)-1) ARK_PANIC ("failed to get dynamic object size.");                               \
		___obj_sz__;                                                                                                   \
	})

#define ark_get_dynamic_objsz(Obj_, Type_)                                                                             \
	({                                                                                                                 \
		sz ___obj_sz__ = ARK_DYNAMIC_OBJECT_SIZE (Obj_, Type_);                                                        \
		(___obj_sz__ != ((size_t)-1))                                                                                  \
			? ___obj_sz__                                                                                              \
			: (sz)(ARK_WARN ("failed to get dynamic object size: %zu", (size_t)___obj_sz__));                          \
	})


/* Memory Poisoning Inline Helpers */
__decl_static_inline__ u0 ark_asan_poison_mem (__decl_unused__ U0* __noalias__ mem_, __decl_unused__ sz sz_) {
#if ARK_ASAN_ENABLE
	ASAN_POISON_MEMORY_REGION (mem_, sz_);
#endif
#if ARK_MSAN_ENABLE
	__msan_poison (mem_, sz_);
#endif
}

__decl_static_inline__ u0 ark_asan_unpoison_mem (__decl_unused__ U0* __noalias__ mem_, __decl_unused__ sz sz_) {
#if ARK_ASAN_ENABLE
	ASAN_UNPOISON_MEMORY_REGION (mem_, sz_);
#endif
#if ARK_MSAN_ENABLE
	__msan_unpoison (mem_, sz_);
#endif
}

ARK_END

#endif /* ARK_INTERNALS_SANITIZER_H_ */
