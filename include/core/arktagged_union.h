#ifndef ARK_CORE_TAGGED_UNION_H_
#define ARK_CORE_TAGGED_UNION_H_

#include "arkassoc_enum.h"
#include "arkdefs.h"
#include "ctype.h"

#define ___decl_tag__(TypeName_, Type_) ark_pp_concat (TypeName_, _is_, Type_)
#define __get_type_(Type_, ...)			Type_
#define __decl_tag_(TypeName_, X_)		___decl_tag__ (TypeName_, __get_type_ X_)
#define __expand_tag_(X_)				__decl_tag_ X_,
#define __wrap_tag_(TypeName_, X_)		__expand_tag_ ((TypeName_, X_))
#define __decl_tag_seq_(TypeName_, ...) ark_pp_arg_foreach (__wrap_tag_, TypeName_, __VA_ARGS__)

#define ___decl_field__(Type_, Field_) Type_ Field_;
#define __decl_field_(X_)			   ___decl_field__ X_
#define __decl_fields_(...)			   ark_pp_foreach (__decl_field_, __VA_ARGS__)

#define ark_tagged_union_t(TypeName_) struct TypeName_

#define ark_tagged_union(TypeName_, ...)                                                                               \
	struct TypeName_ {                                                                                                 \
		union {                                                                                                        \
			__decl_fields_ (__VA_ARGS__)                                                                               \
		};                                                                                                             \
		enum {                                                                                                         \
			__decl_tag_seq_ (TypeName_, __VA_ARGS__) ark_pp_concat (Typename_, _count),                                \
		} _tag;                                                                                                        \
	}

#define ark_make_tagged_union(TypeName_, FieldType_, ...)                                                              \
	({                                                                                                                 \
		(ark_tagged_union_t (TypeName_)) {                                                                             \
			._tag = ___decl_tag__ (TypeName_, FieldType_),                                                             \
			.__VA_ARGS__,                                                                                              \
		};                                                                                                             \
	})

#endif /* ARK_CORE_TAGGED_UNION_H_ */
