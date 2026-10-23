#if 0
	#include <string.h>

	#include "ark_trie.h"

ARK_BEGIN

ark_trienode *ark_make_trienode () {
	ark_trienode *t = ARK_MALLOC (sizeof (ark_trienode));
	*t				= (ark_trienode) {
					 .children = { },
					 .iterator = ARK_TRIE_ITERATOR,
	};
	return t;
}

bool ark_trie_insert_ (ark_trienode **root, const ucstring __restrict__ str) {
	ARK_NO_OP (root, str);
	return 0;
}
bool ark_trie_delete (ark_trienode *root, const ucstring __restrict__ str) {
	ARK_NO_OP (root, str);
	return 0;
}
bool ark_trie_find (const ark_trienode *root, const ucstring __restrict__ str) {
	ARK_NO_OP (root, str);
	return 0;
}
void ark_print_trie (const ark_trienode *root) { ARK_NO_OP (root); }

ark_trienode *ark_trie_begin (const ark_trienode *__restrict__ node) {
	ARK_NO_OP (node);
	return 0;
}
ark_trienode *ark_trie_end (const ark_trienode *__restrict__ node) {
	ARK_NO_OP (node);
	return 0;
}
ark_trienode *ark_trie_next (const ark_trienode *__restrict__ node) {
	ARK_NO_OP (node);
	return 0;
}
ark_trienode *ark_trie_prev (const ark_trienode *__restrict__ node) {
	ARK_NO_OP (node);
	return 0;
}
ark_trienode *ark_trie_peek (const ark_trienode *__restrict__ node) {
	ARK_NO_OP (node);
	return 0;
}
void ark_trie_advance (
	const ark_trienode *__restrict__ node, ptrdiff_t offset) {
	ARK_NO_OP (node, offset);
}
ptrdiff_t ark_trie_distance (
	const ark_trienode *__restrict__ n1, const ark_trienode *__restrict__ n2) {
	ARK_NO_OP (n1, n2);
	return 0;
}

ARK_END
// __attr_internal_symbol__ __attr_static_inline__ void
// ark_print_trie_r(const ark_trienode *node, ucstring prev, usz len) {
// 	uchar curr[len + 2];
// 	memcpy(curr, prev, len);
// 	curr[len + 1] = 0;
//
// 	if (node->terminal) printf("TRIE NODE: %s\n", curr);
//
// 	$ark_for(i, ARK_LC_TRIE_DEPTH) {
// 		if (node->children[i]) {
// 			curr[len] = i;
// 			ark_print_trie_r(node->children[i], curr, len + 1);
// 		}
// 	}
// }
//
// ark_trienode *ark_make_trienode() {
// 	ark_trienode *t = ARK_MALLOC(t, sizeof(ark_trienode));
// 	*t				= (ark_trienode){};
// 	return t;
// }
//
// void ark_destroy_trienode(ark_trienode *__restrict__ root) {
// 	for (usz i = 0; (i) < (256); ++(i)) {
// 		ARK_FREE(root->children[i]);
// 	}
// }
//
// bool ark_trie_insert_(ark_trienode **root, const ucstring __restrict__
// str) { 	if (!(*root)) { 		*root = ark_make_trienode();
// 	}
//
// 	usz			  len = strlen((cstring)str);
// 	ark_trienode *t	  = *root;
//
// 	$ark_for(i, len) {
// 		if (t->children[str[i]] == nullptr) {
// 			t->children[str[i]] = ark_make_trienode();
// 		}
// 		t = t->children[str[i]];
// 	}
//
// 	if (t->terminal) {
// 		return false;
// 	} else {
// 		t->terminal = true;
// 		return true;
// 	}
// }
//
// bool ark_trie_delete(ark_trienode *root, const ucstring str) {
// 	if (!root) return false;
// 	if (ark_trie_find(root, str)) {
// 		size_t		  len = strlen((cstring)str);
// 		ark_trienode *t	  = root;
// 		t				  = t->children[str[len - 1]];
// 		t->terminal		  = false;
// 		return true;
// 	}
// 	return false;
// }
//
// bool ark_trie_find(const ark_trienode *root, const ucstring str) {
// 	usz					len	 = strlen((cstring)str);
// 	ucstring			ustr = (ucstring)str;
// 	const ark_trienode *t	 = root;
//
// 	$ark_for(i, len) {
// 		if (t->children[ustr[i]] == nullptr) {
// 			return false;
// 		}
// 		t = t->children[ustr[i]];
// 	}
// 	return t->terminal;
// }
//
// void ark_print_trie(const ark_trienode *root) {
// 	if (!root) {
// 		puts("TRIE is empty!\n");
// 		return;
// 	}
// 	ark_print_trie_r(root, nullptr, 0);
// }
//
// ark_trienode *ark_trie_begin(const ark_trienode *__restrict__
// node) { 	while (ark_iter_offset(*node) > 0) { 		ark_iter_prev(*node);
// 	}
// }
//
// ark_trienode *ark_trie_end(const ark_trienode *__restrict__ node)
// {}
//
// ark_trienode *ark_trie_next(const ark_trienode *__restrict__ node)
// {}
//
// ark_trienode *ark_trie_prev(const ark_trienode *__restrict__ node)
// { 	if (node) { 		node->children[0];
// 	}
// }
//
// ark_trienode *ark_trie_peek(const ark_trienode *__restrict__ node)
// {}
//
// void
// ark_trie_advance(const ark_trienode *__restrict__ node, ptrdiff_t offset)
// {}
//
// ptrdiff_t ark_trie_distance(
// 	const ark_trienode *__restrict__ n1,
// 	const ark_trienode *__restrict__ n2
// ) {}
//
// ARK_END
#endif
