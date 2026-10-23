#import "algorithms/bmh.h"
#import "collections/arkarray.h"
#import "collections/arkdarray.h"
#import "core/arkstring.h"

ARK_BEGIN

ARKEXPORT __decl_overloadable__
ark_expect (ARK_BMH, offsets_t) ark_string_find_offsets_fn (
	ark_allocator	 allocator_,
	ark_string_view	 haystack_,
	ark_string_view	 pattern_,
	u32* __noalias__ occs_
) {
	if (pattern_._len > haystack_._len || pattern_._len == 0)
		return ark_unexpected (ARK_BMH, INVALID_PATTERN);
	ark_darray (u32) offsets = ark_make_darray (u32, allocator_, 4).unwrap;
	ucstring haystack		 = (ucstring)haystack_._data;
	ucstring pattern		 = (ucstring)pattern_._data;
	ark_class_array (u32, 256, bmt);
	$foreach (e $in_array (bmt)) {
		*e = pattern_._len;
	}
	$for (i, pattern_._len - 1) {
		ark_array_set (bmt, pattern[i], pattern_._len - i - 1);
	}

	u32 occ_count = 0;
	u32 i		  = 0;
	while (i <= haystack_._len - pattern_._len) {
		i32 j = pattern_._len - 1;
		while (j >= 0 && pattern[j] == haystack[i + j])
			j--;
		if (j == -1) {
			ark_darray_push_back (offsets, i);
			occ_count++;
			i += pattern_._len;
		} else {
			u32 shift  = ark_array_at (bmt, haystack[i + pattern_._len - 1]);
			i		  += shift;
		}
	}
	*occs_ = occ_count;
	if (occ_count == 0) {
		ark_destroy_darray (offsets);
		return ark_unexpected (ARK_BMH, ZERO_OCCURENCES);
	}
	return ark_expected (ARK_BMH, offsets);
}

ARKAPI __decl_overloadable__
ark_expect (ARK_BMH, offsets_t) ark_string_find_offsets_fn (
	ark_string_view haystack_, ark_string_view pattern_, u32* __noalias__ occs_
) {
	return ark_string_find_offsets_fn (
		ARK_LIBC_ALLOCATOR, haystack_, pattern_, occs_
	);
}

ARK_END
