#ifndef ARK_CORE_WSTRING_H_
#define ARK_CORE_WSTRING_H_

#import "interfaces/arkstring_interface.h"

ARK_BEGIN

#ifdef ARK_NAMESPACE
#endif

ARK_INTERFACE (ark_allocator);

ARK_CLASS (ark_wstring) {
	wcstring	  data;
	sz			  len;
	sz			  capacity;
	ark_allocator allocator;
};

#define ark_class_wstring(ZStr_, ...) __attr_cleanup__ (ark_destroy_wstring) ZStr_ = ark_make_wstring (__VA_ARGS__)

#define ark_wstring_from(...) ark_pp_concat (ark_wstring_from_, ark_pp_nargs (__VA_ARGS__)) (__VA_ARGS__)

#define ark_wstring_from_1(Str_) ark_make_wstring ((Str_).data, (Str_).len)

#define ark_wstring_from_2(Allocator_, Str_) ark_make_wstring ((Allocator_), (Str_).data, (Str_).len)

ARKAPI
__decl_overloadable__ ark_wstring ark_make_wstring (ark_allocator allocator_, OSCString cstr_, sz len_, sz cap_);

ARKAPI __decl_overloadable__ ark_wstring
ark_make_wstring (ark_allocator allocator_, WCString __noalias__ cstr_, sz len_);

ARKAPI __decl_overloadable__ ark_wstring ark_make_wstring (ark_allocator allocator_, WCString __noalias__ cstr_);

ARKAPI __decl_overloadable__ ark_wstring ark_make_wstring (WCString __noalias__ cstr_, sz len_, sz cap_);

ARKAPI __decl_overloadable__ ark_wstring ark_make_wstring (WCString __noalias__ cstr_, sz len_);

ARKAPI __decl_overloadable__ ark_wstring ark_make_wstring (WCString __noalias__ cstr_);

ARKAPI u0 ark_destroy_wstring (ark_wstring* zstr_);

ARK_END

#endif /* ARK_CORE_WSTRING_H_ */
