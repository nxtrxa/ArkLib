#define ARK_NAMESPACE

#include "arklib.h"

ARK_BEGIN

typedef struct {
	i32 i;
	f32 f;
} data_t;

#define data_t(I_, F_)                                                         \
	(data_t) { .i = I_, .f = F_ }

int main() {
	arena *main_arena = make_arena(KiB(1), ARENA_DEFAULT);
	defer { destroy_arena(main_arena); };
	ark_llist(data_t) ll = ark_make_llist(data_t, ARENA_ALLOCATOR(main_arena));

	data_t d;
	d = (data_t){ 1, 2.0 };
	ark_llist_push_back(ll, data_t, d);
	ark_llist_push_back(ll, data_t, data_t(1, 4.7));
	ark_llist_push_back(ll, data_t, data_t(1, 4.7));
	ark_llist_push_front(ll, data_t, data_t(1, 4.7));

	ark_llist_delete(ll, 2);

	printf("node len: %zu\n", (size_t)ll->len);
	$for(i, ll->len) printf("[%i] : { %i, %f }\n",
		(int)i,
		(int)ark_llist_get(ll, i).i,
		ark_llist_get(ll, i).f);

	return 0;
}
