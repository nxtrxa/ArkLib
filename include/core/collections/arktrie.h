#if 0
	#ifndef ARK_TRIE_H_
		#define ARK_TRIE_H_

		#include "ark_array.h"
		#include "interfaces/allocator.h"
		#include "interfaces/iterator.h"
		#include "internals/arkdef.h"

ARK_BEGIN

		#ifdef ARK_NAMESPACE
			#define trienode	  ark_trienode
			#define make_trienode ark_make_trienode
			#define trie_insert	  ark_trie_insert
		#endif

/*
 * Ensure that a trie will support any ASCII character without the needing of
 * any bounces check
 */
static constexpr u16 ARK_LC_TRIE_DEPTH = 256;

		#define ARK_TRIE_ITERATOR                                                                                      \
			(&(ark_iterator) {                                                                                         \
				.begin = (ark_iter_begin_fn)ark_trie_begin,                                                            \
				.end   = (ark_iter_end_fn)ark_trie_end,                                                                \
				.next  = (ark_iter_next_fn)ark_trie_next,                                                              \
				.prev  = (ark_iter_prev_fn)ark_trie_prev,                                                              \
				.peek  = (ark_iter_prev_fn)ark_trie_peek,                                                              \
			})

ARK_CLASS (trienode) {
	ark_trienode *children[ARK_LC_TRIE_DEPTH];
	ark_iterator *iterator;
	usz			  size;
	bool		  terminal;
};

ARKAPI ark_trienode *ark_make_trienode ();
ARKAPI void			 ark_destroy_trienode (ark_trienode *root);
ARKAPI bool
		#define ark_trie_insert(Root_, Str_) ark_trie_insert_ (Root_, (ucstring)Str_)
ark_trie_insert_ (ark_trienode **root, UCString __restrict__ str);
ARKAPI bool ark_trie_delete (ark_trienode *root, UCString __restrict__ str);
ARKAPI bool ark_trie_find (const ark_trienode *root, UCString __restrict__ str);
ARKAPI void ark_print_trie (const ark_trienode *root);

ARKAPI ark_trienode *ark_trie_begin (const ark_trienode *__restrict__ node);
ARKAPI ark_trienode *ark_trie_end (const ark_trienode *__restrict__ node);
ARKAPI ark_trienode *ark_trie_next (const ark_trienode *__restrict__ node);
ARKAPI ark_trienode *ark_trie_prev (const ark_trienode *__restrict__ node);
ARKAPI ark_trienode *ark_trie_peek (const ark_trienode *__restrict__ node);
ARKAPI void

ark_trie_advance (const ark_trienode *__restrict__ node, ptrdiff_t offset);
ARKAPI ptrdiff_t ark_trie_distance (const ark_trienode *__restrict__ n1, const ark_trienode *__restrict__ n2);

ARK_END

	#endif /* ARK_TRIE_H_ */
#endif
