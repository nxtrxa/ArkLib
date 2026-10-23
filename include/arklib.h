/*
 * @Copyright (c) 2026 Notrya A. Avlis. All Rights Reserved.
 *
 *                          ARKLIB
 *
 *  ARKLIB is a feature test project. The goal is to use and explore
 *  new features of the C language.
 *
 *  Portability is planned but won't be added any time soon ;)
 *
 */
#ifndef ARKLIB_H_
#define ARKLIB_H_

#if !defined(__clang__) || __STDC_VERSION__ < 202311L
	#error                                                                                                             \
		"Any project that uses arklib must be compiled with clang,\n" "with -stc=c2y -fblocks\n"                       \
        "e.g. clang -std=c2y -fblocks -larklib  main.c -o main\n"                                                      \
        "for non apple environments linking with -lBlocksRuntime is required"
#endif

#define ARKLIB_VERSION "0.0.1"

#if !defined(ARKLIB_LEAN)
	#define IMPORT_ARKLIB
#endif

#import "__arklib_attributes_.h"
#import "__arklib_pputils_.h"

#if defined(__linux__)
	#define ARKLIB_OS_DIR linux
#elif defined(_WIN32)
	#define ARKLIB_OS_DIR windows
#elif defined(__APPLE__)
	#define ARKLIB_OS_DIR macos
#elif defined(__FreeBSD__)
	#define ARKLIB_OS_DIR freebsd
#else
	#error "unsupported OS"
#endif


#ifdef IMPORT_ARKLIB
	#import "allocators/arkarena_allocator.h"
	#import "allocators/arklibc_allocator.h"
	#import "allocators/arkmonotonic_buffer.h"
	#import "allocators/arkpool.h"

	/*
	 *
	 */
	#import "core/arkassoc_enum.h"
	#import "core/arkbstring.h"
	#import "core/arkexpect.h"
	#import "core/arkistring_stream.h"
	#import "core/arkiwstring_stream.h"
	#import "core/arkkron.h"
	#import "core/arkosstring.h"
	#import "core/arkosstring_view.h"
	#import "core/arkostring_stream.h"
	#import "core/arkowstring_stream.h"
	#import "core/arkstring.h"
	#import "core/arkstring_stream.h"
	#import "core/arkstring_view.h"
	#import "core/arkthread.h"
	#import "core/arkwstring.h"
	#import "core/arkwstring_stream.h"
	#import "core/arkwstring_view.h"
	#import "core/arkzstring.h"

	/*
	 *
	 */
	#import "collections/arkarray.h"
	#import "collections/arkdarray.h"
	#import "collections/arkiterator.h"
	#import "collections/arklinked_list.h"
	#import "collections/arkslice.h"
	#import "collections/arktrie.h"

	/*
	 *
	 */
	#import "math/arkpair.h"
	#import "math/arkvector.h"

	/*
	 *
	 */
	#import "io/arkfilesystem.h"
	#import "io/arkfstream.h"

#else

	#if defined(IMPORT_ARKALLOCATORS)
		#import "allocators/arkarena.h"
		#import "allocators/arklibc_allocator.h"
		#import "allocators/arkmonotonic_buffer.h"
		#import "allocators/arkpool.h"

	#else

		#if defined(IMPORT_ARKARENA)
			#import "allocators/arkarena.h"
		#endif

		#if defined(IMPORT_ARKLIBC_ALLOCATOR)
			#import "allocators/arklibc_allocator.h"
		#endif

		#if defined(IMPORT_ARKMONOTONIC_BUFFER)
			#import "allocators/arkmonotonic_buffer.h"
		#endif

		#if defined(IMPORT_ARKPOOL)
			#import "allocators/arkpool.h"
		#endif

	#endif

	#if defined(IMPORT_ARKCORE)
		#import "core/arkassoc_enum.h"
		#import "core/arkbstring.h"
		#import "core/arkexpect.h"
		#import "core/arkistring_stream.h"
		#import "core/arkiwstring_stream.h"
		#import "core/arkkron.h"
		#import "core/arkosstring.h"
		#import "core/arkosstring_view.h"
		#import "core/arkostring_stream.h"
		#import "core/arkowstring_stream.h"
		#import "core/arkstring.h"
		#import "core/arkstring_stream.h"
		#import "core/arkstring_view.h"
		#import "core/arkthread.h"
		#import "core/arkwstring.h"
		#import "core/arkwstring_stream.h"
		#import "core/arkwstring_view.h"
		#import "core/arkzstring.h"
	#else

		#if defined(IMPORT_ARKEXPECTED)
			#import "core/arkexpected.h"
		#endif

		#if defined(IMPORT_ARKKRON)
			#import "core/arkkron.h"
		#endif
		#if defined(IMPORT_ARKSTRING)
			#import "core/arkstring.h"
		#endif

	#endif

	#if defined(IMPORT_ARKCOLLECTIONS)
		#import "collections/arkarray.h"
		#import "collections/arkdarray.h"
		#import "collections/arkiterator.h"
		#import "collections/arklinked_list.h"
		#import "collections/arkslice.h"
		#import "collections/arktrie.h"

	#else

		#if defined(IMPORT_ARKARRAY)
			#import "collections/arkarray.h"
		#endif

		#if defined(IMPORT_ARKDARRAY)
			#import "collections/arkdarray.h"
		#endif

		#if defined(IMPORT_ARKITERATOR)
			#import "collections/arkdarray.h"
		#endif

		#if defined(IMPORT_ARKLINKED_LIST)
			#import "collections/arklinked_list.h"
		#endif

		#if defined(IMPORT_ARKTRIE)
			#import "collections/arktrie.h"
		#endif

	#endif

	#if defined(IMPORT_ARKMATH)
		#import "math/arkpair.h"
		#import "math/arkvector.h"

	#else

		#if defined(IMPORT_ARKPAIR)
			#import "math/arkpair.h"
		#endif

		#if defined(IMPORT_ARKVECTOR)
			#import "math/arkvector.h"
		#endif

	#endif
	#if defined(IMPORT_ARKIO)
		#import "io/arkfilesystem.h"
		#import "io/arkfstream.h"

	#else

		#if defined(IMPORT_ARKFILESYSTEM)
			#import "io/arkfilesystem.h"
		#endif

		#if defined(IMPORT_ARKFSTREAM)
			#import "io/arkfstream.h"
		#endif
	#endif

	#if defined(IMPORT_ARKALLOCATORS)
		#import "allocators/arkarena.h"
		#import "allocators/arklibc_allocator.h"
		#import "allocators/arkmonotonic_buffer.h"
		#import "allocators/arkpool.h"

	#else

		#if defined(IMPORT_ARKARENA)
			#import "allocators/arkarena.h"
		#endif

		#if defined(IMPORT_ARKLIBC_ALLOCATOR)
			#import "allocators/arklibc_allocator.h"
		#endif

		#if defined(IMPORT_ARKMONOTONIC_BUFFER)
			#import "allocators/arkmonotonic_buffer.h"
		#endif

		#if defined(IMPORT_ARKPOOL)
			#import "allocators/arkpool.h"
		#endif

	#endif

	#if defined(IMPORT_ARKCORE)
		#import "core/arkexpected.h"
		#import "core/arkkron.h"
		#import "core/arkstring.h"

	#else

		#if defined(IMPORT_ARKEXPECTED)
			#import "core/arkexpected.h"
		#endif

		#if defined(IMPORT_ARKKRON)
			#import "core/arkkron.h"
		#endif
		#if defined(IMPORT_ARKSTRING)
			#import "core/arkstring.h"
		#endif

	#endif

	#if defined(IMPORT_ARKCOLLECTIONS)
		#import "collections/arkarray.h"
		#import "collections/arkdarray.h"
		#import "collections/arklinked_list.h"
		#import "collections/arkslice.h"
		#import "collections/arktrie.h"

	#else

		#if defined(IMPORT_ARKARRAY)
			#import "collections/arkarray.h"
		#endif

		#if defined(IMPORT_ARKDARRAY)
			#import "collections/arkdarray.h"
		#endif

		#if defined(IMPORT_ARKLINKED_LIST)
			#import "collections/arklinked_list.h"
		#endif

		#if defined(IMPORT_ARKTRIE)
			#import "collections/arktrie.h"
		#endif

	#endif

	#if defined(IMPORT_ARKMATH)
		#import "math/arkpair.h"
		#import "math/arkvector.h"

	#else

		#if defined(IMPORT_ARKPAIR)
			#import "math/arkpair.h"
		#endif

		#if defined(IMPORT_ARKVECTOR)
			#import "math/arkvector.h"
		#endif

	#endif
	#if defined(IMPORT_ARKIO)
		#import "io/arkfilesystem.h"
		#import "io/arkfstream.h"

	#else

		#if defined(IMPORT_ARKFILESYSTEM)
			#import "io/arkfilesystem.h"
		#endif

		#if defined(IMPORT_ARKFSTREAM)
			#import "io/arkfstream.h"
		#endif

	#endif

#endif

#undef OS

#endif /* ARKLIB_H_ */
