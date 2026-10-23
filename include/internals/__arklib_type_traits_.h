#ifndef ARK_INTERNALS_TYPE_TRAITS_H_
#define ARK_INTERNALS_TYPE_TRAITS_H_

#import "__arklib_features_.h"

ARK_BEGIN

#ifdef ARK_NAMESPACE
	#define CLASS		   ARK_CLASS
	#define INTERNAL_CLASS ARK_INTERNAL_CLASS
	#define INTERFACE	   ARK_INTERFACE
	#define ENUM		   ARK_ENUM
	#define UNION		   ARK_UNION
#endif

/* Type Class Identifiers matching __builtin_classify_type */
enum {
	ARK_INTEGER_TYPE_CLASS = 1,
#define $integer_type ARK_INTEGER_TYPE_CLASS

	ARK_ENUM_TYPE_CLASS = 3,
#define $enum_type ARK_ENUM_TYPE_CLASS

	ARK_BOOL_TYPE_CLASS = 4,
#define $bool_type ARK_BOOL_TYPE_CLASS

	ARK_POINTER_TYPE_CLASS = 5,
#define $pointer_type ARK_POINTER_TYPE_CLASS

	ARK_REAL_TYPE_CLASS = 8,
#define $float_type ARK_REAL_TYPE_CLASS

	ARK_COMPLEX_TYPE_CLASS = 9,
#define $complex_type ARK_COMPLEX_TYPE_CLASS

	ARK_STRUCT_TYPE_CLASS = 12,
#define $struct_type ARK_STRUCT_TYPE_CLASS

	ARK_UNION_TYPE_CLASS = 13,
#define $union_type ARK_UNION_TYPE_CLASS

	ARK_BITINT_TYPE_CLASS = 18,
#define $bitint_type ARK_BITINT_TYPE_CLASS

	ARK_VECTOR_TYPE_CLASS = 19,
#define $vector_type ARK_VECTOR_TYPE_CLASS
};

/* Trait Evaluation Predicates */
#define $typematch(T_, U_) ARK_TYPES_COMPATIBLE_P (typeof_unqual (T_), typeof_unqual (U_))
#define $typeclass(Type_)  ARK_CLASSIFY_TYPE (Type_)

#define $is_bool(Expr_)				 ($typeclass (Expr_) == $bool_type)
#define $is_primitive_integer(Expr_) ($typeclass (Expr_) == $integer_type)
#define $is_float(Expr_)			 ($typeclass (Expr_) == $float_type)
#define $is_complex(Expr_)			 ($typeclass (Expr_) == $complex_type)
#define $is_bitint(Expr_)			 ($typeclass (Expr_) == $bitint_type)
#define $is_pointer(Expr_)			 ($typeclass (Expr_) == $pointer_type)
#define $is_enum(Expr_)				 ($typeclass (Expr_) == $enum_type)
#define $is_union(Expr_)			 ($typeclass (Expr_) == $union_type)
#define $is_struct(Expr_)			 ($typeclass (Expr_) == $struct_type)
#define $is_vector(Expr_)			 ($typeclass (Expr_) == $vector_type)

#define $is_integer(Expr_)	($is_primitive_integer (Expr_) || $is_bitint (Expr_))
#define $is_real(Expr_)		($is_float (Expr_) || $is_integer (Expr_))
#define $is_integral(Expr_) ($is_integer (Expr_) || $is_bool (Expr_))

#define $is_c_array(Expr_)	 (ARK_IS_C_ARRAY (Expr_))
#define $is_constexpr(Expr_) (ARK_IS_CONSTEXPR (Expr_))

/* Structural & Nominal Construction Templates */
#define ARK_CLASS(Name_, ...)                                                                                          \
	typedef struct ark_pp_concat (class_, Name_) * ark_pp_concat (Name_, _p);                                          \
	typedef struct ark_pp_concat (class_, Name_) Name_;                                                                \
	struct __VA_ARGS__ ark_pp_concat (class_, Name_)

#define ARK_INTERNAL_CLASS(Name_, ...)                                                                                 \
	typedef struct ark_pp_concat (___ark__internal_class_, Name_, __)                                                  \
		* ark_pp_concat (ark_internal_symbol (Name_), p);                                                              \
	typedef struct ark_pp_concat (___ark__internal_class_, Name_, __) ark_internal_symbol (Name_);                     \
	struct __VA_ARGS__ ark_pp_concat (___ark__internal_class_, Name_, __)

#define ARK_INTERFACE(Name_, ...)                                                                                      \
	typedef struct ark_pp_concat (interface_, Name_) Name_;                                                            \
	struct __VA_ARGS__ ark_pp_concat (interface_, Name_)

#define ARK_ENUM(Name_, ...)                                                                                           \
	ARKLIB_PRAGMA_PUSH                                                                                                 \
	ARKLIB_PRAGMA_IGNORE ("-Wpedantic")                                                                                \
	typedef enum ark_pp_concat (enum_, Name_) Name_;                                                                   \
	ARKLIB_PRAGMA_POP                                                                                                  \
	enum __VA_ARGS__ ark_pp_concat (enum_, Name_)

#define ARK_UNION(Name_, ...)                                                                                          \
	typedef union ark_pp_concat (union_, Name_) Name_;                                                                 \
	union ark_pp_concat (union_, Name_) __VA_ARGS__

ARK_END

#endif /* ARK_INTERNALS_TYPE_TRAITS_H_ */
