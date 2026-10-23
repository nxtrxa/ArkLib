#import "arklib.h"

ARK_BEGIN

struct payload {
	ark_sync_arena* _arena;
	u64				_sz;
	i64				_n;
};

u0* worker (ark_stop_token tk_, u0* arg_) {
	CString name = arg_;
	int		C	 = 0;
	while (!ark_thread_stop_requested (tk_)) {
		printf ("%s: working (%d)\n", name, C++);
		usleep (100000);
	}
	printf ("thread %s finished\n", name);
	return nullptr;
}

ark_thread_worker (worker_2, struct payload) {
	struct payload* pl		  = (struct payload*)arg_;
	ark_allocator	allocator = ARK_SYNC_ARENA_ALLOCATOR (pl->_arena);
	u64				size	  = pl->_sz;
	i64				n		  = pl->_n;

	ark_darray (i64) data = ark_make_darray (i64, allocator, size);

	i64 C = 1;
	$for (i, n) {
		ark_darray_push_back (data, C);
		C++;
	}

	$foreach (e $in_darray (data)) {
		printf ("%zu ", *e);
	}

	printf ("\nthread %s finished\n", $__function__);
	return data;
}

int main (u0) {
	ark_class_sync_arena (ARK_PANIC_ON_EXCEPTION, arena);
	ark_class_thread (ARK_PANIC_ON_EXCEPTION, ta, worker_2, &(struct payload) {arena, 256, 16});

	ark_thread_join (ta);

	return 0;
}

ARK_END
