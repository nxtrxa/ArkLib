#ifndef ARKLIB_INTERNALS_ATTRIBUTES_H_
#define ARKLIB_INTERNALS_ATTRIBUTES_H_

/* Legacy GNU/Clang Attribute probe */

#import "__arklib_features_.h"

/* --------------------------------------------------------------------------
 * Declarations / Prefix Modifiers (__decl_*__)
 * Position: Placed BEFORE the function return type or symbol declaration.
 * ----------------------------------------------------------------------- */

#ifndef __decl_always_inline__
	#if ARKLIB_HAS_ATTRIBUTE(__always_inline__)
		#define __decl_always_inline__ __attribute__ ((__always_inline__))
	#elif defined(_MSC_VER)
		#define __decl_always_inline__ __forceinline
	#else
		#define __decl_always_inline__
	#endif
#endif

#ifndef __decl_gnu_inline__
	#if ARKLIB_HAS_ATTRIBUTE(__gnu_inline__)
		#define __decl_gnu_inline__ __attribute__ ((__gnu_inline__))
	#else
		#define __decl_gnu_inline__
	#endif
#endif

#ifndef __decl_noinline__
	#if ARKLIB_HAS_ATTRIBUTE(__noinline__)
		#define __decl_noinline__ __attribute__ ((__noinline__))
	#elif defined(_MSC_VER)
		#define __decl_noinline__ __declspec (noinline)
	#else
		#define __decl_noinline__
	#endif
#endif

#ifndef __decl_hot__
	#if ARKLIB_HAS_ATTRIBUTE(__hot__)
		#define __decl_hot__ __attribute__ ((__hot__))
	#else
		#define __decl_hot__
	#endif
#endif

#ifndef __decl_cold__
	#if ARKLIB_HAS_ATTRIBUTE(__cold__)
		#define __decl_cold__ __attribute__ ((__cold__))
	#else
		#define __decl_cold__
	#endif
#endif

#ifndef __decl_optnone__
	#if ARKLIB_HAS_ATTRIBUTE(__optnone__)
		#define __decl_optnone__ __attribute__ ((__optnone__))
	#else
		#define __decl_optnone__
	#endif
#endif

#ifndef __decl_sentinel__
	#if ARKLIB_HAS_ATTRIBUTE(__sentinel__)
		#define __decl_sentinel__ __attribute__ ((__sentinel__))
	#else
		#define __decl_sentinel__
	#endif
#endif

#ifndef __decl_constructor__
	#if ARKLIB_HAS_ATTRIBUTE(__constructor__)
		#define __decl_constructor__(P_) __attribute__ ((__constructor__ (P_)))
	#else
		#define __decl_constructor__(P_)
	#endif
#endif

#ifndef __decl_destructor__
	#if ARKLIB_HAS_ATTRIBUTE(__destructor__)
		#define __decl_destructor__(P_) __attribute__ ((__destructor__ (P_)))
	#else
		#define __decl_destructor__(P_)
	#endif
#endif

#ifndef __decl_overloadable__
	#if ARKLIB_HAS_ATTRIBUTE(__overloadable__) && !defined(__cplusplus)
		#define __decl_overloadable__ __attribute__ ((overloadable))
	#else
		#define __decl_overloadable__
	#endif
#endif

#ifndef __decl_nodiscard__
	#if ARKLIB_HAS_ATTRIBUTE(warn_unused_result)
		#define __decl_nodiscard__ __attribute__ ((warn_unused_result))
	#elif defined(_MSC_VER) && defined(_SAL_ENABLED)
		#define __decl_nodiscard__ _Check_return_
	#else
		#define __decl_nodiscard__
	#endif
#endif

#ifndef __decl_noreturn__
	#if ARKLIB_HAS_ATTRIBUTE(__noreturn__)
		#define __decl_noreturn__ __attribute__ ((__noreturn__))
	#elif defined(_MSC_VER)
		#define __decl_noreturn__ __declspec (noreturn)
	#else
		#define __decl_noreturn__
	#endif
#endif

#ifndef __decl_deprecated__
	#if ARKLIB_HAS_ATTRIBUTE(__deprecated__)
		#define __decl_deprecated__(...) __attribute__ ((__deprecated__ (__VA_ARGS__)))
	#elif defined(_MSC_VER)
		#define __decl_deprecated__(...) __declspec (deprecated)
	#else
		#define __decl_deprecated__(...)
	#endif
#endif

#ifndef __decl_unused__
	#if ARKLIB_HAS_ATTRIBUTE(__unused__)
		#define __decl_unused__ __attribute__ ((__unused__))
	#else
		#define __decl_unused__
	#endif
#endif

#ifndef __decl_malloc__
	#if ARKLIB_HAS_ATTRIBUTE(__malloc__)
		#define __decl_malloc__ __attribute__ ((__malloc__))
	#else
		#define __decl_malloc__
	#endif
#endif

#ifndef __decl_returns_nonnull__
	#if ARKLIB_HAS_ATTRIBUTE(__returns_nonnull__)
		#define __decl_returns_nonnull__ __attribute__ ((__returns_nonnull__))
	#else
		#define __decl_returns_nonnull__
	#endif
#endif

#ifndef __decl_packed__
	#if ARKLIB_HAS_ATTRIBUTE(__packed__)
		#define __decl_packed__ __attribute__ ((__packed__))
	#else
		#define __decl_packed__
	#endif
#endif

#ifndef __decl_aligned__
	#if ARKLIB_HAS_ATTRIBUTE(__aligned__)
		#define __decl_aligned__(Align_) __attribute__ ((__aligned__ (Align_)))
	#elif defined(_MSC_VER)
		#define __decl_aligned__(Align_) __declspec (align (Align_))
	#else
		#define __decl_aligned__(Align_)
	#endif
#endif

#ifndef __decl_const__
	#if ARKLIB_HAS_ATTRIBUTE(__const__)
		#define __decl_const__ __attribute__ ((__const__))
	#else
		#define __decl_const__
	#endif
#endif

#ifndef __decl_pure__
	#if ARKLIB_HAS_ATTRIBUTE(__pure__)
		#define __decl_pure__ __attribute__ ((__pure__))
	#else
		#define __decl_pure__
	#endif
#endif

#ifndef __decl_artificial__
	#if ARKLIB_HAS_ATTRIBUTE(__artificial__)
		#define __decl_artificial__ __attribute__ ((__artificial__))
	#else
		#define __decl_artificial__
	#endif
#endif

#ifndef __decl_returns_twice__
	#if ARKLIB_HAS_ATTRIBUTE(__returns_twice__)
		#define __decl_returns_twice__ __attribute__ ((__returns_twice__))
	#else
		#define __decl_returns_twice__ ((__returns_twice__))
	#endif
#endif


#ifndef __decl_flatten__
	#if ARKLIB_HAS_ATTRIBUTE(__flatten__)
		#define __decl_flatten__ __attribute__ ((__flatten__))
	#else
		#define __decl_flatten__
	#endif
#endif

#ifndef __decl_visibility__
	#if ARKLIB_HAS_ATTRIBUTE(__visibility__)
		#define __decl_visibility__(Mode_) __attribute__ ((__visibility__ (Mode_)))
	#else
		#define __decl_visibility__(Mode_)
	#endif
#endif

#ifndef __decl_weak_alias__
	#if ARKLIB_HAS_ATTRIBUTE(__weak__)
		#define __decl_weak_alias__(Target_) __attribute__ (__weak__, __alias__ (#Target_))
	#else
		#define __decl_weak_alias__(Target_)
	#endif
#endif

#ifndef __decl_internal_symbol__
	#define __decl_internal_symbol__ __decl_visibility__ ("internal")
#endif

#ifndef __decl_hidden_symbol__
	#define __decl_hidden_symbol__ __decl_visibility__ ("hidden")
#endif

#ifndef __decl_protected_symbol__
	#define __decl_protected_symbol__ __decl_visibility__ ("protected")
#endif

#ifndef __decl_public_symbol__
	#define __decl_public_symbol__ __decl_visibility__ ("default")
#endif

/* Inline Storage Combinations */
#ifndef __decl_inline__
	#define __decl_inline__ __decl_always_inline__ inline
#endif

#ifndef __decl_static_inline__
	#define __decl_static_inline__ __decl_inline__ static
#endif

#ifndef __decl_extern_inline__
	#define __decl_extern_inline__ __decl_gnu_inline__ __decl_inline__ extern
#endif

/* -----------------------------------------------------------------------------------
 * Attributes / Suffix & Parameter Modifiers (__attr_*__)
 * Position: Placed AFTER the function prototype/parameters or on types/parameters.
 * -------------------------------------------------------------------------------- */

#ifndef __attr_noexcept__
	#ifdef __cplusplus
		#if (__cplusplus >= 201103L) || (_MSC_VER > 1900)
			#define __attr_noexcept__ noexcept
		#else
			#define __attr_noexcept__ throw ()
		#endif
	#else
		#if ARKLIB_HAS_ATTRIBUTE(__nothrow__)
			#define __attr_noexcept__ __attribute__ ((__nothrow__))
		#else
			#define __attr_noexcept__
		#endif
	#endif
#endif

#ifndef __attr_cleanup__
	#if ARKLIB_HAS_ATTRIBUTE(__cleanup__)
		#define __attr_cleanup__(Fn_) __attribute__ ((__cleanup__ (Fn_)))
	#else
		#define __attr_cleanup__(Fn_)
	#endif
#endif

#ifndef __attr_nonnull__
	#if ARKLIB_HAS_ATTRIBUTE(__nonnull__)
		#define __attr_nonnull__(...) __attribute__ ((__nonnull__ (__VA_OPT__ () __VA_ARGS__)))
	#else
		#define __attr_nonnull__(...)
	#endif
#endif

#ifndef __attr_format__
	#if ARKLIB_HAS_ATTRIBUTE(__format__)
		#define __attr_format__(arch, ...) __attribute__ ((__format__ (arch, __VA_ARGS__)))
	#else
		#define __attr_format__(arch, ...)
	#endif
#endif

#ifndef __attr_ownership__
	#if ARKLIB_HAS_ATTRIBUTE(__ownership_takes__) && ARKLIB_HAS_ATTRIBUTE(__ownership_returns__)
		#define __attr_ownership__(action, allocator, ...)                                                             \
			__attribute__ ((__ownership_##action##__ (allocator __VA_OPT__ (, ) __VA_ARGS__)))
	#else
		#define __attr_ownership__(action, allocator, ...)
	#endif
#endif

#ifndef __attr_alloc_size__
	#if ARKLIB_HAS_ATTRIBUTE(__alloc_size__)
		#define __attr_alloc_size__(idx) __attribute__ ((__alloc_size__ (idx)))
	#else
		#define __attr_alloc_size__(idx)
	#endif
#endif

#ifndef __attr_alloc_align__
	#if ARKLIB_HAS_ATTRIBUTE(__alloc_align__)
		#define __attr_alloc_align__(idx) __attribute__ ((__alloc_align__ (idx)))
	#else
		#define __attr_alloc_align__(idx)
	#endif
#endif

#ifndef __attr_alloc_params__
	#define __attr_alloc_params__(sidx, aidx) __attr_alloc_size__ (sidx) __attr_alloc_align__ (aidx)
#endif

#ifndef __decl_realloc__
	#if ARKLIB_HAS_ATTRIBUTE(__ownership_takes__) && ARKLIB_HAS_ATTRIBUTE(__ownership_returns__)
		#define __decl_realloc__(F, Idx, ...)                                                                          \
			__attribute__ ((__ownership_takes__ (F, Idx)))                                                             \
			__attribute__ ((__ownership_returns__ (F __VA_OPT__ (, ) __VA_ARGS__)))
	#else
		#define __decl_realloc__(F, Idx, ...)
	#endif
#endif

#ifndef __attr_enable_if__
	#if ARKLIB_HAS_ATTRIBUTE(__enable_if__)
		#define __attr_enable_if__(cond, msg) __attribute__ ((__enable_if__ (cond, msg)))
	#else
		#define __attr_enable_if__(cond, msg)
	#endif
#endif

#ifndef __attr_alias__
	#if ARKLIB_HAS_ATTRIBUTE(__alias__)
		#define __attr_alias__(target) __attribute__ ((__alias__ (target)))
	#else
		#define __attr_alias__(target)
	#endif
#endif

#ifndef __attr_blocks_byref__
	#if ARKLIB_HAS_ATTRIBUTE(__blocks__)
		#define __attr_blocks_byref__ __attribute__ ((__blocks__ (byref)))
	#else
		#define __attr_blocks_byref__
	#endif
#endif

#ifndef __attr_capture__
	#define __attr_capture__ __attr_blocks_byref__
#endif

#ifndef __attr_nullable__
	#ifdef __clang__
		#define __attr_nullable__ _Nullable
	#else
		#define __attr_nullable__
	#endif
#endif

#ifndef __attr_ext_vec_type__
	#if ARKLIB_HAS_ATTRIBUTE(__ext_vector_type__)
		#define __attr_ext_vec_type__(n) __attribute__ ((__ext_vector_type__ (n)))
	#else
		#define __attr_ext_vec_type__(n)
	#endif
#endif

#ifndef __attr_vector_size__
	#if ARKLIB_HAS_ATTRIBUTE(__vector_size__)
		#define __attr_vector_size__(n) __attribute__ ((__vector_size__ (n)))
	#else
		#define __attr_vector_size__(n)
	#endif
#endif

#ifndef __attr_counted_by__
	#if ARKLIB_HAS_ATTRIBUTE(__counted_by__)
		#define __attr_counted_by__(count_field) __attribute__ ((__counted_by__ (count_field)))
	#else
		#define __attr_counted_by__(count_field)
	#endif
#endif

#ifndef __attr_pass_objsz__
	#if ARKLIB_HAS_ATTRIBUTE(__pass_object_size__)
		#define __attr_pass_objsz__(Type_) __attribute__ ((__pass_object_size__ (Type_)))
	#else
		#define __attr_pass_objsz__()
	#endif
#endif

#ifndef __attr_pass_dyn_objsz__
	#if ARKLIB_HAS_ATTRIBUTE(__pass_dynamic_object_size__)
		#define __attr_pass_dyn_objsz__(Type_) __attribute__ ((__pass_dynamic_object_size__ (Type_)))
	#else
		#define __attr_pass_dyn_objsz__()
	#endif
#endif

#ifndef __attr_access__
	#if ARKLIB_HAS_ATTRIBUTE(__access__)
		#define __attr_access__(...) __attribute__ ((__access__ (__VA_ARGS__)))
	#else
		#define __attr_access__(...)
	#endif
#endif

#ifndef __attr_enum_extensibility__
	#if ARKLIB_HAS_ATTRIBUTE(__enum_extensibility__)
		#define __attr_enum_extensibility__(Mode_, ...)                                                                \
			__attribute__ ((__enum_extensibility__ (Mode_) __VA_OPT__ (, ) __VA_ARGS__))
	#else
		#define __attr_enum_extensibility__(Mode_, ...)
	#endif
#endif

#ifndef __attr_transparent_union__
	#if ARKLIB_HAS_ATTRIBUTE(__transparent_union__)
		#define __attr_transparent_union__ __attribute__ ((__transparent_union__))
	#else
		#define __attr_transparent_union__
	#endif
#endif

#ifndef __attr_flag_enum__
	#if ARKLIB_HAS_ATTRIBUTE(__flag_enum__)
		#define __attr_flag_enum__ __attribute__ ((__flag_enum__))
	#else
		#define __attr_flag_enum__
	#endif
#endif

#ifndef __attr_nonstring__
	#if ARKLIB_HAS_ATTRIBUTE(__nonstring__)
		#define __attr_nonstring__ __attribute__ ((__nonstring__))
	#else
		#define __attr_nonstring__
	#endif
#endif

#ifndef __attr_diagnose_if__
	#if ARKLIB_HAS_ATTRIBUTE(__diagnose_if__)
		#define __attr_diagnose_if__(Cond_, Msg_, Type_) __attribute__ ((__diagnose_if__ ((Cond_), (Msg_), (Type_))))
	#else
		#define __attr_diagnose_if__(Cond_, Msg_, Type_)
	#endif
#endif

#ifndef __attr_capability__
	#if ARKLIB_HAS_ATTRIBUTE(__capability__)
		#define __attr_capability__(Obj_) __attribute__ ((__capability__ (Obj_)))
	#else
		#define __attr_capability__
	#endif
#endif

#ifndef __attr_acquire_capability__
	#if ARKLIB_HAS_ATTRIBUTE(__acquire_capability__)
		#define __attr_acquire_capability__(Obj_) __attribute__ ((__acquire_capability__ (Obj_)))
	#else
		#define __attr_acquire_capability__
	#endif
#endif

#ifndef __attr_release_capability__
	#if ARKLIB_HAS_ATTRIBUTE(__release_capability__)
		#define __attr_release_capability__(Obj_) __attribute__ ((__release_capability__ (Obj_)))
	#else
		#define __attr_release_capability__
	#endif
#endif

/* Function storeage class specifiers */
#define ARKAPI __decl_public_symbol__ extern

#define ARKAPI_INLINE __decl_public_symbol__ __decl_gnu_inline__ extern inline

#define ARKAPI_ALWAYS_INLINE __decl_public_symbol__ __decl_gnu_inline__ __decl_always_inline__ extern

#define ARKEXPORT __decl_public_symbol__

#define ARKINTERNAL __decl_internal_symbol__ static

#endif /* ARKLIB_INTERNALS_ATTRIBUTES_H_ */
