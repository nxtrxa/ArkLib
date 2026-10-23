/*
 * @Copyright (c) 2026 Notrya Annes Avlis. All Rights Reserved.
 *
 *                      ARKLIB DEFER
 *
 * ARK_DEFER macro declare a new block marked with
 * __attribute__ ((cleanup(___ark__block_invoke__))).
 * ___ARK__BLOCK_INVOKE__ take as argument a block pointer, execute it and
 * release it with BLOCK_RELEASE in case of an heap block created with
 * BLOCK_COPY. Passing a stack block to BLOCK_RELEASE is harmless.
 *
 * Notes:
 *      1. By default inside a block out-of-scope variables are imutable.
 *         To capture those variables you must declare then with
 *         __attr_capture__ or __block, which are macros
 *         for __attribute__ ((__blocks__(byref))).
 *
 *         Be aware that if you pass a pointer to a free-like function, inside a
 *         block without declaring it with __attr_capture__, and this pointer is
 *         reallocated after the block declaration, the free-like function will
 *         free the old pointer, triggering a double-free.
 *
 *      1. Exiting from a function calling another function like EXIT doesn't
 *         unwind the stack and Therefore calling this function won't excute any ark_defer
 *         block. e.g.
 *
 *         int main (void) {
 *         ark_defer { printf ("DEFER NERVER RUN"); };
 *         exit (EXIT_SUCCESS);
 *         }
 */
#ifndef ARKLIB_INTERNALS_DEFER_H_
#define ARKLIB_INTERNALS_DEFER_H_

#import "arkdefs.h"

ARK_BEGIN

#if (ARKLIB_IS_CLANG && ARKLIB_HAS_FEATURE(blocks)) || defined(__BLOCKS__)
	#define ARK_HAS_BLOCKS 1
#else
	#define ARK_HAS_BLOCKS 0
#endif

#if ARK_HAS_BLOCKS
	#import "BlocksRuntime/Block.h"
#endif

#define ark_defer                                                                                                      \
	__attr_cleanup__ (___ark__block_invoke__) ark_lambda_t (u0, ark_pp_concat (__func__, __LINE__), (u0)) = ^

__decl_static_inline__
__decl_deprecated__ ("C29 working draft introduced defer as a new macro. Defined in `<stddefer.h>`") u0 ___ark__block_invoke__ (u0 (^*block_) (u0)) __attr_enable_if__ (
	ARK_HAS_BLOCKS, "Only supported by clang with -fblocks and -lBlocksRuntime for non apple environments."
) {
	if (*block_) {
		(*block_) ();
		Block_release (*block_);
	}
}

ARK_END

#endif /* ARKLIB_INTERNALS_DEFER_H_ */
