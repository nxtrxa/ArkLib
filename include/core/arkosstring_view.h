/*
 * @Copyright (c) 2026 Murilo Gonzalez. All Rights Reserved.
 *
 *          ARKLIB OPERATION SYSTEM STRING VIEW
 */

#ifndef ARK_OSSTRING_VIEW_H_
#define ARK_OSSTRING_VIEW_H_

#import "interfaces/arkstring_interface.h"

ARK_BEGIN

#ifdef ARK_NAMESPACE
	#define osstring_view		ark_osstring_view
	#define class_osstring_view ark_class_osstring_view
	#define make_osstring_view	ark_make_osstring_view
#endif

ARK_CLASS (ark_osstring_view) {
	WCString data;
	sz		 len;
};

#define ark_class_osstring_view(S_, ...) ark_osstring_view S_ = ark_make_osstring_view (__VA_ARGS__)

#define ark_osstring_view_from(S_) ark_make_osstring_view ((S_).data, (S_).len)

ARKAPI __decl_overloadable__ ark_osstring_view ark_make_osstring_view (WCString __noalias__ wcstr_, sz len);

ARKAPI __decl_overloadable__ ark_osstring_view ark_make_osstring_view (WCString __noalias__ wcstr_);

ARK_END

#endif /* ARK_OSSTRING_VIEW_H_ */
