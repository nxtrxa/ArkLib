#define ARK_NAMESPACE
#import "arklib.h"

ARK_BEGIN

int main (u0) {
	array (i32, 14) arr	 = make_array (i32, 4);
	i32			carr[4]	 = {1, 2, 3, 4};
	i32			carr2[4] = {-5, -6, 7, 8};
	string_view sv		 = make_string_view ("String");

	iterator (i32) arr_iter	  = make_iterator (i32, $from_array (arr));
	iterator (i32) carr_iter  = make_iterator (i32, carr);
	iterator (i32) carr2_iter = make_iterator (i32, carr2);
	iterator (Char) sv_iter	  = make_iterator (Char, $from_string (sv));

	iterate (x, carr_iter) {
		printf ("%i\n", (int)*x);
	}

	if (iterator_all (carr_iter, x, *x > 0)) {
		printf ("all are positive\n");
	}

	if (iterator_any (carr2_iter, x, *x < 0)) {
		printf ("at least one is negative\n");
	}

	$foreach (i, e $in_array (arr)) {
		*e = i * i;
	}

	iterator_chain (x, carr_iter, carr2_iter, arr_iter) {
		printf ("%i\n", (int)*x);
	}

	iterator_zip (x, carr_iter, carr2_iter, sv_iter) {
		printf ("%i - %i - %c \n", (int)*x.it1, (int)*x.it2, (int)*x.it3);
	}

	iterator_cycle (x, sv_iter, 20) {
		printf ("%c ", *x);
	}
}
