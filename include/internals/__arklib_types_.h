/*
 * @Copyright (c) 2026 Murilo Gonzalez. All Rights Reserved.
 *
 *                          ARKLIB TYPES
 *
 * Integers types are typedefs for C23 bit precise integers.
 *
 * Note:
 *  Bit precise integers aren't implicit converted to C primitive integer types
 *  neither <stdint.h> types,
 *
 * Some of these aliases are made for datatypes that use concatanetion to create
 * symbols. e.g. ark_darray(const u32 *) would brake when expanded. Therefore
 * type aliases that don't have spaces or non alphanumeric characters are needed.
 *
 * Aliases format:
 *  f32 * -> f32_p.
 *  const* f32 -> F32_p.
 *  const* const f32 -> UF32_P.
 *  unsigned i32 * -> ui32_p.
 *  const unsigned i32 * -> UI32_p.
 *
 * char_p and cstring are both typedefs for char*, distinction is made just
 * for code clarity,
 *
 * e.g.
 * Char_p == CString == const char*
 * UChar_p == UCstring == const unsigned char*
 */

#ifndef ARK_INTERNALS_TYPES_H_
#define ARK_INTERNALS_TYPES_H_

#import <inttypes.h>
#import <stdint.h>
#import <wchar.h>

#if defined(_WIN32) || defined(__CYGWIN__)
typedef wchar_t oschar_t;
	#define ark_osstr(Str_) L##Str_
#else
typedef char oschar_t;
	#define ark_osstr(Str_) Str_
#endif

#define bitint _BitInt

#define ___ARK_INT_T__(Length_)	 bitint (Length_)
#define ___ARK_UINT_T__(Length_) unsigned bitint (Length_)

typedef ___ARK_UINT_T__ (1) bit_t;

#define ___ARK__BITINT_TYPE_DECLS__                                                                                    \
	___ARK__DECL_BITINT_TYPE__ (2)                                                                                     \
	___ARK__DECL_BITINT_TYPE__ (4)                                                                                     \
	___ARK__DECL_BITINT_TYPE__ (128)

#define ___ARK__DECL_BITINT_TYPE__(Length_)                                                                            \
	typedef ___ARK_INT_T__ (Length_) i##Length_;                                                                       \
	typedef ___ARK_INT_T__ (Length_) * i##Length_##_p;                                                                 \
	typedef ___ARK_INT_T__ (Length_)* const i##Length_##_P;                                                            \
	typedef const ___ARK_INT_T__ (Length_) I##Length_;                                                                 \
	typedef const ___ARK_INT_T__ (Length_) * I##Length_##_p;                                                           \
	typedef const ___ARK_INT_T__ (Length_)* const I##Length_##_P;

___ARK__BITINT_TYPE_DECLS__

#undef ___ARK__DECL_BITINT_TYPE__

#define ___ARK__DECL_BITINT_TYPE__(Length_)                                                                            \
	typedef ___ARK_UINT_T__ (Length_) u##Length_;                                                                      \
	typedef ___ARK_UINT_T__ (Length_) * u##Length_##_p;                                                                \
	typedef ___ARK_UINT_T__ (Length_)* const u##Length_##_P;                                                           \
	typedef const ___ARK_UINT_T__ (Length_) U##Length_;                                                                \
	typedef const ___ARK_UINT_T__ (Length_) * U##Length_##_p;                                                          \
	typedef const ___ARK_UINT_T__ (Length_)* const U##Length_##_P;

___ARK__BITINT_TYPE_DECLS__

#undef ___ARK__DECL_BITINT_TYPE__

/*
 * C primitive integers types aren't included
 */
#define ___ARK__C_PRIMITIVE_TYPES_ALIASES__                                                                            \
	___ARK__TYPE_ALIAS__ (void, u0)                                                                                    \
	___ARK__TYPE_ALIAS__ (const void, U0)                                                                              \
	___ARK__TYPE_ALIAS__ (float, f32)                                                                                  \
	___ARK__TYPE_ALIAS__ (const float, F32)                                                                            \
	___ARK__TYPE_ALIAS__ (double, f64)                                                                                 \
	___ARK__TYPE_ALIAS__ (const double, F64)                                                                           \
	___ARK__TYPE_ALIAS__ (long double, f128)                                                                           \
	___ARK__TYPE_ALIAS__ (const long double, F128)                                                                     \
	___ARK__TYPE_ALIAS__ (const char, Char)                                                                            \
	___ARK__TYPE_ALIAS__ (unsigned char, uchar)                                                                        \
	___ARK__TYPE_ALIAS__ (const unsigned char, UChar)                                                                  \
	___ARK__TYPE_ALIAS__ (char*, cstring)                                                                              \
	___ARK__TYPE_ALIAS__ (unsigned char*, ucstring)                                                                    \
	___ARK__TYPE_ALIAS__ (const char*, CString)                                                                        \
	___ARK__TYPE_ALIAS__ (const unsigned char*, UCString)                                                              \
	___ARK__TYPE_ALIAS__ (const wchar_t, WChar_t)                                                                      \
	___ARK__TYPE_ALIAS__ (wchar_t*, wcstring)                                                                          \
	___ARK__TYPE_ALIAS__ (const wchar_t*, WCString)                                                                    \
	___ARK__TYPE_ALIAS__ (oschar_t*, oscstring)                                                                        \
	___ARK__TYPE_ALIAS__ (const oschar_t*, OSCString)

#define ___ARK__TYPE_ALIAS__(C_Primitive_Type_, Alias_)                                                                \
	typedef C_Primitive_Type_		 Alias_;                                                                           \
	typedef C_Primitive_Type_*		 Alias_##_p;                                                                       \
	typedef C_Primitive_Type_* const Alias_##_P;

___ARK__C_PRIMITIVE_TYPES_ALIASES__

/*
 * `<stdint.h>` type aliases
 */
#define ___ARK__STDINT_TYPE_ALIASES__                                                                                  \
	___ARK__STDINT_TYPE_ALIAS__ (8)                                                                                    \
	___ARK__STDINT_TYPE_ALIAS__ (16)                                                                                   \
	___ARK__STDINT_TYPE_ALIAS__ (32)                                                                                   \
	___ARK__STDINT_TYPE_ALIAS__ (64)

#define ___ARK__STDINT_TYPE_ALIAS__(Length_)                                                                           \
	typedef int##Length_##_t			   i##Length_;                                                                 \
	typedef int##Length_##_t*			   i##Length_##_p;                                                             \
	typedef const int##Length_##_t		   I##Length_;                                                                 \
	typedef const int##Length_##_t*		   I##Length_##_p;                                                             \
	typedef const int##Length_##_t* const  I##Length_##_P;                                                             \
	typedef uint##Length_##_t			   u##Length_;                                                                 \
	typedef uint##Length_##_t*			   u##Length_##_p;                                                             \
	typedef const uint##Length_##_t		   U##Length_;                                                                 \
	typedef const uint##Length_##_t*	   U##Length_##_p;                                                             \
	typedef const uint##Length_##_t* const U##Length_##_P;

___ARK__STDINT_TYPE_ALIASES__

#undef ___ARK__STDINT_TYPE_ALIAS__

typedef u64 sz;
typedef U64 SZ;
typedef i64 ssz;
typedef I64 SSZ;

typedef u32 flag_t;

#endif /* ARK_INTERNALS_TYPES_H_ */
