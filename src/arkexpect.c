#include <errno.h>

#include "core/arkexpect.h"

ARK_BEGIN

ARKEXPORT thread_local __ark_exception_last_t_ ark_exception_last;

ARKEXPORT u0 ark_exception_raise (const __ark_exception_site_t_* site_, int value_) {
	/* errno is read here, once, on the failure path only. */
	ark_exception_last = (__ark_exception_last_t_) {
		._site	= site_,
		._value = value_,
		._errno = errno,
	};
}

ARKEXPORT ark_exception ark_exception_from (int value_) {
	const __ark_exception_last_t_* last = &ark_exception_last;

	if (last->_site && last->_value == value_) {
		return (ark_exception) {
			._to_str = last->_site->_to_str,
			._msg	 = last->_site->_msg,
			._ctx	 = last->_site->_ctx,
			._file	 = last->_site->_file,
			._value	 = value_,
			._errno	 = last->_errno,
			._line	 = last->_site->_line,
		};
	}

	/* Stale or minimal build: only the numeric value is known. */
	return (ark_exception) {
		._to_str = "unknown",
		._msg	 = "",
		._ctx	 = "unknown",
		._file	 = "",
		._value	 = value_,
	};
}

ARK_END
