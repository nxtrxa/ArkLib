#ifndef ARKLIB_INTERNALS_FEATURES_H_
#define ARKLIB_INTERNALS_FEATURES_H_

#ifdef __linux__
	#define ARKLIB_IS_LINUX 1
	#define ARKLIB_IS_WIN	0
	#define ARKLIB_IS_MAC	0
	#define ARKLIB_IS_BSD	0
#elifdef _WIN32
	#define ARKLIB_IS_WIN	1
	#define ARKLIB_IS_LINUX 0
	#define ARKLIB_IS_MAC	0
	#define ARKLIB_IS_BSD	0
#elifdef __APPLE__
	#define ARKLIB_IS_WIN	0
	#define ARKLIB_IS_LINUX 0
	#define ARKLIB_IS_MAC	1
	#define ARKLIB_IS_BSD	0
	#import <TargetConditionals.h>
#elif defined(BSD) || defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__)
	#define ARKLIB_IS_LINUX 0
	#define ARKLIB_IS_WIN	0
	#define ARKLIB_IS_MAC	0
	#define ARKLIB_IS_BSD	1
#else
	#error "Where are you bro???"
#endif

#ifdef __cplusplus
	#define ARKLIB_IS_CXX 1
#else
	#define ARKLIB_IS_CXX 0
#endif
#ifdef __clang__
	#define ARKLIB_IS_CLANG 1
#else
	#define ARKLIB_IS_CLANG 0
#endif
#if defined(__GNUC__)
	#define ARKLIB_IS_GCCISH 1
#else
	#define ARKLIB_IS_GCCISH 0
#endif
#if defined(_MSC_VER)
	#define ARKLIB_IS_MSVCISH 1
#else
	#define ARKLIB_IS_MSVCISH 0
#endif
#if defined(__GNUC__) && !defined(__clang__)
	#define ARKLIB_IS_GCC 1
#else
	#define ARKLIB_IS_GCC 0
#endif
#if defined(_MSC_VER) && !defined(__clang__)
	#define ARKLIB_IS_MSVC 1
#else
	#define ARKLIB_IS_MSVC 0
#endif

/*
 * `ARKLIB_REL_VIEW` is a user defined macro.  is disable DEBUG_BUILD mode for tools like
 *  `code actions` to see what the code will look like in release mode.
 */
#if defined(DEBUG) && !defined(ARKLIB_REL_VIEW)
	#define ARKLIB_RELEASE_BUILD 0
	#define ARKLIB_DEBUG_BUILD	 1
#else
	#define ARKLIB_RELEASE_BUILD 1
	#define ARKLIB_DEBUG_BUILD	 0
#endif

#ifndef ARKLIB_ENABLED_WARNINGS
	#if ARKLIB_DEBUG_BUILD
		#define ARKLIB_ENABLED_WARNINGS 1
	#else
		#define ARKLIB_ENABLED_WARNINGS 0
	#endif
#endif

#ifndef ARKLIB_ENABLED_INFO
	#if ARKLIB_DEBUG_BUILD
		#define ARKLIB_ENABLED_INFO 1
	#else
		#define ARKLIB_ENABLED_INFO 0
	#endif
#endif

#ifndef ARKLIB_REPORT_ALLOCATIONS
	#if ARKLIB_DEBUG_BUILD
		#define ARKLIB_REPORT_ALLOCATIONS 1
	#else
		#define ARKLIB_REPORT_ALLOCATIONS 0
	#endif
#endif

#if ARKLIB_IS_CXX
	#import <atomic>
	#define ark_atomic_t(Typename_) std::atomic<Typename_>
	#define ark_atomic_inc(_val)	(_val).fetch_add (1, std::memory_order_relaxed)
	#define ARKLIB_BEGIN_EXTERN                                                                                        \
		extern "C" {                                                                                                   \
		#define ARKLIB_END_EXTERN                                                                                      \
		}
#else
	#import <stdatomic.h>
	#define ark_atomic_t _Atomic

	#define ARKLIB_BEGIN_EXTERN
	#define ARKLIB_END_EXTERN
#endif

#define ARKLIB_PRAGMA(Pragma_) _Pragma (#Pragma_)

#if ARKLIB_IS_CLANG
	#define ARKLIB_PRAGMA_PUSH		 ARKLIB_PRAGMA (clang diagnostic push)
	#define ARKLIB_PRAGMA_IGNORE(w_) ARKLIB_PRAGMA (clang diagnostic ignored w_)
	#define ARKLIB_PRAGMA_POP		 ARKLIB_PRAGMA (clang diagnostic pop)
#else
	#define ARKLIB_PRAGMA_PUSH
	#define ARKLIB_PRAGMA_IGNORE(w_)
	#define ARKLIB_PRAGMA_POP
#endif

#define ARK_BEGIN                                                                                                      \
	ARKLIB_PRAGMA_PUSH                                                                                                 \
	ARKLIB_PRAGMA_IGNORE ("-Wgnu-statement-expression-from-macro-expansion")                                           \
	ARKLIB_PRAGMA_IGNORE ("-Wimport-preprocessor-directive-pedantic")                                                  \
	ARKLIB_PRAGMA_IGNORE ("-Wgnu-zero-variadic-macro-arguments")                                                       \
	ARKLIB_PRAGMA_IGNORE ("-Wgnu-flexible-array-initializer")                                                          \
	ARKLIB_PRAGMA_IGNORE ("-Wdollar-in-identifier-extension")                                                          \
	ARKLIB_PRAGMA_IGNORE ("-Wgnu-statement-expression")                                                                \
	ARKLIB_PRAGMA_IGNORE ("-Wnullability-completeness")                                                                \
	ARKLIB_PRAGMA_IGNORE ("-Wgnu-alignof-expression")                                                                  \
	ARKLIB_PRAGMA_IGNORE ("-Wnullability-extension")                                                                   \
	ARKLIB_PRAGMA_IGNORE ("-Wgnu-label-as-value")                                                                      \
	ARKLIB_PRAGMA_IGNORE ("-Wignored-attributes")                                                                      \
	ARKLIB_PRAGMA_IGNORE ("-Wzero-length-array")                                                                       \
	ARKLIB_PRAGMA_IGNORE ("-Wgnu-pointer-arith")                                                                       \
	ARKLIB_PRAGMA_IGNORE ("-Wc2y-extensions")                                                                          \
	ARKLIB_PRAGMA_IGNORE ("-Wgcc-compat")                                                                              \
	ARKLIB_BEGIN_EXTERN

#define ARK_END ARKLIB_END_EXTERN ARKLIB_PRAGMA_POP

#ifndef ARKLIB_HAVE_SSE2
	#if defined(__SSE2__) || (ARKLIB_IS_MSVCISH && (defined(_M_X64) || (defined(_M_IX86) && _M_IX86_FP >= 2)))
		#define ARKLIB_HAVE_SSE2 1
	#else
		#define ARKLIB_HAVE_SSE2 0
	#endif
#endif

#ifndef ARKLIB_HAVE_SSSE3
	#ifdef __SSSE3__
		#define ARKLIB_HAVE_SSSE3 1
	#else
		#define ARKLIB_HAVE_SSSE3 0
	#endif
#endif

#if ARKLIB_HAVE_SSE2
	#import <emmintrin.h>
#endif

#if ARKLIB_HAVE_SSSE3
	#if !ARKLIB_HAVE_SSE2
		#error "Bad configuration: SSSE3 implies SSE2!"
	#endif
	#import <tmmintrin.h>
#endif

#ifdef __has_builtin
	#define ARKLIB_HAS_BUILTIN(X_) __has_builtin (X_)
#else
	#define ARKLIB_HAS_BUILTIN(X_) 0
#endif

#ifdef __has_feature
	#define ARKLIB_HAS_FEATURE(X_) __has_feature (X_)
#else
	#define ARKLIB_HAS_FEATURE(X_) 0
#endif

#ifdef __has_include
	#define ARKLIB_HAS_INCLUDE(X_) __has_include (X_)
#else
	#define ARKLIB_HAS_INCLUDE(X_)
#endif

#if defined(__has_c_attribute)
	#define ARKLIB_HAS_C_ATTRIBUTE(X_) __has_c_attribute (X_)
#else
	#define ARKLIB_HAS_C_ATTRIBUTE(X_) 0
#endif

#if defined(__has_attribute)
	#define ARKLIB_HAS_ATTRIBUTE(X_) __has_attribute (X_)
#else
	#define ARKLIB_HAS_ATTRIBUTE(X_) 0
#endif

#endif /* ARKLIB_INTERNALS_FEATURES_H_ */
