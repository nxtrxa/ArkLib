/*
 * Boyer-Moore-Horspool (BMH) algorithm.
 */
#ifndef ARKLIB_BMH_H_
#define ARKLIB_BMH_H_

#import "core/arkexpect.h"
#import "core/arkstring_view.h"

ARK_BEGIN

#ifdef ARK_NAMESPACE
	#define string_find_offsets ark_string_find_offsets
#endif

#define ark_string_find_offsets(...) ark_pp_concat (ark_string_find_offsets_, ark_pp_nargs (__VA_ARGS__)) (__VA_ARGS__)

#define ark_string_find_offsets_3(Haystack_, Pattern_, Occs_)                                                          \
	ark_string_find_offsets_fn (                                                                                       \
		$if ($is_struct (Haystack_), ark_string_view_from (Haystack_), Haystack_),                                     \
		$if ($is_struct (Pattern_), ark_string_view_from (Pattern_), Pattern_),                                        \
		Occs_                                                                                                          \
	)

#define ark_string_find_offsets_4(Allocator_, Haystack_, Pattern_, Occs_)                                              \
	ark_string_find_offsets_fn (                                                                                       \
		(Allocator_),                                                                                                  \
		$if ($is_struct (Haystack_), ark_string_view_from (Haystack_), Haystack_),                                     \
		$if ($is_struct (Pattern_), ark_string_view_from (Pattern_), Pattern_),                                        \
		Occs_                                                                                                          \
	)

ARK_DECL_FAULTS (ARK_BMH, INVALID_PATTERN, ZERO_OCCURENCES);

typedef ark_darray (u32) offsets_t;

ARKAPI __decl_overloadable__ ark_expect (ARK_BMH, offsets_t) ark_string_find_offsets_fn (
	ark_allocator allocator_, ark_string_view haystack_, ark_string_view pattern_, u32* __noalias__ occs_
) __attr_noexcept__;

ARKAPI __decl_overloadable__ __decl_flatten__ ark_expect (ARK_BMH, offsets_t) ark_string_find_offsets_fn (
	ark_string_view haystack_, ark_string_view pattern_, u32* __noalias__ occs_
) __attr_noexcept__;

// ARKAPI __decl_overloadable__ ark_darray_u32
// ark_get_offsets (CString txt_, CString pattern_, u32* __noalias__ occs_) __attr_noexcept__;


ARK_END

#endif /* ARKLIB_BMH_H_ */
