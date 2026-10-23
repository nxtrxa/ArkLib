#import "arklib.h"


int main (int argc, char** argv) {
	ark_unused (argc, argv);

	int	 arr[8]					   = {1, 2, 3, 4, 5, 6, 7, 8};
	int* ptr					   = arr;
	ark_raii (ark_darray) (int) da = ark_make_darray (i32);
	ark_darray_vpush_back (da, 14, 17, 47, 74);

	ark_slice (int) sl	= ark_make_slice (int, arr, 8);
	ark_slice (int) sl2 = ark_make_slice (int, ptr, 4);
	ark_slice (int) sl3 = ark_make_slice (int, da->data, da->size, 1, 3);

	$foreach (e $in_slice (sl)) {
		printf ("%i ", *e);
	}
	putchar ('\n');
	$foreach (e $in_slice (sl2)) {
		printf ("%i ", *e);
	}
	putchar ('\n');
	$foreach (e $in_slice (sl3)) {
		printf ("%i ", *e);
	}

	return 0;
}
