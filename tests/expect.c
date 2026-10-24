#define ARK_NAMESPACE
#import "arklib.h"

BEGIN

#define ARK_EXPECT_CONTEXT MAIN

ark_faultdefs (ZERODIV, ZEROLEN);

expect (f64) fn (f64 x, f64 y) {
	if (!y) return ark_unexpected (ZERODIV);

	return ark_expected (x / y);
}

#undef ARK_EXPECT_CONTEXT /* MAIN */

typedef struct a {
	int a;
} a;

ark_tagged_union (types, (a, i), (float, f));

int main (int argc, char** argv) {

	ark_tagged_union_t (types) tu = ark_make_tagged_union (types, a, i = {.a = 1});

	f64 f = $catch (fn (10, 0), e, printf ("%s", e.to_str));

	return 0;
}
