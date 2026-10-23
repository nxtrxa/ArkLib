#define ARK_NAMESPACE
#define IMPORT_ARKLIB
#include "arklib.h"

ARK_BEGIN

int int_compar_fn (int* a, int* b) {
	return *a - *b;
}

ARK_CLASS (S) {
	int a;
	int b;
};

int main (__attr_unused__ int argc, __attr_unused__ char** argv) {

	class_array (arr, S, 8);
	class_array (arr2, S, 12);

	ark_array_fill (arr, ((S) {1, 4}));

	class_array (arr3, S_p, 8);

	// $foreach (i, e, arr.data, arr.size) {
	// 	printf ("%i, %i  ", e->a, e->b);
	// 	array_set (arr2, i, e);
	// }

	int buff[] = {1, 2, 3, 4};

	class_array (arr4, int, 1);

	putchar ('\n');


	array_sort (arr, int_compar_fn);

	$foreach (e $in_array (arr)) {
		printf ("%i, %i  ", (e)->a, (e)->b);
	}
	putchar ('\n');

	return 0;
}
