#include <errno.h>

#include "core/arkexpect.h"

ARK_BEGIN

ARKEXPORT thread_local ark_fault ark_fault_last = {};

ARKEXPORT u0 __decl_overloadable__
ark_expect_set_fault (CString to_str_, CString file_, CString fnsig_, CString ctx_, u32 line_, int value_) {
	ark_fault_last = (ark_fault) {
		.to_str = to_str_,
		._file	= file_,
		._func	= fnsig_,
		._line	= line_,
		._ctx	= ctx_,
		._errno = errno,
		._value = value_,
	};
}

ARKEXPORT u0 __decl_overloadable__ ark_expect_set_fault (
	CString to_str_, CString file_, CString fnsig_, CString ctx_, CString msg_, u32 line_, int value_
) {
	ark_fault_last = (ark_fault) {
		.to_str = to_str_,
		._file	= file_,
		._func	= fnsig_,
		._line	= line_,
		._ctx	= ctx_,
		._msg	= msg_,
		._errno = errno,
		._value = value_,
	};
}

ARKEXPORT ark_fault ark_expect_fault (u0) {
	return ark_fault_last;
}

ARK_END
