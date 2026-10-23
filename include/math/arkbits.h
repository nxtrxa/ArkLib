#ifndef ARKLIB_MATH_BITS_H_
#define ARKLIB_MATH_BITS_H_

#import "__stddef_max_align_t.h"

#ifdef ARK_NAMESPACE
	#define math_min			 ark_math_min
	#define math_max			 ark_math_max
	#define math_is_pow2		 ark_math_is_pow2
	#define math_align_up_pow2	 ark_math_align_up_pow2
	#define math_align_down_pow2 ark_math_align_down_pow2
	#define math_align_up		 ark_math_align_up
	#define math_align_down		 ark_math_align_down
#elif defined(ARK_MATH_NAMESPACE)
	#define min				ark_math_min
	#define max				ark_math_max
	#define is_pow2			ark_math_is_pow2
	#define align_up_pow2	ark_math_align_up_pow2
	#define align_down_pow2 ark_math_align_down_pow2
	#define align_up		ark_math_align_up
	#define align_down		ark_math_align_down
#endif

/* Min / Max Macros */
#define ark_math_min(A_, B_) (((A_) < (B_)) ? (A_) : (B_))
#define ark_math_max(A_, B_) (((A_) > (B_)) ? (A_) : (B_))

/* Alignment and Power of Two Math */
#define ark_math_is_pow2(X_) !(((X_) & (X_ - 1)))

#define ark_math_align_up_pow2(X_) (((u64)(X_) <= 1) ? 1 : (1UL << (64 - ARK_CLZLL ((u64)(X_) - 1))))

#define ark_math_align_down_pow2(X_) (((u64)(X_) == 0) ? 0 : (1UL << (64 - ARK_CLZLL ((u64)(X_)) - 1)))

#define ark_math_align_up(N_, Align_) (((N_) + ((Align_) - 1)) & ~((Align_) - 1))

#define ark_math_align_down(N_, Align_) ((N_) & ~((Align_) - 1))

static constexpr auto ARK_MATH_MAX_ALIGN = alignof (max_align_t);

#endif /* ARKLIB_MATH_BITS_H_ */
