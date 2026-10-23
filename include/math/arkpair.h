#ifndef ARK_PAIR_H_
#define ARK_PAIR_H_

#include "internals/arkdefs.h"

ARK_BEGIN

#ifdef ARK_NAMESPACE
	#define pair	  ark_pair
	#define make_pair ark_make_pair
#endif

#define ark_pair(...) ark_pp_concat (ark_pair_, ark_pp_nargs (__VA_ARGS__)) (__VA_ARGS__)

/*
 *  Just to make code more explicity
 */

#define ark_pair_1(T_)                                                                                                 \
	struct ark_pp_concat (ark_pair_, T_) {                                                                             \
		T_ first;                                                                                                      \
		T_ second;                                                                                                     \
	}

#define ark_pair_2(T_, U_)                                                                                             \
	struct ark_pp_concat (ark_pair_, T_, _, U_) {                                                                      \
		T_ first;                                                                                                      \
		U_ second;                                                                                                     \
	}

#define ark_pair_3(T_, First_, Second_)                                                                                \
	struct ark_pp_concat (ark_pair_, T_, _, First_, Second_) {                                                         \
		T_ First_;                                                                                                     \
		T_ Second_;                                                                                                    \
	}

#define ark_make_pair(...) ark_pp_concat (ark_make_pair_, ark_pp_nargs (__VA_ARGS__)) (__VA_ARGS__)

#define ark_make_pair_2(First_, Second_) {First_, Second_}

#define ark_make_pair_3(Type_, First_, Second_)                                                                        \
	(ark_pair (Type_)) {                                                                                               \
		First_, Second_                                                                                                \
	}

ARK_END

#endif // ARK_PAIR_H_
