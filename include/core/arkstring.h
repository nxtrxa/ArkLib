/*
 * @Copyright (c) 2026 Murilo Gonzalez. All Rights Reserved.
 *
 *                          ARKLIB STRING
 *
 */

#ifndef ARK_CORE_STRING_H_
#define ARK_CORE_STRING_H_

#import "interfaces/arkstring_interface.h"

#ifdef ARK_NAMESPACE
	#define string			ark_string
	#define class_string	ark_class_string
	#define make_string		ark_make_string
	#define destroy_string	ark_destroy_string
	#define destroy_strings ark_destroy_strings
	#define string_from		ark_string_from
	#define strcopy			ark_strcopy
	#define strlchop		ark_strlchop
	#define strrchop		ark_strrchop
	#define strtrim			ark_strtrim
	#define strltrim		ark_strltrim
	#define strrtrim		ark_strrtrim
	#define strreserve		ark_strreserve
	#define strlappend		ark_strlappend
	#define strrappend		ark_strrappend
	#define strcomp			ark_strcomp
	#define strconcat		ark_strconcat
	#define strtolower		ark_strtolower
	#define strtoupper		ark_strtoupper
	#define strsplit		ark_strsplit
	#define strsplit_at		ark_strsplit_at
#endif

ARK_BEGIN

ARK_CLASS (ark_bstring);
ARK_CLASS (ark_string_view);

ARK_CLASS (ark_string) {
	cstring		  _data;
	sz			  _len; /* exclude null terminator */
	sz			  _capacity;
	ark_allocator _allocator;
};

#define ark_class_string(Str_, ...)                                            \
	__attr_cleanup__ (ark_destroy_string_fn)                                   \
		ark_string Str_ = ark_make_string (__VA_ARGS__)

#define ___ARK__DESTROY_STRING__(StrRef_) ark_destroy_string_fn (StrRef_);
#define ark_destroy_string(...)                                                \
	ark_pp_foreach (___ARK__DESTROY_STRING__, __VA_ARGS__)

#define ark_string_from(...)                                                   \
	ark_pp_concat (ark_string_from_, ark_pp_nargs (__VA_ARGS__)) (__VA_ARGS__)

#define ark_string_from_1(S_) ark_make_string ((S_)._data, (S_)._len)

#define ark_string_from_2(Allocator_, S_)                                      \
	ark_make_string ((Allocator_), (S_)._data, (S_)._len)

ARKAPI __decl_overloadable__ ark_string ark_make_string (
	ark_allocator allocator_, CString __noalias__ cstr_, sz len_, sz cap_
);

ARKAPI __decl_overloadable__ ark_string
ark_make_string (ark_allocator allocator_, CString __noalias__ cstr_, sz len_);

ARKAPI __decl_overloadable__ ark_string
ark_make_string (CString __noalias__ cstr_, sz len_, sz cap_);

ARKAPI __decl_overloadable__ ark_string
ark_make_string (ark_allocator allocator_, CString __noalias__ cstr_);

ARKAPI __decl_overloadable__ ark_string
ark_make_string (CString __noalias__ cstr_, sz len_);

ARKAPI __decl_overloadable__ ark_string
ark_make_string (CString __noalias__ cstr_);

ARKAPI u0 ark_destroy_string_fn (ark_string* __noalias__ str_);

__decl_static_inline__ u0 ark_string_cleanup (ark_string* str_) {
	return ark_destroy_string_fn (str_);
}

// ARKAPI cstring ark_strtrunc (ark_string* __noalias__ str_);

ARKAPI __decl_overloadable__ u0
ark_strcopy (ark_string* __noalias__ dest_, CString __noalias__ src_);

ARKAPI __decl_overloadable__ u0
ark_strcopy (ark_string* __noalias__ dest_, ark_string src_);

ARKAPI __decl_overloadable__ u0
ark_strcopy (ark_string* __noalias__ dest_, ark_string_view src_);


ARKEXPORT int ark_strreserve (ark_string* __noalias__ str_, sz expec_cap_);

/*
 * Left append every SRC(N) onto DEST.
 *
 * Note(s):
 * 1. Every SRC(N) must strictly have the same type.
 * 2. Return -1 on failure
 */
// ARKAPI __decl_overloadable__ int ark_strlappend (ark_string* __noalias__
// dest_, ark_string src1_, ...);
//
// ARKAPI __decl_overloadable__ int ark_strlappend (ark_string* __noalias__
// dest_, ark_string_view src1_, ...);
//
// ARKAPI __decl_overloadable__ int ark_strlappend (ark_string* __noalias__
// dest_, CString src1_, ...);
//
// ARKAPI __decl_overloadable__ int ark_strrappend (ark_string* __noalias__
// dest_, Char src1_, ...);

/*
 * Right append every SRC(N) onto DEST.
 *
 * Note(s):
 * 1. Every SRC(N) must strictly have the same type.
 * 2. Return -1 on failure
 */
ARKAPI __decl_overloadable__ u0
ark_strrappend (ark_string* __noalias__ str_, ark_string src_);

ARKAPI __decl_overloadable__ u0
ark_strrappend (ark_string* __noalias__ str_, ark_string_view src_);

ARKAPI __decl_overloadable__ u0
ark_strrappend (ark_string* __noalias__ str_, CString __noalias__ src_);

ARKAPI __decl_overloadable__ u0
ark_strrappend (ark_string* __noalias__ str_, Char src_);

ARK_UTF8_STRING_INTERFACE_PROTOTYPES (ark_string)

ARK_END

#endif /* ARK_CORE_STRING_H_ */
