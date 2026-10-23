#include <argz.h>

#define ARK_NAMESPACE
#import "arklib.h"

ARK_BEGIN

int main () {
	darray (u32) da = make_darray (u32, 0).unwrap;

	ark_darray_reserve (da, 32);

	u32* slot = $catch (darray_push_back (da, 1), e, PANIC_ON_EXCEPTION (e)); // slot points to last element

	printf ("%p - %i\n", (u0*)slot, *slot);

	// slot points to the first element in list (`1` here)
	u32* slot_2 = $catch (darray_vpush_back (da, 2, 3, 4, 5, 6, 7, 8, 9), e, PANIC_ON_EXCEPTION (e));

	printf ("%p - %i\n", (u0*)slot_2, *slot_2);

	u32* slot_3 = $catch (darray_push_back (da, 7), e, PANIC_ON_EXCEPTION (e)); // slot points to last element

	u32* slot_4 = $catch (darray_vpush_back (da, 2, 3, 4, 5, 6, 7, 8, 9), e, PANIC_ON_EXCEPTION (e));

	// u32* p = alloca (16);
	// ark_darray_append (da, p, 16);

	printf (
		"capacity: %zu\nsize: %zu\nheader size: %zu\nelement size: "
		"%zu\nalignment: %zu\n",
		da->_capacity,
		da->_size,
		sizeof (*(da)),
		sizeof ((da)->_data[0]),
		alignof (da)
	);

	$foreach (e $in_darray (da)) {
		printf ("%i ", *e);
	}

	ark_destroy_darray (da);
}
