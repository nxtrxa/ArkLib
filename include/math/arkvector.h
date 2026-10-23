/*
 *  Copyright (c) 2026 Murilo Gonzalez. All Rights Reserved.
 *
 *                          ARKLIB VECTOR
 *
 * Simple macro wrappers for Clang elementwise/reduction/masked builtins.
 *
 * Note:
 *  By default OpenCl vectors are used. OpenCL vector types support swizzling
 *  syntax e.g.
 *      ark_vector (f32, 4) v = {1.0, 2.0, 3.0, 4.0}; // expands to: ark_vf32x4 v
 *      ark_vector (f32, 2) v2;
 *      v2 = v.wz;  // v2 = {3.0, 4.0}
 */

#ifndef ARK_VECTOR_H_
#define ARK_VECTOR_H_

#include "internals/arkdefs.h"

ARK_BEGIN

#ifdef ARK_NAMESPACE
	#define vector						 ark_vector
	#define vector_sin					 ark_vector_sin
	#define vector_cos					 ark_vector_cos
	#define vector_tan					 ark_vector_tan
	#define vector_asin					 ark_vector_asin
	#define vector_acos					 ark_vector_acos
	#define vector_atan					 ark_vector_atan
	#define vector_sinh					 ark_vector_sinh
	#define vector_cosh					 ark_vector_cosh
	#define vector_tanh					 ark_vector_tanh
	#define vector_ceil					 ark_vector_ceil
	#define vector_floor				 ark_vector_floor
	#define vector_log					 ark_vector_log
	#define vector_log2					 ark_vector_log2
	#define vector_log10				 ark_vector_log10
	#define vector_exp					 ark_vector_exp
	#define vector_exp2					 ark_vector_exp2
	#define vector_exp10				 ark_vector_exp10
	#define vector_sqrt					 ark_vector_sqrt
	#define vector_roundeven			 ark_vector_roundeven
	#define vector_round				 ark_vector_round
	#define vector_trunc				 ark_vector_trunc
	#define vector_nearbyint			 ark_vector_nearbyint
	#define vector_rint					 ark_vector_rint
	#define vector_canonicalize			 ark_vector_canonicalize
	#define vector_abs					 ark_vector_abs
	#define vector_popcount				 ark_vector_popcount
	#define vector_bitreverse			 ark_vector_bitreverse
	#define vector_atan2				 ark_vector_atan2
	#define vector_pow					 ark_vector_pow
	#define vector_copysign				 ark_vector_copysign
	#define vector_fmod					 ark_vector_fmod
	#define vector_max					 ark_vector_max
	#define vector_min					 ark_vector_min
	#define vector_maxnum				 ark_vector_maxnum
	#define vector_minnum				 ark_vector_minnum
	#define vector_add_sat				 ark_vector_add_sat
	#define vector_sub_sat				 ark_vector_sub_sat
	#define vector_maximum				 ark_vector_maximum
	#define vector_minimum				 ark_vector_minimum
	#define vector_maximumnum			 ark_vector_maximumnum
	#define vector_minimumnum			 ark_vector_minimumnum
	#define vector_clzg					 ark_vector_clzg
	#define vector_ctzg					 ark_vector_ctzg
	#define vector_clmul				 ark_vector_clmul
	#define vector_fma					 ark_vector_fma
	#define vector_ldexp				 ark_vector_ldexp
	#define vector_fshl					 ark_vector_fshl
	#define vector_fshr					 ark_vector_fshr
	#define vector_reduce_max			 ark_vector_reduce_max
	#define vector_reduce_min			 ark_vector_reduce_min
	#define vector_reduce_add			 ark_vector_reduce_add
	#define vector_reduce_mul			 ark_vector_reduce_mul
	#define vector_reduce_and			 ark_vector_reduce_and
	#define vector_reduce_or			 ark_vector_reduce_or
	#define vector_reduce_xor			 ark_vector_reduce_xor
	#define vector_reduce_maximum		 ark_vector_reduce_maximum
	#define vector_reduce_minimum		 ark_vector_reduce_minimum
	#define vector_reduce_assoc_fadd	 ark_vector_reduce_assoc_fadd
	#define vector_reduce_in_order_fadd	 ark_vector_reduce_in_order_fadd
	#define vector_masked_load			 ark_vector_masked_load
	#define vector_masked_store			 ark_vector_masked_store
	#define vector_masked_expand_load	 ark_vector_masked_expand_load
	#define vector_masked_compress_store ark_vector_masked_compress_store
	#define vector_masked_gather		 ark_vector_masked_gather
#endif

#define ark_vector(Type_, NCmps_) ark_pp_concat (ark_v, Type_, x, NCmps_)

#ifndef ARK_VECTOR_TYPE
	#define ARK_VECTOR_TYPE(N_) __attr_ext_vec_type__ (N_)
#endif

#define ARK_VECTOR_TYPES                                                                                               \
	X (i8)                                                                                                             \
	X (i16)                                                                                                            \
	X (i32)                                                                                                            \
	X (i64)                                                                                                            \
	X (i128)                                                                                                           \
	X (u8)                                                                                                             \
	X (u16)                                                                                                            \
	X (u32)                                                                                                            \
	X (u64)                                                                                                            \
	X (u128)                                                                                                           \
	X (f32)                                                                                                            \
	X (f64)                                                                                                            \
	X (f128)

#define X(Type_) typedef Type_ ark_v##Type_##x2 ARK_VECTOR_TYPE (2);

ARK_VECTOR_TYPES

#undef X

#define X(Type_) typedef Type_ ark_v##Type_##x3 ARK_VECTOR_TYPE (3);

ARK_VECTOR_TYPES

#undef X

#define X(Type_) typedef Type_ ark_v##Type_##x4 ARK_VECTOR_TYPE (4);

ARK_VECTOR_TYPES

#undef X

#ifdef ARK_NAMESPACE
	#define X(Type_) typedef ark_v##Type_##x2 v##Type_##x2;
ARK_VECTOR_TYPES
	#undef X

	#define X(Type_) typedef ark_v##Type_##x3 v##Type_##x3;
ARK_VECTOR_TYPES
	#undef X

	#define X(Type_) typedef ark_v##Type_##x4 v##Type_##x4;
ARK_VECTOR_TYPES
	#undef X
#endif

/* ---------- Builtin macro wrappers (original param names:
   - "floating point types" means scalar or floating-point vector lanes.
   - "integer types" means scalar or integer vector lanes.
   - For two-/three-arg forms the parameter names match the builtin signatures.
*/

/* Elementwise single-arg (fp) */


/*
 * floating point types
 */
#define ark_vector_sin(X_) __builtin_elementwise_sin (X_)

/*
 * floating point types
 */
#define ark_vector_cos(X_) __builtin_elementwise_cos (X_)

/*
 * floating point types
 */
#define ark_vector_tan(X_) __builtin_elementwise_tan (X_)

/*
 * floating point types
 */
#define ark_vector_asin(X_) __builtin_elementwise_asin (X_)

/*
 * floating point types
 */
#define ark_vector_acos(X_) __builtin_elementwise_acos (X_)

/*
 * floating point types
 */
#define ark_vector_atan(X_) __builtin_elementwise_atan (X_)

/*
 * floating point types
 */
#define ark_vector_sinh(X_) __builtin_elementwise_sinh (X_)

/*
 * floating point types
 */
#define ark_vector_cosh(X_) __builtin_elementwise_cosh (X_)
/*
 * floating point types
 */
#define ark_vector_tanh(X_) __builtin_elementwise_tanh (X_)

/*
 * floating point types
 */
#define ark_vector_ceil(X_) __builtin_elementwise_ceil (X_)

/*
 * floating point types
 */
#define ark_vector_floor(X_) __builtin_elementwise_floor (X_)

/*
 * floating point types
 */
#define ark_vector_log(X_) __builtin_elementwise_log (X_)

/*
 * floating point types
 */
#define ark_vector_log2(X_) __builtin_elementwise_log2 (X_)

/*
 * floating point types
 */
#define ark_vector_log10(X_) __builtin_elementwise_log10 (X_)

/*
 * floating point types
 */
#define ark_vector_exp(X_) __builtin_elementwise_exp (X_)

/*
 * floating point types
 */
#define ark_vector_exp2(X_) __builtin_elementwise_exp2 (X_)

/*
 * floating point types
 */
#define ark_vector_exp10(X_) __builtin_elementwise_exp10 (X_)

/*
 * floating point types
 */
#define ark_vector_sqrt(X_) __builtin_elementwise_sqrt (X_)

/*
 * floating point types
 */
#define ark_vector_roundeven(X_) __builtin_elementwise_roundeven (X_)

/*
 * floating point types
 */
#define ark_vector_round(X_) __builtin_elementwise_round (X_)

/*
 * floating point types
 */
#define ark_vector_trunc(X_) __builtin_elementwise_trunc (X_)

/*
 * floating point types
 */
#define ark_vector_nearbyint(X_) __builtin_elementwise_nearbyint (X_)

/*
 * floating point types
 */
#define ark_vector_rint(X_) __builtin_elementwise_rint (X_)

/*
 * floating point types
 */
#define ark_vector_canonicalize(X_) __builtin_elementwise_canonicalize (X_)

/*
 * Signed integer and floating point types.
 *
 * Note(s):
 *  most-neg stays most-neg.
 */
#define ark_vector_abs(X_) __builtin_elementwise_abs (X_)

/*
 * integer types
 */
#define ark_vector_popcount(X_) __builtin_elementwise_popcount (X_)

/*
 * integer types
 */
#define ark_vector_bitreverse(X_) __builtin_elementwise_bitreverse (X_)


/* Elementwise two-arg (fp/int) */

/*
 * floating point types
 */
#define ark_vector_atan2(Y_, X_) __builtin_elementwise_atan2 (Y_, X_)

/*
 * floating point types
 */
#define ark_vector_pow(X_, Y_) __builtin_elementwise_pow (X_, Y_)

/*
 * floating point types
 */
#define ark_vector_copysign(X_, Y_) __builtin_elementwise_copysign (X_, Y_)

/*
 * floating point types
 */
#define ark_vector_fmod(X_, Y_) __builtin_elementwise_fmod (X_, Y_)

/*
 * integer and floating point (deprecated name)
 */
#define ark_vector_max(X_, Y_) __builtin_elementwise_max (X_, Y_)

/*
 * integer and floating point (deprecated name)
 */
#define ark_vector_min(X_, Y_) __builtin_elementwise_min (X_, Y_)

/*
 * floating point types
 */
#define ark_vector_maxnum(X_, Y_) __builtin_elementwise_maxnum (X_, Y_)

/*
 * floating point types
 */
#define ark_vector_minnum(X_, Y_) __builtin_elementwise_minnum (X_, Y_)

/*
 * integer types
 */
#define ark_vector_add_sat(X_, Y_) __builtin_elementwise_add_sat (X_, Y_)

/*
 * integer types
 */
#define ark_vector_sub_sat(X_, Y_) __builtin_elementwise_sub_sat (X_, Y_)

/*
 * floating point types
 */
#define ark_vector_maximum(X_, Y_) __builtin_elementwise_maximum (X_, Y_)

/*
 * floating point types
 */
#define ark_vector_minimum(X_, Y_) __builtin_elementwise_minimum (X_, Y_)

/*
 * floating point types
 */
#define ark_vector_maximumnum(X_, Y_) __builtin_elementwise_maximumnum (X_, Y_)

/*
 * floating point types
 */
#define ark_vector_minimumnum(X_, Y_) __builtin_elementwise_minimumnum (X_, Y_)

/*
 * integer types; optional fallback Y when X == 0
 */
#define ark_vector_clzg(X_, Y_) __builtin_elementwise_clzg (X_, Y_)

/*
 * integer types; optional fallback Y when X==0
 */
#define ark_vector_ctzg(X_, Y_) __builtin_elementwise_ctzg (X_, Y_)

/*
 * integer types
 */
#define ark_vector_clmul(X_, Y_) __builtin_elementwise_clmul (X_, Y_)


/* Elementwise three-arg */

/*
 * floating point types: (X* Y) + Z fused
 */
#define ark_vector_fma(X_, Y_, Z_) __builtin_elementwise_fma (X_, Y_, Z_)

/*
 * floating point X, integer Y (matching shape)
 */
#define ark_vector_ldexp(X_, Y_) __builtin_elementwise_ldexp (X_, Y_)

/*
 * integer types: funnel shift left (X_ MSB concat Y_)
 */
#define ark_vector_fshl(X_, Y_, Z_) __builtin_elementwise_fshl (X_, Y_, Z_)

/*
 * integer types: funnel shift right
 */
#define ark_vector_fshr(X_, Y_, Z_) __builtin_elementwise_fshr (X_, Y_, Z_)

/* ---------- Reduction builtins (vector -> scalar) ----------
   All accept X_ single vector 'X_' unless noted. Integer reductions are
   constexpr-capable.
*/

/*
 * integer and floating point types
 */
#define ark_vector_reduce_max(X_) __builtin_reduce_max (X_)

/*
 * integer and floating point types
 */
#define ark_vector_reduce_min(X_) __builtin_reduce_min (X_)

/*
 * integer types
 */
#define ark_vector_reduce_add(X_) __builtin_reduce_add (X_)

/*
 * integer types
 */
#define ark_vector_reduce_mul(X_) __builtin_reduce_mul (X_)

/*
 * integer types
 */
#define ark_vector_reduce_and(X_) __builtin_reduce_and (X_)

/*
 * integer types
 */
#define ark_vector_reduce_or(X_) __builtin_reduce_or (X_)

/*
 * integer types
 */
#define ark_vector_reduce_xor(X_) __builtin_reduce_xor (X_)

/*
 * floating point types
 */
#define ark_vector_reduce_maximum(X_) __builtin_reduce_maximum (X_)

/*
 * floating point types
 */
#define ark_vector_reduce_minimum(X_) __builtin_reduce_minimum (X_)

/*
 * floating point types; optional init S_
 */
#define ark_vector_reduce_assoc_fadd(X_, S_) __builtin_reduce_assoc_fadd (X_, S_)

/*
 * floating point types; in-order add with init S_
 */
#define ark_vector_reduce_in_order_fadd(X_, S_) __builtin_reduce_in_order_fadd (X_, S_)

/* ---------- Masked memory / gather-scatter ----------
   signatutres:
   - masked_load(Mask_, Ptr_, opt) -> vector   (Mask_: bool vector, Ptr_: pointer to
   element type, opt: vector for masked-off lanes)
   - masked_store(Mask_, val, Ptr_) -> void
   - masked_expand_load(Mask_, Ptr_, opt) -> vector (loads into consecutive
   indices where Mask_ true)
   - masked_compress_store(Mask_, val, Ptr_) -> void (stores val lanes where Mask_
   true to consecutive memory)
   - masked_gather(Mask_, Idx_, Ptr_) -> vector (Idx_: integer vector of indices)
   - masked_scatter(Mask_, val, Idx_, Ptr_) -> void
*/

/*
 * Mask_: bool vector, Ptr_: T*, opt: vector for masked-off lanes
 */
#define ark_vector_masked_load(Mask_, Ptr_, opt) __builtin_masked_load (Mask_, Ptr_, opt)

/*
 * Mask_: bool vector, val: vector, Ptr_: T*
 */
#define ark_vector_masked_store(Mask_, val, Ptr_) __builtin_masked_store (Mask_, val, Ptr_)

#define ark_vector_masked_expand_load(Mask_, Ptr_, opt) __builtin_masked_expand_load (Mask_, Ptr_, opt)

#define ark_vector_masked_compress_store(Mask_, val, Ptr_) __builtin_masked_compress_store (Mask_, val, Ptr_)

/*
 * Idx_: integer vector of indices
 */
#define ark_vector_masked_gather(Mask_, Idx_, Ptr_) __builtin_masked_gather (Mask_, Idx_, Ptr_)
ARK_END

#endif /* ARK_VECTOR_H_ */
