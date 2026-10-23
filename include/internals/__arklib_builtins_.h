#ifndef ARKLIB_INTERNALS_BUILTINS_H_
#define ARKLIB_INTERNALS_BUILTINS_H_

#import "__arklib_features_.h"

ARK_BEGIN

#ifdef ARKLIB_NAMESPACE
	#define LIKELY				ARK_LIKELY
	#define UNLIKELY			ARK_UNLIKELY
	#define PREFETCH			ARK_PREFETCH
	#define UNREACHABLE			ARK_UNREACHABLE
	#define ASSUME_ALIGNED		ARK_ASSUME_ALIGNED
	#define ASSUME				ARK_ASSUME
	#define CHOOSE_EXPR			ARK_CHOOSE_EXPR
	#define TYPES_COMPATIBLE_P	ARK_TYPES_COMPATIBLE_P
	#define CLASSIFY_TYPE		ARK_CLASSIFY_TYPE
	#define IS_CONSTEXPR		ARK_IS_CONSTEXPR
	#define OBJECT_SIZE			ARK_OBJECT_SIZE
	#define DYNAMIC_OBJECT_SIZE ARK_DYNAMIC_OBJECT_SIZE
	#define FILE_PATH			ARK_FPATH
	#define FILE_NAME			ARK_FILE
#endif

/* Branch Hints & Optimization Intrinsics */
#if ARKLIB_HAS_BUILTIN(__builtin_expect) || ARKLIB_IS_GCC
	#define ARK_LIKELY(Cond_)	(__builtin_expect (!!(Cond_), true))
	#define ARK_UNLIKELY(Cond_) (__builtin_expect (!!(Cond_), false))
#else
	#define ARK_LIKELY(Cond_)	(Cond_)
	#define ARK_UNLIKELY(Cond_) (Cond_)
#endif

#if ARKLIB_HAS_BUILTIN(__builtin_prefetch) || ARKLIB_IS_GCC
	#define ARKLIB_PREFETCH(Addr_, Locality_) __builtin_prefetch (Addr_, 0, Locality_)
#else
	#error "__builtin_prefetch not available."
#endif

#if ARKLIB_HAS_BUILTIN(__builtin_unreachable)
	#define ARK_UNREACHABLE() __builtin_unreachable ()
#else
	#define ARK_UNREACHABLE() ARK_PANIC ("Unreachable statement reached")
#endif

#if ARKLIB_HAS_BUILTIN(__builtin_trap)
	#define ARK_TRAP() __builtin_trap ()
#else
	#define ARK_TRAP() ARK_PANIC ("trap triggered")
#endif

#if ARKLIB_HAS_BUILTIN(__builtin_assume_aligned)
	#define ARK_ASSUME_ALIGNED __builtin_assume_aligned
#else
	#define ARK_ASSUME_ALIGNED(...) __VA_ARGS__
#endif

#if ARKLIB_HAS_BUILTIN(__builtin_assume)
	#define ARK_ASSUME __builtin_assume
#else
	#define ARK_ASSUME(Cond_)
#endif

/* Expressions & Builtin Trait Abstractions */
#if ARKLIB_HAS_BUILTIN(__builtin_choose_expr)
	#define ARK_CHOOSE_EXPR(Cond_, Then_, Else_) (__builtin_choose_expr (Cond_, Then_, Else_))
#else
	#define ARK_CHOOSE_EXPR(Cond_, Then_, Else_) ((Cond_) ? (Then_) : (Else_))
#endif

#if ARKLIB_HAS_BUILTIN(__builtin_types_compatible_p)
	#define ARK_TYPES_COMPATIBLE_P(T_, U_) __builtin_types_compatible_p (T_, U_)
#else
	#define ARK_TYPES_COMPATIBLE_P(T_, U_) _Generic ((typeof_unqual (U_) {}), T_: 1, default: 0)
#endif

#if ARKLIB_HAS_BUILTIN(__builtin_classify_type)
	#define ARK_CLASSIFY_TYPE(Type_) __builtin_classify_type (Type_)
#else
	#error "__builtin_classify_type not available."
#endif

#if ARKLIB_HAS_BUILTIN(__builtin_bit_cast)
	#define ARK_BIT_CAST __builtin_bit_cast
#else
	#define ARK_BIT_CAST(Type_, Expr_) *(Type_*)(&(Expr_)) // And hope for the best :)
#endif

#if ARKLIB_HAS_BUILTIN(__builtin_constant_p)
	#define ARK_IS_CONSTEXPR(X_) __builtin_constant_p (X_)
#else
	#define ARK_IS_CONSTEXPR(X_) 0
#endif

#if ARKLIB_HAS_BUILTIN(__builtin_object_size)
	#define ARK_OBJECT_SIZE(Ptr_, T_) __builtin_object_size (Ptr_, T_)
#else
	#define ARK_OBJECT_SIZE(Ptr_, T_)
#endif

#if ARKLIB_HAS_BUILTIN(__builtin_dynamic_object_size)
	#define ARK_DYNAMIC_OBJECT_SIZE(Ptr_, T_) __builtin_object_size (Ptr_, T_)
#else
	#define ARK_DYNAMIC_OBJECT_SIZE(Ptr_, T_)
#endif

#if ARKLIB_HAS_BUILTIN(__builtin_clzll)
	#define ARK_CLZLL(X_) __builtin_clzll (X_)
#else
	#define ARK_CLZLL(X_)
#endif


/* Source Location Intrinsics */
#if ARKLIB_HAS_BUILTIN(__builtin_FILE)
	#define $file_path __builtin_FILE ()
#else
	#define $file_path __FILE__
#endif

#define $line	  __LINE__
#define $file	  __FILE_NAME__
#define $fnsig	  __extension__ __PRETTY_FUNCTION__
#define $function __builtin_FUNCTION ()

ARK_END

#endif /* ARKLIB_INTERNALS_BUILTINS_H_ */
