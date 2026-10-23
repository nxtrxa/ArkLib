#ifndef ARK_INTERNALS_CRT_H_
#define ARK_INTERNALS_CRT_H_

#import "__arklib_features_.h"

ARK_BEGIN

#ifdef ARK_NAMESPACE
	#define MALLOC		  ARK_MALLOC
	#define ALIGNED_ALLOC ARK_ALIGNED_ALLOC
	#define CALLOC		  ARK_CALLOC
	#define FREE		  ARK_FREE
	#define REALLOC		  ARK_REALLOC
	#define MEMSET		  ARK_MEMSET
	#define MEMCPY		  ARK_MEMCPY
	#define MEMCMP		  ARK_MEMCMP
	#define MEMMOVE		  ARK_MEMMOVE
	#define MEMCHR		  ARK_MEMCHR
	#define STRLEN		  ARK_STRLEN
	#define STRCMP		  ARK_STRCMP
	#define STRNCMP		  ARK_STRNCMP
	#define STRRCHR		  ARK_STRRCHR
#endif

#ifndef ARK_MALLOC
	#if ARKLIB_HAS_BUILTIN(__builtin_malloc)
		#define ARK_MALLOC __builtin_malloc
	#else
		#define ARK_MALLOC malloc
	#endif
#endif

#ifndef ARK_ALIGNED_ALLOC
	#define ARK_ALIGNED_ALLOC aligned_alloc
#endif

#ifndef ARK_CALLOC
	#if ARKLIB_HAS_BUILTIN(__builtin_calloc)
		#define ARK_CALLOC __builtin_calloc
	#else
		#define ARK_CALLOC calloc
	#endif
#endif

#ifndef ARK_FREE
	#if ARKLIB_HAS_BUILTIN(__builtin_free)
		#define ARK_FREE __builtin_free
	#else
		#define ARK_FREE free
	#endif
#endif

#ifndef ARK_REALLOC
	#if ARKLIB_HAS_BUILTIN(__builtin_realloc)
		#define ARK_REALLOC __builtin_realloc
	#else
		#define ARK_REALLOC realloc
	#endif
#endif

#ifndef ARK_MEMSET
	#if ARKLIB_HAS_BUILTIN(__builtin_memset)
		#define ARK_MEMSET __builtin_memset
	#else
		#define ARK_MEMSET memset
	#endif
#endif

#ifndef ARK_MEMCPY
	#if ARKLIB_HAS_BUILTIN(__builtin_memcpy)
		#define ARK_MEMCPY __builtin_memcpy
	#else
		#define ARK_MEMCPY memcpy
	#endif
#endif

#ifndef ARK_MEMCMP
	#if ARKLIB_HAS_BUILTIN(__builtin_memcmp)
		#define ARK_MEMCMP __builtin_memcmp
	#else
		#define ARK_MEMCMP memcmp
	#endif
#endif

#ifndef ARK_MEMMOVE
	#if ARKLIB_HAS_BUILTIN(__builtin_memmove)
		#define ARK_MEMMOVE __builtin_memmove
	#else
		#define ARK_MEMMOVE memmove
	#endif
#endif

#ifndef ARK_MEMCHR
	#if ARKLIB_HAS_BUILTIN(__builtin_memchr)
		#define ARK_MEMCHR __builtin_memchr
	#else
		#define ARK_MEMCHR memchr
	#endif
#endif

#ifndef ARK_STRLEN
	#if ARKLIB_HAS_BUILTIN(__builtin_strlen)
		#define ARK_STRLEN (__builtin_strlen)
	#else
		#define ARK_STRLEN strlen
	#endif
#endif

#ifndef ARK_STRCMP
	#if ARKLIB_HAS_BUILTIN(__builtin_strcmp)
		#define ARK_STRCMP __builtin_strcmp
	#else
		#define ARK_STRCMP strcmp
	#endif
#endif

#ifndef ARK_STRNCMP
	#if ARKLIB_HAS_BUILTIN(__builtin_strncmp)
		#define ARK_STRNCMP __builtin_strncmp
	#else
		#define ARK_STRNCMP strncmp
	#endif
#endif

#ifndef ARK_STRRCHR
	#if ARKLIB_HAS_BUILTIN(__builtin_strrchr)
		#define ARK_STRRCHR __builtin_strrchr
	#else
		#define ARK_STRRCHR strrchr
	#endif
#endif

ARK_END

#endif /* ARK_INTERNALS_CRT_H_ */
