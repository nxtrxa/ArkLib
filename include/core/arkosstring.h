#ifndef ARK_CORE_OSSTRING_H_
#define ARK_CORE_OSSTRING_H_

#import "interfaces/arkstring_interface.h"

ARK_BEGIN

#ifndef ARK_NAMESPACE
	#define osstring ark_ostring

#endif

ARK_INTERFACE (ark_allocator);
ARK_CLASS (ark_string);

ARK_CLASS (ark_osstring) {
	oscstring	  data;
	sz			  len;
	sz			  capacity;
	ark_allocator allocator;
};

#define ark_osstring_from(...) ark_pp_concat (ark_osstring_from_, ark_pp_nargs (__VA_ARGS__)) (__VA_ARGS__)

#define ark_osstring_from_1(S_) ark_make_osstring ((S_).data, (S_).len)

#define ark_osstring_from_2(Allocator_, S_) ark_make_osstring ((Allocator_), (S_).data, (S_).len)

ARKAPI
__decl_overloadable__ ark_osstring ark_make_osstring (ark_allocator allocator_, OSCString cstr_, sz len_, sz cap_);

ARKAPI __decl_overloadable__ ark_osstring ark_make_osstring (ark_allocator allocator_, OSCString cstr_, sz len_);

ARKAPI __decl_overloadable__ ark_osstring ark_make_osstring (ark_allocator allocator_, OSCString cstr_);

ARKAPI __decl_overloadable__ ark_osstring ark_make_osstring (OSCString cstr_, sz len_, sz cap_);

ARKAPI __decl_overloadable__ ark_osstring ark_make_osstring (OSCString cstr_, sz len_);

ARKAPI __decl_overloadable__ ark_osstring ark_make_osstring (OSCString cstr);

ARKAPI __decl_overloadable__ ark_string ark_osstr_to_utf8 (ark_allocator allocator_, ark_osstring osstr_);

ARKAPI __decl_overloadable__ ark_string ark_osstr_to_utf8 (ark_osstring osstr_);

ARKAPI __decl_overloadable__ ark_osstring ark_utf8_to_osstr (ark_allocator allocator_, ark_string utf8_);

ARKAPI __decl_overloadable__ ark_osstring ark_utf8_to_osstr (ark_string utf8_);

ARKAPI u0 ark_destroy_osstring (ark_osstring* osstr_);

ARK_END

#endif /* ARK_CORE_OSSTRING_H_ */
