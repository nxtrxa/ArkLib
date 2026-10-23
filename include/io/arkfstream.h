#ifndef ARK_FSTREAM_H_
#define ARK_FSTREAM_H_

#include <sys/stat.h>

#include "core/arkstring_view.h"

ARK_CLASS (ark_file) {
	cstring		_data;
	CString		_path;
	int			_fd;
	struct stat _sb;
};

ARK_BEGIN

ARKAPI ark_file* ark_fstream_open (CString __noalias__ fpath_, CString mode_);

ARKAPI u0 ark_fstream_close (ark_file* __restrict__ f_);

ARKAPI __decl_overloadable__ u0
ark_fstream_write (ark_file* __noalias__ f_, ark_string_view buff_);

ARKAPI __decl_overloadable__ u0
ark_fstream_write (ark_file* __noalias__ f_, CString buff_);

ARKAPI ssz ark_fstream_read (ark_file*, u0* __noalias__ buff_, sz nbytes_);

ARK_END

#endif /* ARK_FSTREAM_H_ */
