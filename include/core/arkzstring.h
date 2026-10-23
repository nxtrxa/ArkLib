#ifndef ARK_CORE_ZSTRING_H_
#define ARK_CORE_ZSTRING_H_

#import "interfaces/arkstring_interface.h"

ARK_BEGIN

#ifdef ARK_NAMESPACE
#endif

ARK_INTERFACE (ark_allocator);

ARK_CLASS (ark_zstring) {
	cstring		  data;
	sz			  len;
	sz			  capacity;
	ark_allocator allocator;
};

#define ark_class_zstring(ZStr_, ...) __attr_cleanup__ (ark_destroy_zstring) ZStr_ = ark_make_zstring (__VA_ARGS__)

#define ark_zstring_from(...) ark_pp_concat (ark_zstring_from_, ark_pp_nargs (__VA_ARGS__)) (__VA_ARGS__)

#define ark_zstring_from_1(Str_) ark_make_zstring ((Str_).data, (Str_).len)

#define ark_zstring_from_2(Allocator_, Str_) ark_make_zstring ((Allocator_), (Str_).data, (Str_).len)

ARKAPI
__decl_overloadable__ ark_zstring ark_make_zstring (ark_allocator allocator_, OSCString cstr_, sz len_, sz cap_);

ARKAPI __decl_overloadable__ ark_zstring
ark_make_zstring (ark_allocator allocator_, CString __noalias__ cstr_, sz len_);

ARKAPI __decl_overloadable__ ark_zstring ark_make_zstring (ark_allocator allocator_, CString __noalias__ cstr_);

ARKAPI __decl_overloadable__ ark_zstring ark_make_zstring (CString __noalias__ cstr_, sz len_, sz cap_);

ARKAPI __decl_overloadable__ ark_zstring ark_make_zstring (CString __noalias__ cstr_, sz len_);

ARKAPI __decl_overloadable__ ark_zstring ark_make_zstring (CString __noalias__ cstr_);

ARKAPI u0 ark_destroy_zstring_fn (ark_zstring* zstr_);

ARK_END

#endif /* ARK_CORE_ZSTRING_H_ */
