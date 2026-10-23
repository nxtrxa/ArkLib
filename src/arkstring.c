#if 0
	#import <ctype.h>
	#import <string.h>

	#import "algorithms/bmh.h"
	#import "allocators/arkarena.h"
	#import "allocators/arkmonotonic_buffer.h"
	#import "core/arkbasic_string.h"
	#import "core/arkstring.h"

ARK_BEGIN

/*
 * INTERNAL API IMPLEMENTATION
 */

static constexpr usz STRING_INITIAL_CAPACITY = 16;

static constexpr f64 STRING_GROW_FACTOR = 2.0f;

static constexpr f64 STRING_SHRINK_FACTOR = 0.5f;

static constexpr f64 STRING_SHRINK_THRESHOLD = 0.25;

	#define ARK_STRGROW(Str_)	((Str_)->capacity * STRING_GROW_FACTOR)
	#define ARK_STRSHRINK(Str_) ((Str_)->capacity * STRING_SHRINK_FACTOR)

ARKINTERNAL cstring stradup (ark_allocator* __noalias__ allocator_, CString __noalias__ cstr_, usz cap) {
	size_t len = ARK_STRLEN (cstr_);
	if (len + 1 > cap) cap = len;
	cstring str_ = ark_allocator_alloc (allocator_, cap, 1);
	ARK_MEMCPY (str_, cstr_, len);
	str_[len] = '\0';
	return str_;
}

ARKINTERNAL ark_string strvconcat_str (ark_allocator* __noalias__ allocator_,
									   ark_string				  s1_,
									   ark_string				  s2_,
									   va_list					  args_) {
	ark_monotonic_buffer (mbuff);
	ark_class_darray (ss, ark_string, ARK_MONOTONIC_BUFFER_ALLOCATOR (mbuff), 0);
	ark_string va		= va_arg (args_, ark_string);
	usz		   buff_len = s1_.len + s2_.len + 1;
	while (va.data) {
		ark_darray_push_back (ss, va);
		buff_len += va.len;
		va		  = va_arg (args_, ark_string);
	}

	usz		offset = s1_.len + s2_.len;
	cstring buff   = ark_monotonic_buffer_alloc (mbuff, buff_len, 1);

	snprintf (buff, buff_len, ark_strfmt ark_strfmt, ark_strarg (s1_), ark_strarg (s2_));
	if (ss->size) {
		$foreach (s $in_darray (ss)) {
			snprintf (buff + offset, buff_len - offset, ark_strfmt, ark_strarg (*s));
			offset += s->len;
		}
	}

	return ark_make_string (allocator_, buff, buff_len);
}

ARKINTERNAL ark_string strvconcat_sv (ark_allocator* __noalias__ allocator_,
									  ark_string				 s1_,
									  ark_string_view			 s2_,
									  va_list					 args_) {
	ark_class_arena (arena);
	ark_class_darray (ss, ark_string_view, ARK_ARENA_ALLOCATOR (arena), 0);
	ark_string_view va		 = va_arg (args_, ark_string_view);
	usz				buff_len = s1_.len + s2_.len + 1;
	while (va.data) {
		ark_darray_push_back (ss, va);
		buff_len += va.len;
		va		  = va_arg (args_, ark_string_view);
	}

	usz		offset = s1_.len + s2_.len;
	cstring buff   = ark_arena_alloc (arena, buff_len, 1);

	snprintf (buff, buff_len, ark_strfmt ark_strfmt, ark_strarg (s1_), ark_strarg (s2_));
	if (ss) {
		$foreach (s $in_darray (ss)) {
			snprintf (buff + offset, buff_len - offset, ark_strfmt, ark_strarg (*s));
			offset += s->len;
		}
	}

	return ark_make_string (allocator_, buff, buff_len);
}

ARKINTERNAL ark_string strvconcat_cstr (ark_allocator* __noalias__ allocator_,
										ark_string				   s1_,
										CString __noalias__		   s2_,
										va_list					   args_) {
	ark_monotonic_buffer (mbuff);
	ark_class_darray (ss, ark_string_view, ARK_MONOTONIC_BUFFER_ALLOCATOR (mbuff), 2);

	ark_string_view s1 = ark_make_string_view (s1_.data, s1_.len);
	ark_string_view s2 = ark_make_string_view (s2_);
	ark_darray_vpush_back (ss, s1, s2);

	usz				buff_len = s1.len + s2.len + 1;
	ark_string_view nth_str;
	CString			arg = va_arg (args_, CString);
	while (arg) {
		nth_str	  = ark_make_string_view (arg);
		buff_len += nth_str.len;
		ark_darray_vpush_back (ss, nth_str);
		arg = va_arg (args_, CString);
	}

	cstring buff = ark_monotonic_buffer_alloc (mbuff, buff_len, 1);

	usz offset = 0;
	$foreach (s $in_darray (ss)) {
		snprintf (buff + offset, buff_len - offset, ark_strfmt, ark_strarg (*s));
		offset += s->len;
	}

	return ark_make_string (allocator_, buff, buff_len - 1);
}


/*
 * PUBLIC API IMPLEMENTATION
 */

ARKEXPORT __decl_overloadable__ ark_string ark_make_string (ark_allocator* __noalias__ allocator_,
															CString __noalias__		   cstr_,
															usz						   len_,
															usz						   cap_) {
	usz capacity = (cap_ > len_) ? cap_ : len_ + 1;
	return (ark_string) {
		.data	   = stradup (allocator_, cstr_, capacity),
		.len	   = len_,
		.capacity  = capacity,
		.allocator = ark_allocator_dup (allocator_),
	};
}

ARKEXPORT __decl_overloadable__ ark_string ark_make_string (ark_allocator* __noalias__ allocator_,
															CString __noalias__		   cstr_,
															usz						   len_) {
	return ark_make_string (allocator_, cstr_, len_, STRING_INITIAL_CAPACITY);
}

ARKEXPORT __decl_overloadable__ ark_string ark_make_string (ark_allocator* __noalias__ allocator_,
															CString __noalias__		   cstr_) {
	return ark_make_string (allocator_, cstr_, (usz)ARK_STRLEN (cstr_), STRING_INITIAL_CAPACITY);
}

ARKEXPORT __decl_overloadable__ ark_string ark_make_string (CString __noalias__ cstr_, usz len_, usz cap_) {
	return ark_make_string (ARK_LIBC_ALLOCATOR, cstr_, len_, cap_);
}

ARKEXPORT __decl_overloadable__ ark_string ark_make_string (CString __noalias__ cstr_, usz len_) {
	return ark_make_string (ARK_LIBC_ALLOCATOR, cstr_, len_, STRING_INITIAL_CAPACITY);
}


ARKEXPORT __decl_overloadable__ ark_string ark_make_string (CString __noalias__ cstr_) {
	return ark_make_string (ARK_LIBC_ALLOCATOR, cstr_, (usz)ARK_STRLEN (cstr_), STRING_INITIAL_CAPACITY);
}

ARKEXPORT u0 ark_destroy_string (ark_string* __noalias__ str_) {
	ark_allocator_dealloc (str_->allocator, str_->data);
	if (str_->allocator->type == ARK_DYNAMIC_ALLOCATOR) ark_dealloc (str_->allocator);
}


ARKEXPORT __decl_overloadable__ ark_string ark_string_from (ark_string_view sv_) {
	return ark_make_string (ARK_LIBC_ALLOCATOR, sv_.data, sv_.len, STRING_INITIAL_CAPACITY);
}

ARKEXPORT __decl_overloadable__ ark_string ark_string_from (ark_bstring sv_) {
	return ark_make_string (ARK_LIBC_ALLOCATOR, sv_.data, sv_.len, STRING_INITIAL_CAPACITY);
}

ARKEXPORT __decl_overloadable__ ark_string ark_string_from (ark_allocator* __noalias__ allocator_,
															ark_string_view			   sv_) {
	return ark_make_string (allocator_, sv_.data, sv_.len, STRING_INITIAL_CAPACITY);
}

ARKEXPORT __decl_overloadable__ ark_string ark_string_from (ark_allocator* __noalias__ allocator_, ark_bstring sv_) {
	return ark_make_string (allocator_, sv_.data, sv_.len, STRING_INITIAL_CAPACITY);
}

// ARKEXPORT cstring ark_strtrunc (ark_string* __noalias__ str_) {
// 	if (!str_->data) return nullptr;
//
// 	ark_strrappend (str_, (Char)'\0');
// 	return str_->data;
// }

ARKEXPORT __decl_overloadable__ u0 ark_strcopy (ark_string* __noalias__ dest_, CString __noalias__ src_) {
	ark_destroy_string (dest_);
	*dest_ = ark_make_string (src_);
}

ARKEXPORT __decl_overloadable__ u0 ark_strcopy (ark_string* __noalias__ dest_, ark_string src_) {
	ark_string cp = ark_make_string (dest_->allocator, src_.data, src_.len, src_.capacity);
	ark_destroy_string (dest_);
	*dest_ = cp;
}

ARKEXPORT __decl_overloadable__ u0 ark_strcopy (ark_string* __noalias__ dest_, ark_string_view src_) {
	ark_destroy_string (dest_);
	*dest_ = ark_string_from (src_);
}

ARKEXPORT u0 ark_strlchop (ark_string* __noalias__ str_, u8 n) {
	if (str_->len < n) n = str_->len;
	str_->data += n;
	str_->len  -= n;
}

ARKEXPORT u0 ark_strrchop (ark_string* __noalias__ str_, u8 n) {
	if (str_->len < n) n = str_->len;
	str_->len -= n;
}

ARKEXPORT u0 ark_strtrim (ark_string* __noalias__ str_) {
	ark_strltrim (str_);
	ark_strrtrim (str_);
}
ARKEXPORT u0 ark_strltrim (ark_string* __noalias__ str_) {
	while (str_->len > 0 && isspace ((uchar)*str_->data)) {
		ark_strlchop (str_, 1);
	}
}
ARKEXPORT u0 ark_strrtrim (ark_string* __noalias__ str_) {
	while (str_->len > 0 && isspace ((uchar) * (str_->data + str_->len - 1))) {
		ark_strrchop (str_, 1);
	}
}

ARKEXPORT int ark_strreserve (ark_string* __noalias__ str_, usz expec_cap_) {
	const usz old_alloc_sz = str_->capacity;
	ark_allocator_realloc (str_->allocator, str_->data, old_alloc_sz, expec_cap_, 1);
	if (!str_->data) {
		ARK_WARN ("realloc failed");
		return -1;
	}
	str_->capacity = expec_cap_;
	return 0;
}

ARKEXPORT __decl_overloadable__ u0 ark_strrappend (ark_string* __noalias__ str_, CString __noalias__ src_) {
	ark_string_view src	 = ark_make_string_view (src_);
	str_->len			+= src.len;
	if (str_->len >= str_->capacity) {
		if (ark_strreserve (str_, ARK_STRGROW (str_)) != 0) return;
	}
	ARK_MEMCPY (str_->data + str_->len, src.data, src.len);
}

ARKEXPORT __decl_overloadable__ u0 ark_strrappend (ark_string* __noalias__ str_, Char src_) {
	if (str_->len + 1 >= str_->capacity) {
		ark_strreserve (str_, ARK_STRGROW (str_));
	}
	str_->data[str_->len++] = src_;
}

ARKEXPORT __decl_overloadable__ bool ark_strcomp (ark_string s1_, ark_string s2_) {
	return (s1_.len == s2_.len && ARK_MEMCMP (s1_.data, s2_.data, s1_.len) == 0);
}

ARKEXPORT __decl_overloadable__ bool ark_strcomp (ark_string s1_, ark_string_view s2_) {
	return (s1_.len == s2_.len && ARK_MEMCMP (s1_.data, s2_.data, s1_.len) == 0);
}

ARKEXPORT __decl_overloadable__ bool ark_strcomp (ark_string s1_, CString __noalias__ s2_) {
	return (s1_.len == ARK_STRLEN (s2_) && ARK_MEMCMP (s1_.data, s2_, s1_.len) == 0);
}

/*
 * Compar N characters of S1 and S2.
 */
ARKEXPORT __decl_overloadable__ bool ark_strncomp (ark_string s1_, ark_string s2_, usz n_) {
	return ARK_MEMCMP (s1_.data, s2_.data, n_) == 0;
}

ARKEXPORT __decl_overloadable__ bool ark_strncomp (ark_string s1_, ark_string_view s2_, usz n_) {
	return ARK_MEMCMP (s1_.data, s2_.data, n_) == 0;
}

ARKEXPORT __decl_overloadable__ bool ark_strncomp (ark_string s1_, CString __noalias__ s2_, usz n_) {
	return ARK_MEMCMP (s1_.data, s2_, n_) == 0;
}

/*
 * Find first occurrence of NEEDLE in HAYSTACK.
 */
ARKEXPORT char* ark_strffo (ark_string haystack_, CString __noalias__ needle_) {
	usz needle_len = strlen (needle_);
	$foreach (c $in_string (haystack_)) {
		if (ARK_STRNCMP (c, needle_, needle_len) == 0) {
			return c;
		}
	}
	return nullptr;
}

/*
 * Find last occurrence of NEEDLE in HAYSTACK.
 */
ARKEXPORT char* ark_strflo (ark_string haystack_, CString __noalias__ needle_) {
	usz needle_len = strlen (needle_);
	$foreach_r (c $in_string (haystack_)) {
		if (ARK_STRNCMP (c, needle_, needle_len) == 0) {
			return c;
		}
	}
	return nullptr;
}

/*
 * Append S1_, S2_, ... onto a new ARK_STRING and return it.
 * *Any string passed as argument is modified.*
 */
ARKEXPORT __decl_overloadable__ ark_string ___ark__strconcat__ (ark_allocator* __noalias__ allocator_,
																ark_string				   s1_,
																ark_string				   s2_,
																...) {
	va_list args;
	va_start (args);

	ark_string r = strvconcat_str (allocator_, s1_, s2_, args);
	va_end (args);
	return r;
}

__decl_overloadable__ ark_string ___ark__strconcat__ (ark_string s1_, ark_string s2_, ...) {
	va_list args;
	va_start (args);

	ark_string r = strvconcat_str (s1_.allocator, s1_, s2_, args);
	va_end (args);
	return r;
}

ARKEXPORT __decl_overloadable__ ark_string ___ark__strconcat__ (ark_allocator* __noalias__ allocator_,
																ark_string				   s1_,
																ark_string_view			   s2_,
																...) {
	va_list args;
	va_start (args);

	ark_string r = strvconcat_sv (allocator_, s1_, s2_, args);
	va_end (args);
	return r;
}

ARKEXPORT __decl_overloadable__ ark_string ___ark__strconcat__ (ark_string s1_, ark_string_view s2_, ...) {
	va_list args;
	va_start (args);

	ark_string r = strvconcat_sv (s1_.allocator, s1_, s2_, args);
	va_end (args);
	return r;
}

ARKEXPORT __decl_overloadable__ ark_string ___ark__strconcat__ (ark_allocator* __noalias__ allocator_,
																ark_string				   s1_,
																CString __noalias__		   s2_,
																...) {
	va_list args;
	va_start (args);

	ark_string r = strvconcat_cstr (allocator_, s1_, s2_, args);
	va_end (args);
	return r;
}

ARKEXPORT __decl_overloadable__ ark_string ___ark__strconcat__ (ark_string s1_, CString __noalias__ s2_, ...) {
	va_list args;
	va_start (args);

	ark_string r = strvconcat_cstr (s1_.allocator, s1_, s2_, args);
	va_end (args);
	return r;
}

ARKEXPORT __decl_overloadable__ ark_string ark_strtolower (ark_allocator* __noalias__ allocator_, ark_string str_) {
	ark_darray (char) r = ark_make_darray (char, str_.len);
	defer {
		ark_destroy_darray (r);
	}
	$for (i, str_.len) {
		ark_darray_push_back (r, (char)tolower ((uchar)str_.data[i]));
	}
	ark_darray_push_back (r, '\0');
	return ark_make_string (allocator_, r->data);
}

ARKEXPORT __decl_overloadable__ ark_string ark_strtolower (ark_allocator* __noalias__ allocator_, ark_string_view sv_) {
	ark_darray (char) r = ark_make_darray (char, sv_.len);
	defer {
		ark_destroy_darray (r);
	}
	$for (i, sv_.len) {
		ark_darray_push_back (r, (char)tolower ((uchar)sv_.data[i]));
	}
	ark_darray_push_back (r, '\0');
	return ark_make_string (allocator_, r->data);
}

ARKEXPORT __decl_overloadable__ ark_string ark_strtoupper (ark_string str_) {
	ark_darray (char) r = ark_make_darray (char, str_.len);
	defer {
		ark_destroy_darray (r);
	}
	$for (i, str_.len) {
		ark_darray_push_back (r, (char)toupper ((uchar)str_.data[i]));
	}
	ark_darray_push_back (r, '\0');
	return ark_make_string (str_.allocator, r->data);
}

ARKEXPORT __decl_overloadable__ ark_string ark_strtoupper (ark_string_view sv) {
	ark_darray (char) r = ark_make_darray (char, sv.len);
	defer {
		ark_destroy_darray (r);
	}
	$for (i, sv.len) {
		ark_darray_push_back (r, (char)toupper ((uchar)sv.data[i]));
	}
	ark_darray_push_back (r, '\0');
	return ark_make_string (ARK_LIBC_ALLOCATOR, r->data);
}

ARKEXPORT __decl_overloadable__ ark_darray (ark_bstring)
	ark_strsplit (ark_allocator* __noalias__ allocator_, ark_string str_, ark_string_view patt_) {
	ark_string_view str			= ark_string_view_from (str_);
	ark_darray (ark_bstring) ss = ark_make_darray (ark_bstring, allocator_, 1);
	if (patt_.data[0] == '\0' || patt_.len == 0) {
		ark_darray_push_back (ss, ark_bstring_from (str));
		return ss;
	}
	u32 occ					 = 0;
	ark_darray (u32) offsets = ark_get_offsets (str, patt_, &occ);
	if (!offsets) {
		ark_darray_push_back (ss, ark_bstring_from (str));
		return ss;
	}
	defer {
		ark_destroy_darray (offsets);
	}
	ark_darray_reserve (ss, occ);
	usz start = 0;
	$for (i, occ) {
		usz			pos	 = ark_darray_at (offsets, i);
		ark_bstring bstr = ark_make_bstring (str.data + start, pos - start);
		ark_darray_push_back (ss, bstr);
		start = pos + patt_.len;
	}
	if (start <= str.len) {
		ark_bstring bstr = ark_make_bstring (str.data + start, str.len - start);
		ark_darray_push_back (ss, bstr);
	}
	return ss;
}

ARKEXPORT __decl_overloadable__ ark_darray (ark_bstring)
	ark_strsplit (ark_allocator* __noalias__ allocator_, ark_string str_, char delim_) {
	if (delim_ == '\0') return nullptr;
	ark_darray (ark_bstring) ss = ark_make_darray (ark_bstring, allocator_, 4);
	usz start					= 0;
	while (1) {
		Char* next = ARK_MEMCHR (str_.data + start, delim_, str_.len - start);
		if (!next) {
			ark_bstring bstr_ = ark_make_bstring (str_.data + start, str_.len - start);
			ark_darray_push_back (ss, bstr_);
			break;
		}
		usz			pos	  = next - str_.data;
		ark_bstring bstr_ = ark_make_bstring (str_.data + start, pos - start);
		ark_darray_push_back (ss, bstr_);
		start = pos + 1;
	}
	return ss;
}

ARKEXPORT __decl_overloadable__ ark_darray (ark_bstring) ark_strsplit (ark_string str_, CString __nonnull__ patt_) {
	return ark_strsplit (str_.allocator, str_, ark_make_string_view (patt_));
}

ARKEXPORT __decl_overloadable__ ark_darray (ark_bstring) ark_strsplit (ark_string str_, char delim_) {
	return ark_strsplit (str_.allocator, str_, delim_);
}

ARKEXPORT ark_pair (ark_string) ark_strsplit_at (ark_string str_, char* __noalias__ at_) {
	ptrdiff_t offset = (ptrdiff_t)(ark_string_end (str_) - at_);
	$assert (offset > 0 && offset < (susz)str_.len && "AT should be in SV inner string bounds");
	ark_string first_half  = ark_make_string (str_.allocator, str_.data, str_.len - offset);
	ark_string second_half = ark_make_string (str_.allocator, at_ + 1, offset);
	return ark_make_pair (ark_string, first_half, second_half);
}

ARK_END
#endif
