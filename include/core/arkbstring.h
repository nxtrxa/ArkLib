#ifndef ARK_CORE_BSTRING_H_
#define ARK_CORE_BSTRING_H_

#import "interfaces/arkstring_interface.h"

ARK_BEGIN

ARK_CLASS (ark_bstring) {
	cstring _data;
	sz		_len;
};

#define ark_class_bstring(S_, ...)                                             \
	__attr_cleanup__ (ark_bstring_cleanup)                                     \
		ark_bstring S_ = ark_make_bstring (__VA_ARGS__)

#define ark_null_bstring()                                                     \
	(ark_bstring) {                                                            \
		._data = nullptr, ._len = 0,                                           \
	}

#define ark_bstring_from(Str_)                                                 \
	({                                                                         \
		auto ___str__ = (Str_);                                                \
		ark_make_bstring (___str__._data, ___str__._len);                      \
	})

ARKAPI __decl_overloadable__ ark_bstring
ark_make_bstring (CString __noalias__ cstr_, sz len_);

ARKAPI __decl_overloadable__ ark_bstring
ark_make_bstring (CString __noalias__ cstr_);

ARKAPI u0 ark_destroy_bstring (ark_bstring* __noalias__ bstr_);

#define ark_destroy_bstrings(...)                                              \
	do {                                                                       \
		u0* const ___ss__[] = {__VA_ARGS__};                                   \
		$for (i, ARK_NARGS (__VA_ARGS__)) {                                    \
			ark_destroy_bstring (___ss__[i]);                                  \
		}                                                                      \
	} while (0)

__decl_static_inline__ u0 ark_bstring_cleanup (ark_bstring* bstr_) {
	ark_destroy_bstring (bstr_);
}

ARK_UTF8_STRING_INTERFACE_PROTOTYPES (ark_bstring)

ARK_END

#endif /* ARK_CORE_BSTRING_H_ */
