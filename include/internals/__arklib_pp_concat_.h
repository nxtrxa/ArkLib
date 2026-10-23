#ifndef ARKLIB_INTERNALS_PP_CONCAT_H_
#define ARKLIB_INTERNALS_PP_CONCAT_H_

#define __CONCAT_(a, b)	  ___CONCAT__ (a, b)
#define ___CONCAT__(a, b) a##b

#define __CONCAT_2(a, b) __CONCAT_2_ (a, b)

#define __CONCAT_3(a, b, c) __CONCAT_3_ (a, b, c)

#define __CONCAT_4(a, b, c, d) __CONCAT_4_ (a, b, c, d)

#define __CONCAT_5(a, b, c, d, e) __CONCAT_5_ (a, b, c, d, e)

#define __CONCAT_2_(a, b) a##b

#define __CONCAT_3_(a, b, c) a##b##c

#define __CONCAT_4_(a, b, c, d) a##b##c##d

#define __CONCAT_5_(a, b, c, d, e) a##b##c##d##e

#endif /* ARKLIB_INTERNALS_PP_CONCAT_H_ */
