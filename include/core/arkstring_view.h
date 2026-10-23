/*
 * @Copyright (c) 2026 Murilo Gonzalez. All Rights Reserved.
 *
 *                      ARKLIB STRING_VIEW
 */

#ifndef ARK_STRING_VIEW_H_
#define ARK_STRING_VIEW_H_

#import "interfaces/arkstring_interface.h"

ARK_BEGIN

#ifdef ARK_NAMESPACE
	#define string_view		  ark_string_view
	#define class_string_view ark_class_string_view
	#define make_string_view  ark_make_string_view
#endif

ARK_CLASS (ark_string_view) {
	CString _data;
	sz		_len;
};

#define ark_class_string_view(S_, ...)                                         \
	ark_string_view S_ = ark_make_string_view (__VA_ARGS__)

#define ark_string_view_from(S_)                                               \
	({                                                                         \
		auto ___str__ = (S_);                                                  \
		ark_make_string_view (___str__._data, ___str__._len);                  \
	})

ARKAPI __decl_overloadable__ ark_string_view
ark_make_string_view (CString __noalias__ cstr_, sz _len);

ARKAPI __decl_overloadable__ ark_string_view
ark_make_string_view (CString __noalias__ cstr_);

ARK_UTF8_STRING_INTERFACE_PROTOTYPES (ark_string_view)

ARK_END

#endif /* ARK_STRING_VIEW_H_ */
