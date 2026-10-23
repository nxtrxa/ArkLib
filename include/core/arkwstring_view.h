/*
 * @Copyright (c) 2026 Murilo Gonzalez. All Rights Reserved.
 *
 *                      ARKLIB WIDE STRING VIEW
 */

#ifndef ARK_WSTRING_VIEW_H_
#define ARK_WSTRING_VIEW_H_

#import "interfaces/arkstring_interface.h"

ARK_BEGIN

#ifdef ARK_NAMESPACE
	#define wstring_view	   ark_wstring_view
	#define class_wstring_view ark_class_wstring_view
	#define make_wstring_view  ark_make_wstring_view
#endif

ARK_CLASS (ark_wstring_view) {
	WCString data;
	sz		 len;
};

#define ark_class_wstring_view(Sv_, ...) ark_wstring_view Sv_ = ark_make_wstring_view (__VA_ARGS__)

#define ark_wstring_view_from(Str_) ark_make_wstring_view ((Str_).data, (Str_).len)

ARKAPI __decl_overloadable__ ark_wstring_view ark_make_wstring_view (WCString __noalias__ cstr_, sz len);

ARKAPI __decl_overloadable__ ark_wstring_view ark_make_wstring_view (WCString __noalias__ cstr_);

ARK_END

#endif /* ARK_WSTRING_VIEW_H_ */
