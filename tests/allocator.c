#define ARK_NAMESPACE
#import "arklib.h"

int main (void) {
	ark_class_async_arena (PANIC_ON_EXCEPTION, as_arena);
	ark_class_sync_arena (PANIC_ON_EXCEPTION, sc_arena);

	ark_scoped_arena_resource (as_arena) {
		darray (i32) da = make_darray (i32, ASYNC_ARENA_ALLOCATOR (as_arena));
		darray_vpush_back (da, 1, 2, 3, 4, 5, 6, 7);
		$foreach (e $in_darray (da)) {
			printf ("%i ", *e);
		}
	}

	return 0;
}
