#ifndef ARK_CORE_ASSOC_ENUM_H_
#define ARK_CORE_ASSOC_ENUM_H_

#import "arkdefs.h"

ARK_BEGIN

#ifdef ARK_NAMESPACE
	#define assoc_enum			ark_assoc_enum
	#define make_assoc_enum		ark_make_assoc_enum
	#define assoc_enum_valuesof ark_assoc_enum_valuesof
	#define assoc_enum_asvalsof ark_assoc_enum_asvalsof
	#define assoc_enum_nameof	ark_assoc_enum_nameof
	#define assoc_enum_asvalof	ark_assoc_enum_asvalof
#endif

#define ___get_value__(X_, ...) X_,
#define __get_value_(X_)		___get_value__ X_

#define __decl_enum_seq_(...) ark_pp_foreach (__get_value_, __VA_ARGS__)

#define ___value__(X_, ...) [X_] = #X_,
#define __value_(X_)		___value__ X_

#define ___enum_assoc__(X_, Y_) [X_] = Y_,
#define __assoc_(X_)			___enum_assoc__ X_

#define ark_assoc_enum(AS_, AssocType_, ...)                                                                           \
	enum ark_pp_concat (AS_, _e) {                                                                                     \
		__decl_enum_seq_ (__VA_ARGS__) ark_pp_concat (AS_, _count),                                                    \
	};                                                                                                                 \
	__decl_unused__ static CString ark_pp_concat (AS_, _values_table)[] = {ark_pp_foreach (__value_, __VA_ARGS__)};    \
	__decl_unused__ static const AssocType_ ark_pp_concat (AS_, _assoc_table)[]                                        \
		= {ark_pp_foreach (__assoc_, __VA_ARGS__)};                                                                    \
	struct {                                                                                                           \
		CString* const	  valuesof;                                                                                    \
		AssocType_* const assoc_valuesof;                                                                              \
		CString const	  nameof;                                                                                      \
	} ark_pp_concat (AS_, _meta) = {                                                                                   \
		.valuesof = ark_pp_concat (AS_, _values_table),                                                                \
		.nameof	  = ark_pp_stringfy (AS_),                                                                             \
	};                                                                                                                 \
	typedef struct {                                                                                                   \
		enum ark_pp_concat (AS_, _e) value;                                                                            \
		AssocType_ assoc;                                                                                              \
	} AS_

#define ark_assoc_enum_valuesof(AS_)		ark_pp_concat (AS_, _meta).valuesof
#define ark_assoc_enum_asvalsof(AS_)		ark_pp_concat (AS_, _meta).assoc_valuesof
#define ark_assoc_enum_nameof(AS_)			ark_pp_concat (AS_, _meta).nameof
#define ark_assoc_enum_asvalof(AS_, Value_) ark_pp_concat (AS_, _assoc_table)[Value_]

#define ark_make_assoc_enum(AS_, Value_)                                                                               \
	({                                                                                                                 \
		$static_assert (ark_pp_concat (AS_, _count) > (Value_), "VALUE exceeds enum count!");                          \
		if (ARK_STRCMP (#Value_, ark_assoc_enum_valuesof (AS_)[Value_]))                                               \
			ARK_PANIC ("Value " #Value_ " is not part of enum " #AS_);                                                 \
		(AS_) {                                                                                                        \
			.value = (Value_),                                                                                         \
			.assoc = ark_assoc_enum_asvalof (AS_, Value_),                                                             \
		};                                                                                                             \
	})

ARK_END

#endif /* ARK_CORE_ASSOC_ENUM_H_ */
