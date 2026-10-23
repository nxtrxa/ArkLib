#define ARK_NAMESPACE
#include "arklib.h"

ARK_BEGIN

/*
 * 100000 iterations of get & return operations takes ~ 30s
 * performance is roughly 3300G&R/s
 */
#define ITERATIONS 10000

ARK_CLASS (data) {
	char**	 env;
	locale_t loc;
};


MAKE_POOL (data_p, 10000)

int main () {
	$for (i, ITERATIONS) {
		sz		p = rand () % ITERATIONS;
		data_p* d[p];
		$for (j, p) {
			d[j] = pool_get_data_p ();
		}
		printf ("%zuth iteration | structs count %zu \n", (size_t)i, (size_t)p);
		$for (j, p) {
			pool_return_data_p (d[j]);
		}
	}
}
