#define ARK_NAMESPACE
#include "arklib.h"

ARK_BEGIN

#define SET_BIT(X_, N_)		   (X_ |= ((typeof_unqual(X_))1 << N_))
#define UNSET_BIT(X_, N_)	   (X_ &= (~(typeof_unqual(X_))1 << N_))
#define ARK_IS_BIT_SET(X_, N_) (((X_) >> (N_)) & 0x1)

int main() {
	// u64 ba2 = 0;
	//
	// SET_BIT(ba2, 0);
	// SET_BIT(ba2, 1);
	// SET_BIT(ba2, 4);
	// SET_BIT(ba2, 8);
	// SET_BIT(ba2, 31);
	// SET_BIT(ba2, 63);
	// UNSET_BIT(ba2, 63);
	//
	// if (ARK_IS_BIT_SET(ba2, 63)) printf("%lb\n", ba2);

	ark_trienode *trie = ark_make_trienode();

	bool ok = ark_trie_insert(&trie, "FOO");

	if (!ok) printf("already in trie\n");
	ok = ark_trie_insert(&trie, "FOO");
	if (!ok) printf("already in trie\n");
	ok = ark_trie_insert(&trie, "foo");
	if (!ok) printf("already in trie\n");
	ok = ark_trie_insert(&trie, "baz");
	if (!ok) printf("already in trie\n");
	ok = ark_trie_insert(&trie, "bazz");
	if (!ok) printf("already in trie\n");
	ok = ark_trie_insert(&trie, "bazii");
	if (!ok) printf("already in trie\n");

	if (ark_trie_find(trie, (ucstring) "foo")) ark_print_trie(trie);

	// ark_trie_delete(trie, (ucstring) "foo");

	// ark_print_trie(trie);

	ark_destroy_trienode(trie);
}
