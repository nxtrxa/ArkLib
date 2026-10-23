#import <ctype.h>

#import "algorithms/bmh.h"
#import "allocators/arkarena_allocator.h"
#import "collections/arkarray.h"
#import "core/arkbstring.h"
#import "core/arkosstring.h"
#import "core/arkosstring_view.h"
#import "core/arkstring.h"
#import "core/arkstring_view.h"
#import "core/arkwstring.h"
#import "core/arkwstring_view.h"
#import "core/arkzstring.h"

ARK_BEGIN

ARK_BEGIN_IMPL (ark_string)

static constexpr sz STRING_INITIAL_CAPACITY = 16;

static constexpr f64 STRING_GROW_FACTOR = 1.5f;

static constexpr f64 STRING_SHRINK_FACTOR = 0.6f;

static constexpr f64 STRING_SHRINK_THRESHOLD = 0.3;

static constexpr sz STRTOUPPER_BUFFER_CAPACITY = 4096;

#define STRGROW(Str_)	((Str_)->_capacity * STRING_GROW_FACTOR)
#define STRSHRINK(Str_) ((Str_)->_capacity * STRING_SHRINK_FACTOR)

/*
 * INTERNAL API IMPLEMENTATION
 */

ARKINTERNAL cstring
stradup (ark_allocator allocator_, CString __noalias__ cstr_, sz len_, sz cap) {
	if (len_ + 1 > cap) cap = len_ + 1;
	cstring s_ = ark_allocator_alloc (allocator_, cap, 1);
	ARK_MEMCPY (s_, cstr_, len_);
	s_[len_] = '\0';
	return s_;
}


ARKINTERNAL __decl_overloadable__ ark_string vstrconcat (
	ark_allocator allocator_, ark_string s1_, ark_string s2_, va_list ss_
) {
	va_list ss_copy;
	va_copy (ss_copy, ss_);

	sz buff_sz = s1_._len + s2_._len;
	while (true) {
		ark_string s = va_arg (ss_copy, ark_string);
		if (ark_is_string_null (s)) break;
		buff_sz += s._len;
	}
	va_end (ss_copy);

	cstring buff = ark_allocator_calloc (allocator_, buff_sz, 1);
	if (!buff) return ark_null_string ();

	sz offset = 0;
	if (ARK_LIKELY (!(ark_is_string_null (s1_) || ark_is_string_empty (s1_)))) {
		ARK_MEMCPY (buff, s1_._data, s1_._len);
		offset += s1_._len;
	}
	if (ARK_LIKELY (!(ark_is_string_null (s2_) || ark_is_string_empty (s2_)))) {
		ARK_MEMCPY (buff + offset, s2_._data, s2_._len);
		offset += s2_._len;
	}

	while (true) {
		ark_string sv = va_arg (ss_, ark_string);
		if (ARK_LIKELY (
				!(ark_is_string_null (sv) || ark_is_string_empty (sv))
			)) {
			ARK_MEMCPY (buff + offset, sv._data, sv._len);
			offset += sv._len;
		} else
			break;
	}
	return (ark_string) {
		._allocator = allocator_,
		._data		= buff,
		._len		= buff_sz,
		._capacity	= buff_sz,
	};
}

ARKINTERNAL __decl_overloadable__ ark_string
vstrconcat (ark_string s1_, ark_string s2_, va_list ss_) {
	return vstrconcat (ARK_LIBC_ALLOCATOR, s1_, s2_, ss_);
}

ARKINTERNAL __decl_overloadable__ ark_string vstrconcat (
	ark_allocator allocator_, ark_string s1_, ark_string_view s2_, va_list ss_
) {
	va_list ss_copy;
	va_copy (ss_copy, ss_);

	sz buff_sz = s1_._len + s2_._len;
	while (true) {
		ark_string_view s = va_arg (ss_copy, ark_string_view);
		if (ark_is_string_null (s)) break;
		buff_sz += s._len;
	}
	va_end (ss_copy);

	cstring buff = ark_allocator_calloc (allocator_, buff_sz + 1, 1);
	if (!buff) return ark_null_string ();

	sz offset = 0;
	if (ARK_LIKELY (!(ark_is_string_null (s1_) || ark_is_string_empty (s1_)))) {
		ARK_MEMCPY (buff, s1_._data, s1_._len);
		offset += s1_._len;
	}
	if (ARK_LIKELY (!(ark_is_string_null (s2_) || ark_is_string_empty (s2_)))) {
		ARK_MEMCPY (buff + offset, s2_._data, s2_._len);
		offset += s2_._len;
	}

	while (true) {
		ark_string_view s = va_arg (ss_, ark_string_view);
		if (!ark_is_string_null (s)) {
			ARK_MEMCPY (buff + offset, s._data, s._len);
			offset += s._len;
		} else
			break;
	}

	return (ark_string) {
		._allocator = allocator_,
		._data		= buff,
		._len		= buff_sz,
		._capacity	= buff_sz,
	};
}

ARKINTERNAL __decl_overloadable__ ark_string vstrconcat (
	ark_allocator allocator_, ark_string s1_, CString s2_, va_list ss_
) {
	va_list ss_copy;
	va_copy (ss_copy, ss_);

	ark_class_string_view (s2, s2_);
	sz buff_sz = s1_._len + s2._len;
	while (true) {
		CString s = va_arg (ss_copy, CString);
		if (!s) break;
		buff_sz += ARK_STRLEN (s);
	}
	va_end (ss_copy);

	cstring buff = ark_allocator_calloc (allocator_, buff_sz + 1, 1);
	if (!buff) return ark_null_string ();

	sz offset = 0;
	if (ARK_LIKELY (!(ark_is_string_null (s1_) || ark_is_string_empty (s1_)))) {
		ARK_MEMCPY (buff, s1_._data, s1_._len);
		offset += s1_._len;
	}
	if (ARK_LIKELY (!(ark_is_string_null (s2) || ark_is_string_empty (s2)))) {
		ARK_MEMCPY (buff + offset, s2._data, s2._len);
		offset += s2._len;
	}

	while (true) {
		CString s = va_arg (ss_, CString);
		if (s) {
			sz _len = ARK_STRLEN (s);
			ARK_MEMCPY (buff + offset, s, _len);
			offset += _len;
		} else
			break;
	}

	return (ark_string) {
		._allocator = allocator_,
		._data		= buff,
		._len		= buff_sz,
		._capacity	= buff_sz,
	};
}

ARKINTERNAL __decl_overloadable__ ark_string
vstrconcat (ark_string s1_, CString s2_, va_list ss_) {
	return vstrconcat (s1_._allocator, s1_, s2_, ss_);
}

ARKINTERNAL __decl_overloadable__ ark_string
ark_strtoupper_big_string (ark_allocator allocator_, ark_string_view s_) {
	ark_class_async_arena_unsafe (arena, s_._len);
	ark_darray (char) buff
		= ark_make_darray (char, ARK_ASYNC_ARENA_ALLOCATOR (arena), s_._len)
			  .unwrap;
	$for (i, s_._len) {
		ark_darray_push_back (buff, (char)toupper ((uchar)s_._data[i]));
	}
	ark_darray_push_back (buff, (char)'\0');
	return ark_make_string (allocator_, buff->_data, s_._len);
}

ARKINTERNAL __decl_overloadable__ ark_bstring
ark_strtoupper_big_string (ark_string_view s_) {
	ark_class_async_arena_unsafe (arena);
	ark_darray (char) buff
		= ark_make_darray (char, ARK_ASYNC_ARENA_ALLOCATOR (arena), s_._len)
			  .unwrap;
	$for (i, s_._len) {
		ark_darray_push_back (buff, (char)toupper ((uchar)s_._data[i]));
	}
	ark_darray_push_back (buff, (char)'\0');
	return ark_make_bstring (buff->_data, s_._len);
}

/*
 * PUBLIC API IMPLEMENTATION
 */

ARKEXPORT __decl_overloadable__ ark_string ark_make_string (
	ark_allocator allocator_, CString __noalias__ cstr_, sz len_, sz cap_
) {
	sz _capacity = (cap_ > len_) ? cap_ : len_ + 1;
	return (ark_string) {
		._data		= stradup (allocator_, cstr_, len_, _capacity),
		._len		= len_,
		._capacity	= _capacity,
		._allocator = allocator_,
	};
}

ARKEXPORT __decl_overloadable__ ark_string
ark_make_string (ark_allocator allocator_, CString __noalias__ cstr_, sz len_) {
	return ark_make_string (allocator_, cstr_, len_, STRING_INITIAL_CAPACITY);
}

ARKEXPORT __decl_overloadable__ ark_string
ark_make_string (ark_allocator allocator_, CString __noalias__ cstr_) {
	return ark_make_string (
		allocator_, cstr_, (sz)ARK_STRLEN (cstr_), STRING_INITIAL_CAPACITY
	);
}

ARKEXPORT __decl_overloadable__ ark_string
ark_make_string (CString __noalias__ cstr_, sz len_, sz cap_) {
	return ark_make_string (ARK_LIBC_ALLOCATOR, cstr_, len_, cap_);
}

ARKEXPORT __decl_overloadable__ ark_string
ark_make_string (CString __noalias__ cstr_, sz len_) {
	return ark_make_string (
		ARK_LIBC_ALLOCATOR, cstr_, len_, STRING_INITIAL_CAPACITY
	);
}


ARKEXPORT __decl_overloadable__ ark_string
ark_make_string (CString __noalias__ cstr_) {
	return ark_make_string (
		ARK_LIBC_ALLOCATOR,
		cstr_,
		(sz)ARK_STRLEN (cstr_),
		STRING_INITIAL_CAPACITY
	);
}

ARKEXPORT u0 ark_destroy_string_fn (ark_string* __noalias__ s_) {
	ark_allocator_dealloc (s_->_allocator, s_->_data);
}

// ARKEXPORT cstring ark_strtrunc (ark_string* __noalias__ s_) {
// 	if (!s_->_data) return nullptr;
//
// 	ark_strrappend (s_, (Char)'\0');
// 	return s_->_data;
// }

ARKEXPORT __decl_overloadable__ u0
ark_strcopy (ark_string* __noalias__ dest_, CString __noalias__ src_) {
	ark_destroy_string (dest_);
	*dest_ = ark_make_string (src_);
}

ARKEXPORT __decl_overloadable__ u0
ark_strcopy (ark_string* __noalias__ dest_, ark_string src_) {
	ark_string cp = ark_make_string (
		dest_->_allocator, src_._data, src_._len, src_._capacity
	);
	ark_destroy_string (dest_);
	*dest_ = cp;
}

ARKEXPORT __decl_overloadable__ u0
ark_strcopy (ark_string* __noalias__ dest_, ark_string_view src_) {
	ark_destroy_string (dest_);
	*dest_ = ark_string_from (src_);
}

ARKEXPORT u0 ark_strlchop (ark_string* __noalias__ s_, u8 n_) {
	if (ark_is_string_empty (*s_)) return;
	if (n_ >= s_->_len) {
		ark_nullify_string (s_);
	}
	ARK_MEMMOVE (s_->_data, s_->_data + n_, s_->_len - n_);
}

ARKEXPORT u0 ark_strrchop (ark_string* __noalias__ s_, u8 n_) {
	if (ark_is_string_empty (*s_)) return;
	sz chop_  = (n_ > s_->_len) ? s_->_len : n_;
	s_->_len -= chop_;
}

ARKEXPORT __decl_overloadable__ u0 ark_strltrim (ark_string* __noalias__ s_) {
	if (ark_is_string_empty (*s_)) return;
	sz i = 0;
	while (i < s_->_len && isspace ((uchar)s_->_data[i])) {
		i++;
	}
	if (ARK_LIKELY (i > 0)) {
		ARK_MEMMOVE (s_->_data, s_->_data + i, s_->_len - i);
		s_->_len -= i;
	}
}

ARKEXPORT __decl_overloadable__ u0 ark_strrtrim (ark_string* __noalias__ s_) {
	if (ark_is_string_empty (*s_)) return;
	sz i = 0;
	while (i < s_->_len && isspace ((uchar)s_->_data[i])) {
		i++;
	}
	s_->_len -= i;
}

ARKEXPORT __decl_overloadable__ u0 ark_strtrim (ark_string* __noalias__ s_) {
	ark_strltrim (s_);
	ark_strrtrim (s_);
}

ARKEXPORT int ark_strreserve (ark_string* __noalias__ s_, sz expec_cap_) {
	const sz old_alloc_sz = s_->_capacity;
	u0*		 ptr		  = ark_allocator_realloc (
		s_->_allocator, s_->_data, old_alloc_sz, expec_cap_, 1
	);
	if (!ptr) {
		ARK_WARN ("realloc failed");
		return -1;
	}
	ark_allocator_dealloc (s_->_allocator, s_->_data);
	s_->_data	  = ptr;
	s_->_capacity = expec_cap_;
	return 0;
}

ARKEXPORT __decl_overloadable__ u0
ark_strrappend (ark_string* __noalias__ s_, ark_string_view src_) {
	sz cap = s_->_len + src_._len + 1;
	if (cap > s_->_capacity) {
		if (ark_strreserve (s_, cap) != 0) {
			ARK_WARN ("Failed to reserve memory");
			return;
		}
	}
	ARK_MEMCPY (s_->_data + s_->_len, src_._data, src_._len);
	s_->_len += src_._len;
}

ARKEXPORT __decl_overloadable__ u0
ark_strrappend (ark_string* __noalias__ s_, CString __nonnull__ src_) {
	ark_strrappend (s_, ark_make_string_view (src_));
}

ARKEXPORT __decl_overloadable__ u0
ark_strrappend (ark_string* __noalias__ s_, Char src_) {
	if (s_->_len + 1 >= s_->_capacity) {
		if (ark_strreserve (s_, STRGROW (s_)) != 0) {
			ARK_WARN ("Failed to reserve memory");
			return;
		}
	}
	s_->_data[s_->_len++] = src_;
}

ARKEXPORT __decl_overloadable__ Char*
ark_strffo (ark_string haystack_, ark_string needle_) {
	if (needle_._len > haystack_._len) {
		ARK_WARN ("needle length is greater than haystack length");
		return nullptr;
	}
	Char* ch = nullptr;
	$for (i, haystack_._len - needle_._len + 1) {
		if (ARK_STRNCMP (haystack_._data + i, needle_._data, needle_._len)
			== 0) {
			ch = haystack_._data + i;
		}
	}
	return ch;
}

ARKEXPORT __decl_overloadable__ Char*
ark_strffo (ark_string haystack_, CString __nonnull__ needle_) {
	return ark_strffo (ark_string_view_from (haystack_), needle_);
}

ARKEXPORT __decl_overloadable__ Char*
ark_strffo (ark_string haystack_, Char chr_) {
	Char* ch = nullptr;
	$foreach (c $in_string (haystack_)) {
		if (*c == chr_) ch = c;
	}
	return ch;
}

ARKEXPORT __decl_overloadable__ Char*
ark_strflo (ark_string haystack_, ark_string needle_) {
	if (needle_._len > haystack_._len) {
		ARK_WARN ("needle length is greater than haystack length");
		return nullptr;
	}
	Char* ch = nullptr;
	$rfor (i, haystack_._len - needle_._len) {
		if (ARK_STRNCMP (haystack_._data + i, needle_._data, needle_._len)
			== 0) {
			ch = haystack_._data + i;
		}
	}
	return ch;
}

ARKEXPORT __decl_overloadable__ Char*
ark_strflo (ark_string haystack_, CString __nonnull__ needle_) {
	return ark_strffo (
		ark_string_view_from (haystack_), ark_make_string_view (needle_)
	);
}

ARKEXPORT __decl_overloadable__ Char*
ark_strflo (ark_string haystack_, Char chr_) {
	Char* ch = nullptr;
	$rforeach (c $in_string (haystack_)) {
		if (*c == chr_) ch = c;
	}
	return ch;
}

ARKEXPORT __decl_overloadable__ Char*
ark_strfno (ark_string haystack_, ark_string needle_, sz nth_) {
	if (needle_._len > haystack_._len) {
		ARK_WARN ("needle length is greater than haystack length");
		return nullptr;
	}
	Char* ch  = nullptr;
	sz	  occ = 1;
	$for (i, haystack_._len - needle_._len + 1) {
		if (ARK_STRNCMP (haystack_._data, needle_._data, needle_._len) == 0
			&& occ == nth_) {
			ch = haystack_._data + i;
			break;
		}
		occ++;
	}
	return ch;
}

ARKEXPORT __decl_overloadable__ Char*
ark_strfno (ark_string haystack_, Char chr_, sz nth_) {
	Char* ch  = nullptr;
	sz	  occ = 1;
	$foreach (c $in_string (haystack_)) {
		if (*c == chr_ && occ == nth_) ch = c;
		occ++;
	}
	return ch;
}

ARKEXPORT __decl_overloadable__ Char*
ark_strfno (ark_string haystack_, CString __nonnull__ needle_, sz nth_) {
	return ark_strfno (
		ark_string_view_from (haystack_), ark_make_string_view (needle_), nth_
	);
}

ARKEXPORT __decl_overloadable__ ark_string
ark_strtolower (ark_allocator allocator_, ark_string s_) {
	if (ARK_UNLIKELY (s_._len > STRTOUPPER_BUFFER_CAPACITY))
		return ark_strtoupper_big_string (
			allocator_, ark_string_view_from (s_)
		);
	ark_class_array (char, STRTOUPPER_BUFFER_CAPACITY, buffer);
	$for (i, s_._len) {
		ark_array_set (buffer, i, (char)tolower ((uchar)s_._data[i]));
	}
	ark_array_set (buffer, s_._len, (char)'\0');
	return ark_make_string (allocator_, buffer->_data, s_._len);
}

ARKEXPORT __decl_overloadable__ ark_bstring ark_strtoupper (ark_string s_) {
	if (ARK_UNLIKELY (s_._len > STRTOUPPER_BUFFER_CAPACITY))
		return ark_strtoupper_big_string (ark_string_view_from (s_));
	ark_class_array (char, STRTOUPPER_BUFFER_CAPACITY, buffer);
	$for (i, s_._len) {
		ark_array_set (buffer, i, (char)tolower ((uchar)s_._data[i]));
	}
	ark_array_set (buffer, s_._len, (char)'\0');
	return ark_make_bstring (buffer->_data, s_._len);
}


ARKEXPORT __decl_overloadable__ ark_string ark_strconcat_fn (
	ark_allocator allocator_, ark_string s1_, ark_string s2_, ...
) {
	va_list ss;
	va_start (ss);
	defer {
		va_end (ss);
	}
	return vstrconcat (allocator_, s1_, s2_, ss);
}

ARKEXPORT __decl_overloadable__ ark_string ark_strconcat_fn (
	ark_allocator allocator_, ark_string s1_, ark_string_view s2_, ...
) {
	va_list ss;
	va_start (ss);
	defer {
		va_end (ss);
	}
	return vstrconcat (allocator_, s1_, s2_, ss);
}

ARKEXPORT __decl_overloadable__ ark_string
ark_strconcat_fn (ark_allocator allocator_, ark_string s1_, CString s2_, ...) {
	va_list ss;
	va_start (ss);
	defer {
		va_end (ss);
	}
	return vstrconcat (allocator_, s1_, s2_, ss);
}

ARKEXPORT __decl_overloadable__ ark_string
ark_strconcat_fn (ark_string s1_, ark_string s2_, ...) {
	va_list ss;
	va_start (ss);
	defer {
		va_end (ss);
	}
	return vstrconcat (s1_, s2_, ss);
}

ARKEXPORT __decl_overloadable__ ark_string
ark_strconcat_fn (ark_string s1_, CString s2_, ...) {
	va_list ss;
	va_start (ss);
	defer {
		va_end (ss);
	}
	return vstrconcat (s1_, s2_, ss);
}

ARKEXPORT __decl_overloadable__ u0* ark_strsplit_fn (
	ark_allocator allocator_, ark_string s_, ark_string_view pattern_
) {
	u32 occ = 0;
	ark_raii (ark_darray) (u32) offsets
		= ark_string_find_offsets (allocator_, s_, pattern_, &occ).unwrap;
	ark_darray (ark_bstring)
		ss = ark_make_darray (ark_bstring, allocator_, occ + 1).unwrap;
	if (!offsets) {
		ark_darray_push_back (ss, ark_bstring_from (s_));
		return ss;
	}
	sz pos = 0;
	$foreach (offset $in_darray (offsets)) {
		ark_bstring s = ark_make_bstring (s_._data + pos, *offset - pos);
		ark_darray_push_back (ss, s);
		pos = *offset + pattern_._len;
	}
	ark_bstring remain = ark_make_bstring (s_._data + pos, s_._len - pos);
	ark_darray_push_back (ss, remain);
	return ss;
}

ARKEXPORT __decl_overloadable__ u0*
ark_strsplit_fn (ark_string s_, ark_string_view pattern_) {
	return ark_strsplit_fn (ARK_LIBC_ALLOCATOR, s_, pattern_);
}

ARKEXPORT __decl_overloadable__ u0* ark_strsplit_fn (
	ark_allocator allocator_, ark_string s_, CString __nonnull__ pattern_
) {
	return ark_strsplit_fn (allocator_, s_, ark_make_string_view (pattern_));
}

ARKEXPORT __decl_overloadable__ u0*
ark_strsplit_fn (ark_string s_, CString __nonnull__ pattern_) {
	return ark_strsplit_fn (
		ARK_LIBC_ALLOCATOR, s_, ark_make_string_view (pattern_)
	);
}


ARKEXPORT __decl_overloadable__ ark_pair (ark_string) ark_strsplit_at (
	ark_allocator allocator_, ark_string s_, Char* __noalias__ delim_
) {
	ptrdiff_t offset = delim_ - s_._data;
	$assert_internal (
		offset > 0 && offset < (ssz)s_._len && "pointer is out of string bounds"
	);
	ark_string first_half  = ark_make_string (allocator_, s_._data, offset);
	ark_string second_half = ark_make_string (allocator_, delim_ + 1, offset);
	return ark_make_pair (ark_string, first_half, second_half);
}

ARKEXPORT __decl_overloadable__ ark_pair (ark_string) ark_strsplit_at (
	ark_string s_, Char* __noalias__ delim_
) {
	return ark_strsplit_at (ARK_LIBC_ALLOCATOR, s_, delim_);
}

ARK_END_IMPL (ark_string)


ARK_BEGIN_IMPL (ark_bstring)

/*
 * INTERNAL API IMPLEMENTATION
 */

/*
 * PUBLIC API IMPLEMENTATION
 */

ARKEXPORT __decl_overloadable__ ark_bstring
ark_make_bstring (CString __noalias__ cstr_, sz len_) {
	return (ark_bstring) {
		._data = strdup (cstr_),
		._len  = len_,
	};
}

ARKEXPORT __decl_overloadable__ ark_bstring
ark_make_bstring (CString __noalias__ cstr_) {
	return (ark_bstring) {
		._data = strdup (cstr_),
		._len  = strlen (cstr_),
	};
}

ARKEXPORT u0 ark_destroy_bstring (ark_bstring* s_) {
	ark_dealloc (s_->_data);
	s_->_len = 0;
}

ARKEXPORT __decl_overloadable__ u0 ark_strltrim (ark_bstring* __noalias__ s_) {
	if (ark_is_string_empty (*s_)) return;
	sz i = 0;
	while (i < s_->_len && isspace ((uchar)s_->_data[i])) {
		i++;
	}
	if (ARK_LIKELY (i > 0)) {
		ARK_MEMMOVE (s_->_data, s_->_data + i, s_->_len - i);
		s_->_len -= i;
	}
}

ARKEXPORT __decl_overloadable__ u0 ark_strrtrim (ark_bstring* __noalias__ s_) {
	if (ark_is_string_empty (*s_)) return;
	sz i = 0;
	while (i < s_->_len && isspace ((uchar)s_->_data[i])) {
		i++;
	}
	s_->_len -= i;
}

ARKEXPORT __decl_overloadable__ u0 ark_strtrim (ark_bstring* __noalias__ s_) {
	ark_strltrim (s_);
	ark_strrtrim (s_);
}

ARK_END_IMPL (ark_bstring)

ARK_BEGIN_IMPL (ark_string_view)

/*
 * INTERNAL API IMPLEMENTATION
 */

ARKINTERNAL __decl_overloadable__ ark_string vstrconcat (
	ark_allocator	allocator_,
	ark_string_view s1_,
	ark_string_view s2_,
	va_list			ss_
) {
	va_list ss_copy;
	va_copy (ss_copy, ss_);

	sz buff_sz = s1_._len + s2_._len;
	while (true) {
		ark_string_view s = va_arg (ss_copy, ark_string_view);
		if (ark_is_string_null (s)) break;
		buff_sz += s._len;
	}
	va_end (ss_copy);

	cstring buff = ark_allocator_calloc (allocator_, buff_sz, 1);
	if (!buff) return ark_null_string ();

	sz offset = 0;
	if (ARK_LIKELY (!(ark_is_string_null (s1_) || ark_is_string_empty (s1_)))) {
		ARK_MEMCPY (buff, s1_._data, s1_._len);
		offset += s1_._len;
	}
	if (ARK_LIKELY (!(ark_is_string_null (s2_) || ark_is_string_empty (s2_)))) {
		ARK_MEMCPY (buff + offset, s2_._data, s2_._len);
		offset += s2_._len;
	}

	while (true) {
		ark_string_view sv = va_arg (ss_, ark_string_view);
		if (ARK_LIKELY (
				!(ark_is_string_null (sv) || ark_is_string_empty (sv))
			)) {
			ARK_MEMCPY (buff + offset, sv._data, sv._len);
			offset += sv._len;
		} else
			break;
	}

	return (ark_string) {
		._allocator = allocator_,
		._data		= buff,
		._len		= buff_sz,
		._capacity	= buff_sz,
	};
}

ARKINTERNAL __decl_overloadable__ ark_string
vstrconcat (ark_string_view s1_, ark_string_view s2_, va_list ss_) {
	return vstrconcat (ARK_LIBC_ALLOCATOR, s1_, s2_, ss_);
}

ARKINTERNAL __decl_overloadable__ ark_string vstrconcat (
	ark_allocator allocator_, ark_string_view s1_, CString s2_, va_list ss_
) {
	va_list ss_copy;
	va_copy (ss_copy, ss_);

	sz s2_len  = ARK_STRLEN (s2_);
	sz buff_sz = s1_._len + s2_len;
	while (true) {
		CString s = va_arg (ss_copy, CString);
		if (!s) break;
		buff_sz += ARK_STRLEN (s);
	}
	va_end (ss_copy);

	cstring buff = ark_allocator_calloc (allocator_, buff_sz + 1, 1);
	if (!buff) return ark_null_string ();

	snprintf (buff, buff_sz, ark_strfmt, ark_strarg (s1_));
	snprintf (buff, buff_sz, "%.*s", (int32_t)s2_len, s2_);
	sz offset = s1_._len;
	while (true) {
		CString s = va_arg (ss_, CString);
		if (s) {
			sz _len = ARK_STRLEN (s);
			ARK_MEMCPY (buff + offset, s, _len);
			offset += _len;
		} else
			break;
	}

	return (ark_string) {
		._allocator = allocator_,
		._data		= buff,
		._len		= buff_sz,
		._capacity	= buff_sz,
	};
}

ARKINTERNAL __decl_overloadable__ ark_string
vstrconcat (ark_string_view s1_, CString s2_, va_list ss_) {
	return vstrconcat (ARK_LIBC_ALLOCATOR, s1_, s2_, ss_);
}

ARKEXPORT __decl_overloadable__ ark_string_view
ark_make_string_view (CString __nonnull__ cstr_) {
	return (ark_string_view) {
		._data = cstr_,
		._len  = ARK_STRLEN (cstr_),
	};
}

ARKEXPORT __decl_overloadable__ ark_string_view
ark_make_string_view (CString __nonnull__ cstr_, sz _len) {
	return (ark_string_view) {
		._data = cstr_,
		._len  = _len,
	};
}

ARKEXPORT __decl_overloadable__ u0
ark_strlchop (ark_string_view* __noalias__ s_, sz n_) {
	if (ARK_UNLIKELY (ark_is_string_empty (*s_))) return;
	sz chop_   = (n_ > s_->_len) ? s_->_len : n_;
	s_->_data += chop_;
	s_->_len  -= chop_;
}

ARKEXPORT __decl_overloadable__ u0
ark_strlchop (ark_string_view* __noalias__ s_) {
	return ark_strlchop (s_, 1);
}

ARKEXPORT __decl_overloadable__ u0
ark_strrchop (ark_string_view* __noalias__ s_, sz n_) {
	if (ARK_UNLIKELY (ark_is_string_empty (*s_))) return;
	sz chop_  = (n_ > s_->_len) ? s_->_len : n_;
	s_->_len -= chop_;
}

ARKEXPORT __decl_overloadable__ u0
ark_strrchop (ark_string_view* __noalias__ s_) {
	return ark_strrchop (s_, 1);
}

ARKEXPORT __decl_overloadable__ u0
ark_strltrim (ark_string_view* __noalias__ s_) {
	if (ark_is_string_empty (*s_)) return;
	sz i = 0;
	while (i < s_->_len && isspace ((uchar)s_->_data[i])) {
		i++;
	}
	s_->_data += i;
	s_->_len  -= i;
}

ARKEXPORT __decl_overloadable__ u0
ark_strrtrim (ark_string_view* __noalias__ s_) {
	if (ark_is_string_empty (*s_)) return;
	sz i = 0;
	while (i < s_->_len && isspace ((uchar)s_->_data[i])) {
		i++;
	}
	s_->_len -= i;
}

ARKEXPORT __decl_overloadable__ u0
ark_strtrim (ark_string_view* __noalias__ s_) {
	ark_strltrim (s_);
	ark_strrtrim (s_);
}

ARKEXPORT __decl_overloadable__ Char*
ark_strffo (ark_string_view haystack_, ark_string_view needle_) {
	if (ark_is_string_empty (haystack_) || ark_is_string_empty (needle_))
		return nullptr;
	if (needle_._len > haystack_._len) return nullptr;
	Char* ch = nullptr;
	$for (i, haystack_._len - needle_._len + 1) {
		if (ARK_STRNCMP (haystack_._data + i, needle_._data, needle_._len)
			== 0) {
			ch = haystack_._data + i;
		}
	}
	return ch;
}

ARKEXPORT __decl_overloadable__ Char*
ark_strffo (ark_string_view haystack_, Char chr_) {
	if (ark_is_string_empty (haystack_)) return nullptr;
	Char* ch = nullptr;
	$foreach (c $in_string (haystack_)) {
		if (*c == chr_) ch = c;
	}
	return ch;
}

ARKEXPORT __decl_overloadable__ Char*
ark_strffo (ark_string_view haystack_, CString __nonnull__ needle_) {
	return ark_strffo (haystack_, ark_make_string_view (needle_));
}

ARKEXPORT __decl_overloadable__ Char*
ark_strflo (ark_string_view haystack_, ark_string_view needle_) {
	if (ark_is_string_empty (haystack_) || ark_is_string_empty (needle_))
		return nullptr;
	if (needle_._len > haystack_._len + 1) return nullptr;
	Char* ch = nullptr;
	$rfor (i, haystack_._len - needle_._len) {
		if (ARK_STRNCMP (haystack_._data + i, needle_._data, needle_._len)
			== 0) {
			ch = haystack_._data + i;
		}
	}
	return ch;
}

ARKEXPORT __decl_overloadable__ Char*
ark_strflo (ark_string_view haystack_, Char chr_) {
	if (ark_is_string_empty (haystack_)) return nullptr;
	Char* ch = nullptr;
	$rforeach (c $in_string (haystack_)) {
		if (*c == chr_) ch = c;
	}
	return ch;
}

ARKEXPORT __decl_overloadable__ Char*
ark_strflo (ark_string_view haystack_, CString __nonnull__ needle_) {
	return ark_strffo (haystack_, ark_make_string_view (needle_));
}

ARKEXPORT __decl_overloadable__ Char*
ark_strfno (ark_string_view haystack_, ark_string_view needle_, sz n_) {
	if (needle_._len > haystack_._len) {
		ARK_WARN ("needle length is greater than haystack length");
		return nullptr;
	}
	Char* ch  = nullptr;
	sz	  occ = 1;
	$for (i, haystack_._len - needle_._len + 1) {
		if (ARK_STRNCMP (haystack_._data, needle_._data, needle_._len) == 0
			&& occ == n_) {
			ch = haystack_._data + i;
			break;
		}
		occ++;
	}
	return ch;
}

ARKEXPORT __decl_overloadable__ Char*
ark_strfno (ark_string_view haystack_, CString __nonnull__ needle_, sz n_) {
	return ark_strfno (haystack_, ark_make_string_view (needle_), n_);
}

ARKEXPORT __decl_overloadable__ Char*
ark_strfno (ark_string_view haystack_, Char chr_, sz n_) {
	Char* ch  = nullptr;
	sz	  occ = 1;
	$foreach (c $in_string (haystack_)) {
		if (*c == chr_ && occ == n_) ch = c;
		occ++;
	}
	return ch;
}


ARKEXPORT __decl_overloadable__ ark_string
ark_strtolower (ark_allocator allocator_, ark_string_view sv_) {
	if (ARK_UNLIKELY (sv_._len > STRTOUPPER_BUFFER_CAPACITY))
		return ark_strtoupper_big_string (allocator_, sv_);
	ark_class_array (char, STRTOUPPER_BUFFER_CAPACITY, buffer);
	$for (i, sv_._len) {
		ark_array_set (buffer, i, (char)tolower ((uchar)sv_._data[i]));
	}
	ark_array_set (buffer, sv_._len, (char)'\0');
	return ark_make_string (allocator_, buffer->_data, sv_._len);
}

ARKEXPORT __decl_overloadable__ ark_bstring
ark_strtoupper (ark_string_view sv_) {
	if (ARK_UNLIKELY (sv_._len > STRTOUPPER_BUFFER_CAPACITY))
		return ark_strtoupper_big_string (sv_);
	ark_class_array (char, STRTOUPPER_BUFFER_CAPACITY, buffer);
	$for (i, sv_._len) {
		ark_array_set (buffer, i, (char)toupper ((uchar)sv_._data[i]));
	}
	ark_array_set (buffer, sv_._len, (char)'\0');
	return ark_make_bstring (buffer->_data, sv_._len);
}

ARKEXPORT __decl_overloadable__ ark_string ark_strconcat_fn (
	ark_allocator allocator_, ark_string_view s1_, ark_string_view s2_, ...
) {
	va_list ss;
	va_start (ss);
	defer {
		va_end (ss);
	}
	return vstrconcat (allocator_, s1_, s2_, ss);
}

ARKEXPORT __decl_overloadable__ ark_string ark_strconcat_fn (
	ark_allocator allocator_, ark_string_view s1_, CString s2_, ...
) {
	va_list ss;
	va_start (ss);
	defer {
		va_end (ss);
	}
	return vstrconcat (allocator_, s1_, s2_, ss);
}

ARKEXPORT __decl_overloadable__ ark_string
ark_strconcat_fn (ark_string_view s1_, ark_string_view s2_, ...) {
	va_list ss;
	va_start (ss);
	defer {
		va_end (ss);
	}
	return vstrconcat (s1_, s2_, ss);
}

ARKEXPORT __decl_overloadable__ ark_string
ark_strconcat_fn (ark_string_view s1_, CString s2_, ...) {
	va_list ss;
	va_start (ss);
	defer {
		va_end (ss);
	}
	return vstrconcat (s1_, s2_, ss);
}

ARK_END_IMPL (ark_string_view)


ARK_END
