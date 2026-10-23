#if 0
	#include "core/arkbasic_string.h"
	#include "core/arkstring.h"

ARK_BEGIN

ARKEXPORT __decl_overloadable__ ark_bstring ark_make_bstring (CString __noalias__ cstr_) {
	return (ark_bstring) {
		.data = strdup (cstr_),
		.len  = strlen (cstr_),
	};
}

ARKEXPORT __decl_overloadable__ ark_bstring ark_make_bstring (CString __noalias__ cstr_, size_t len_) {
	return (ark_bstring) {
		.data = strdup (cstr_),
		.len  = len_,
	};
}

ARKEXPORT u0 ark_destroy_bstring (ark_bstring* __noalias__ bs_) {
	if (bs_ && bs_->data) free (bs_->data);
}

ARKEXPORT __decl_overloadable__ u0 ark_bstrcopy (ark_bstring* __noalias__ dest_, CString src_) {
	ark_destroy_bstring (dest_);
	*dest_ = ark_make_bstring (src_);
}

ARKEXPORT __decl_overloadable__ u0 ark_bstrcopy (ark_bstring* __noalias__ dest_, ark_string_view src_) {
	ark_destroy_bstring (dest_);
	*dest_ = ark_bstring_from (src_);
}

ARKEXPORT __decl_overloadable__ u0 ark_bstrcopy (ark_bstring* __noalias__ dest_, ark_string src_) {
	ark_destroy_bstring (dest_);
	*dest_ = ark_bstring_from (src_);
}

ARK_END
#endif
