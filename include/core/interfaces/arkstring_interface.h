#ifndef ARK_INTEFACES_STRING_H_
#define ARK_INTEFACES_STRING_H_

/*------------------------------------------------------------------------------
 * Mutable -> can modify size in memory.
 *
 * Name                 Owned   Mutable  Null terminated  Specialized allocator
 *
 * ark_string           Yes     Yes      No               Yes
 * ark_bstring          Yes     No       No               No
 * ark_zstring          Yes     Yes      Yes              Yes
 * ark_wstring          Yes     Yes      No               Yes
 * ark_wbstring         Yes     No       No               No
 * ark_wzstring         Yes     Yes      Yes              Yes
 * ark_osstring         Yes     Yes      No               Yes
 * ark_string_view      No      No       No               No
 * ark_wstring_view     No      No       No               No
------------------------------------------------------------------------------*/

#import "collections/arkdarray.h"
#import "math/arkpair.h"

ARK_BEGIN

#ifdef ARK_NAMESPACE
	#define string				  ark_string
	#define zstring				  ark_zstring
	#define wstring				  ark_wstring
	#define sstring				  ark_sstring
	#define bstring				  ark_bstring
	#define string_view			  ark_string_view
	#define zstring_view		  ark_zstring_view
	#define wstring_view		  ark_wstring_view
	#define osstring			  ark_osstring
	#define strfmt				  ark_strfmt
	#define strarg				  ark_strarg
	#define to_string_view		  ark_to_string_view
	#define is_string_null		  ark_is_string_null
	#define is_string_empty		  ark_is_string_empty
	#define is_string_initialized ark_is_string_initialized
	#define null_string			  ark_null_string
	#define string_front		  ark_string_front
	#define string_back			  ark_string_back
	#define string_begin		  ark_string_begin
	#define string_end			  ark_string_end
#endif

#define ark_strfmt		 "%.*s"
#define ark_strarg(Str_) (int)(Str_)._len, (Str_)._data

#define ark_strtrunc(Str_)

#define ark_is_string_null(Str_)  (!(Str_)._data || *(Str_)._data == '\0')
#define ark_is_string_empty(Str_) ((Str_)._len == 0)
#define ark_is_string_initialized(Str_)                                        \
	(!ark_is_string_null (Str_) && !ark_is_string_empty (Str_))

#define ark_null_string()                                                      \
	((ark_string) {                                                            \
		._data = nullptr,                                                      \
	})

#define ark_nullify_string(StrPtr_)                                            \
	do {                                                                       \
		auto ___strptr__ = &(StrPtr_);                                         \
		(*___strptr__)	 = (typeof (*___strptr__)) {};                         \
	} while (0)

#define ark_string_front(Str_) (Str_)._data[0]
#define ark_string_back(Str_)  (Str_)._data[(Str_)._len ? (Str_)._len - 1 : 0]

#define ark_string_begin(Str_) (Str_)._data
#define ark_string_end(Str_)   ((Str_)._data + (Str_)._len)

#define $in_string(Str_)   ARK_DECL_IN_MACRO ((Str_), ark_string)
#define $from_string(Str_) ARK_DECL_FROM_MACRO ((Str_), ark_string)


#define ark_string_at(Str_, Idx_)                                              \
	({                                                                         \
		sz	 ___idx__ = (Idx_);                                                \
		auto ___str__ = (Str_);                                                \
		$assert (                                                              \
			___idx__ < ___str__._len                                           \
			&& "ark_string_at: index overflows string length"                  \
		);                                                                     \
		___str__._data[___idx__];                                              \
	})

#define ark_string_ref(Str_, Idx_)                                             \
	({                                                                         \
		sz	 ___idx__ = (Idx_);                                                \
		auto ___str__ = (Str_);                                                \
		$assert (                                                              \
			___idx__ < ___str__._len                                           \
			&& "ark_string_ref: index overflows string length"                 \
		);                                                                     \
		___str__._data + ___idx__;                                             \
	})

#define ark_strcomp(S1_, S2_)                                                  \
	({                                                                         \
		bool ___cmp__ = false;                                                 \
		if (ark_is_string_null (S1_) || ark_is_string_null (S2_)) {            \
			___cmp__ = (ark_is_string_null (S1_) && ark_is_string_null (S2_)); \
		} else {                                                               \
			___cmp__                                                           \
				= (S1_._len == S2_._len                                        \
				   && ARK_MEMCMP (S1_._data, S2_._data, S1_._len) == 0);       \
		}                                                                      \
		___cmp__;                                                              \
	})

#define ark_strncomp(S1_, S2_, N_)                                             \
	({                                                                         \
		bool ___cmp__ = false;                                                 \
		if (ark_is_string_null (S1_) || ark_is_string_null (S2_)) {            \
			___cmp__ = (ark_is_string_null (S1_) && ark_is_string_null (S2_)); \
		} else {                                                               \
			___cmp__ = (ARK_MEMCMP (S1_._data, S2_._data, N_) == 0);           \
		}                                                                      \
		___cmp__;                                                              \
	})

#define ark_strconcat(Arg1_, ...)                                              \
	$if (                                                                      \
		$is_struct (ark_pp_va_arg_2 (__VA_ARGS__)),                            \
		ark_strconcat_fn (Arg1_, __VA_ARGS__, ark_null_string ()),             \
		ark_strconcat_fn (Arg1_, __VA_ARGS__, nullptr)                         \
	)

#define ark_strsplit(...)                                                      \
	(ark_darray (ark_bstring)) (ark_strsplit_fn (__VA_ARGS__))

#define ARK_STRING_TYPES                                                       \
	ARK_STRING_TYPE (ark_string)                                               \
	ARK_STRING_TYPE (ark_osstring)                                             \
	ARK_STRING_TYPE (ark_zstring)                                              \
	ARK_STRING_TYPE (ark_wstring)                                              \
	ARK_STRING_TYPE (ark_bstring)                                              \
	ARK_STRING_TYPE (ark_string_view)                                          \
	ARK_STRING_TYPE (ark_wstring_view)                                         \
	ARK_STRING_TYPE (ark_osstring_view)

#define ARK_STRING_TYPE(StrType_) ARK_CLASS (StrType_);
ARK_STRING_TYPES
#undef ARK_STRING_TYPE

/*
 * interface implementation for utf8 strings
 */

#define ARK_UTF8_STRING_INTERFACE_PROTOTYPES(StrType_)                         \
	ARK_UTF8_STRING_FUNCTIONS_PROTOTYPES (StrType_)

#define ARK_UTF8_STRING_FUNCTIONS_PROTOTYPES(StrType_)                         \
	ARK_STRLCHOP_PROT (StrType_)                                               \
	ARK_STRRCHOP_PROT (StrType_)                                               \
	ARK_STRLTRIM_PROT (StrType_)                                               \
	ARK_STRRTRIM_PROT (StrType_)                                               \
	ARK_STRTRIM_PROT (StrType_)                                                \
	ARK_STRFFO_PROT (StrType_)                                                 \
	ARK_STRFLO_PROT (StrType_)                                                 \
	ARK_STRFNO_PROT (StrType_)                                                 \
	ARK_STRCONCAT_PROT (StrType_)                                              \
	ARK_STRSPLIT_PROT (StrType_)                                               \
	ARK_STRSPLIT_AT_PROT (StrType_)

#define ARK_STRLCHOP_PROT(StrType_)                                            \
	ARKAPI __decl_overloadable__ u0 ark_strlchop (                             \
		StrType_* __noalias__ s_, sz n_                                        \
	);                                                                         \
	ARKAPI __decl_overloadable__ __decl_flatten__ u0 ark_strlchop (            \
		StrType_* __noalias__ s_                                               \
	);

#define ARK_STRRCHOP_PROT(StrType_)                                            \
	ARKAPI __decl_overloadable__ u0 ark_strrchop (                             \
		StrType_* __noalias__ s_, sz n_                                        \
	);                                                                         \
	ARKAPI __decl_overloadable__ __decl_flatten__ u0 ark_strrchop (            \
		StrType_* __noalias__ s_                                               \
	);

#define ARK_STRLTRIM_PROT(StrType_)                                            \
	ARKAPI __decl_overloadable__ u0 ark_strltrim (StrType_* __noalias__ s_);

#define ARK_STRRTRIM_PROT(StrType_)                                            \
	ARKAPI __decl_overloadable__ u0 ark_strrtrim (StrType_* __noalias__ s_);

#define ARK_STRTRIM_PROT(StrType_)                                             \
	ARKAPI __decl_overloadable__ __decl_flatten__ u0 ark_strtrim (             \
		StrType_* __noalias__ s_                                               \
	);

#define ARK_STRFFO_PROT(StrType_)                                              \
	ARKAPI __decl_overloadable__ Char* ark_strffo (                            \
		StrType_ haystack_, StrType_ needle_                                   \
	);                                                                         \
	ARKAPI __decl_overloadable__ Char* ark_strffo (                            \
		StrType_ haystack_, Char needle_                                       \
	);                                                                         \
	ARKAPI __decl_overloadable__ __decl_flatten__ Char* ark_strffo (           \
		StrType_ haystack_, CString __nonnull__ needle_                        \
	);

#define ARK_STRFLO_PROT(StrType_)                                              \
	ARKAPI __decl_overloadable__ Char* ark_strflo (                            \
		StrType_ haystack_, StrType_ needle_                                   \
	);                                                                         \
	ARKAPI __decl_overloadable__ Char* ark_strflo (                            \
		StrType_ haystack_, Char needle_                                       \
	);                                                                         \
	ARKAPI __decl_overloadable__ __decl_flatten__ Char* ark_strflo (           \
		StrType_ haystack_, CString __nonnull__ needle_                        \
	);

#define ARK_STRFNO_PROT(StrType_)                                              \
	ARKAPI __decl_overloadable__ Char* ark_strfno (                            \
		StrType_ haystack_, StrType_ needle_, sz nth_                          \
	);                                                                         \
	ARKAPI __decl_overloadable__ Char* ark_strfno (                            \
		StrType_ haystack_, Char needle_, sz nth_                              \
	);                                                                         \
	ARKAPI __decl_overloadable__ __decl_flatten__ Char* ark_strfno (           \
		StrType_ haystack_, CString __nonnull__ needle_, sz nth_               \
	);

#define ARK_STRCONCAT_PROT(StrType_)                                           \
	ARKAPI __decl_overloadable__ __decl_nodiscard__ ark_string                 \
	ark_strconcat_fn (                                                         \
		ark_allocator allocator_, StrType_ s1_, StrType_ s2_, ...              \
	);                                                                         \
	ARKAPI __decl_overloadable__ __decl_nodiscard__ ark_string                 \
	ark_strconcat_fn (                                                         \
		ark_allocator allocator_, StrType_ s1_, ark_string_view s2_, ...       \
	);                                                                         \
	ARKAPI __decl_overloadable__ __decl_nodiscard__ ark_string                 \
	ark_strconcat_fn (                                                         \
		ark_allocator allocator_, StrType_ s1_, CString __nonnull__ s2_, ...   \
	);                                                                         \
	ARKAPI __decl_overloadable__ __decl_nodiscard__ ark_string                 \
	ark_strconcat_fn (StrType_ s1_, StrType_ s2_, ...);                        \
	ARKAPI __decl_overloadable__ __decl_nodiscard__ ark_string                 \
	ark_strconcat_fn (StrType_ s1_, CString __nonnull__ s2_, ...);


/*
 * Return a pointer to a dynamic array of ark_bstrings or nullptr
 */
#define ARK_STRSPLIT_PROT(StrType_)                                            \
	ARKAPI __decl_overloadable__ u0* ark_strsplit_fn (                         \
		ark_allocator allocator_, StrType_ s_, ark_string_view pattern_        \
	);                                                                         \
	ARKAPI __decl_overloadable__ __decl_flatten__ u0* ark_strsplit_fn (        \
		ark_allocator allocator_, StrType_ s_, CString __nonnull__ pattern_    \
	);                                                                         \
	ARKAPI __decl_overloadable__ __decl_flatten__ u0* ark_strsplit_fn (        \
		StrType_ s_, ark_string_view pattern_                                  \
	);                                                                         \
	ARKAPI __decl_overloadable__ __decl_flatten__ u0* ark_strsplit_fn (        \
		StrType_ s_, CString __nonnull__ pattern_                              \
	);                                                                         \
	ARKAPI __decl_overloadable__ __decl_flatten__ u0* ark_strsplit_fn (        \
		ark_allocator allocator_, StrType_ s_, Char delim_                     \
	);                                                                         \
	ARKAPI __decl_overloadable__ __decl_flatten__ u0* ark_strsplit_fn (        \
		StrType_ s_, Char delim_                                               \
	);

#define ARK_STRSPLIT_AT_PROT(StrType_)                                         \
	ARKAPI __decl_overloadable__ ark_pair (StrType_) ark_strsplit_at (         \
		ark_allocator allocator_, StrType_ s_, ark_string_view pattern_        \
	);                                                                         \
	ARKAPI __decl_overloadable__ ark_pair (StrType_) ark_strsplit_at (         \
		ark_allocator allocator_, StrType_ s_, Char* __noalias__ delim_        \
	);                                                                         \
	ARKAPI __decl_overloadable__ ark_pair (StrType_)                           \
		ark_strsplit_at (StrType_ s_, ark_string_view pattern_);               \
	ARKAPI __decl_overloadable__ ark_pair (StrType_)                           \
		ark_strsplit_at (StrType_ s_, Char* __noalias__ delim_);

/*
 * interface implementation for wide strings
 */

#define ARK_WIDE_STRING_INTERFACE_PROTOTYPES(StrType_)                         \
	ARK_WIDE_STRING_FUNCTIONS_PROTOTYPES (StrType_)

#define ARK_WIDE_STRING_FUNCTIONS_PROTOTYPES(StrType_)                         \
	ARK_WSTRLCHOP_PROT (StrType_)                                              \
	ARK_WSTRRCHOP_PROT (StrType_)                                              \
	ARK_WSTRLTRIM_PROT (StrType_)                                              \
	ARK_WSTRRTRIM_PROT (StrType_)                                              \
	ARK_WSTRTRIM_PROT (StrType_)                                               \
	ARK_WSTRFFO_PROT (StrType_)                                                \
	ARK_WSTRFLO_PROT (StrType_)

#define ARK_WSTRLCHOP_PROT(StrType_)                                           \
	ARKAPI __decl_overloadable__ char ark_wstrlchop (                          \
		StrType_* __noalias__ s_, sz n_                                        \
	);                                                                         \
	ARKAPI __decl_overloadable__ __decl_flatten__ char ark_wstrlchop (         \
		StrType_* __noalias__ s_                                               \
	);

#define ARK_WSTRRCHOP_PROT(StrType_)                                           \
	ARKAPI __decl_overloadable__ char ark_wstrrchop (                          \
		StrType_* __noalias__ s_, sz n_                                        \
	);                                                                         \
	ARKAPI __decl_overloadable__ __decl_flatten__ char ark_wstrrchop (         \
		StrType_* __noalias__ s_                                               \
	);

#define ARK_WSTRLTRIM_PROT(StrType_)                                           \
	ARKAPI __decl_overloadable__ u0 ark_wstrltrim (StrType_* __noalias__ s_);

#define ARK_WSTRRTRIM_PROT(StrType_)                                           \
	ARKAPI __decl_overloadable__ u0 ark_wstrrtrim (StrType_* __noalias__ s_);

#define ARK_WSTRTRIM_PROT(StrType_)                                            \
	ARKAPI __decl_overloadable__ __decl_flatten__ u0 ark_wstrtrim (            \
		StrType_* __noalias__ s_                                               \
	);

#define ARK_WSTRFFO_PROT(StrType_)                                             \
	ARKAPI __decl_overloadable__ char* ark_wstrffo (                           \
		StrType_ haystack_, StrType_ needdle_                                  \
	);                                                                         \
	ARKAPI __decl_overloadable__ char* ark_wstrffo (                           \
		StrType_ haystack_, WCString __noalias__ needdle_                      \
	);

#define ARK_WSTRFLO_PROT(StrType_)                                             \
	ARKAPI __decl_overloadable__ char* ark_wstrflo (                           \
		StrType_ haystack_, StrType_ needdle_                                  \
	);                                                                         \
	ARKAPI __decl_overloadable__ char* ark_wstrflo (                           \
		StrType_ haystack_, WCString __noalias__ needdle_                      \
	);

#define ARK_WSTRFNO_PROT(StrType_)                                             \
	ARKAPI __decl_overloadable__ char* ark_wstrfno (                           \
		StrType_ haystack_, StrType_ needdle_, sz nth_                         \
	);                                                                         \
	ARKAPI __decl_overloadable__ char* ark_wstrfno (                           \
		StrType_ haystack_, WCString __noalias__ needdle_, sz nth_             \
	);

#define ARK_WSTRCONCAT_PROT(StrType_)                                          \
	ARKAPI __decl_overloadable__ ark_wstring ark_wstrconcat (                  \
		ark_allocator allocator_, StrType_ s1_, StrType_ s2_, ...              \
	);                                                                         \
	ARKAPI __decl_overloadable__ ark_wstring ark_wstrconcat (                  \
		ark_allocator allocator_, StrType_ s1_, WCString s2_, ...              \
	);                                                                         \
	ARKAPI __decl_overloadable__ ark_wstring ark_wstrconcat (                  \
		StrType_ s1_, StrType_ s2_, ...                                        \
	);                                                                         \
	ARKAPI __decl_overloadable__ ark_wstring ark_wstrconcat (                  \
		StrType_ s1_, WCString s2_, ...                                        \
	);

#define ARK_WSTRSPLIT_PROT(StrType_)                                           \
	ARKAPI __decl_overloadable__ ark_darray (ark_wbstring)                     \
		ark_wstrsplit (StrType_ s_, ark_wstring_view pattern_);                \
	ARKAPI __decl_overloadable__ ark_darray (ark_wbstring)                     \
		ark_wstrsplit (StrType_ s_, Char delim_);

#define ARK_WSTRSPLIT_AT_PROT(StrType_)                                        \
	ARKAPI ark_pair (ark_wbstring) ark_wstrsplit_at (StrType_ s_, Char* delim_);

#define ARK_BEGIN_IMPL(StrType_)
#define ARK_END_IMPL(StrType_)

ARK_END

#endif /* ARK_INTEFACES_STRING_H_ */
