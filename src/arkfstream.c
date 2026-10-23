
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>

#include "io/arkfstream.h"

ARK_BEGIN

#if ARK_IS_WIN
	#include <io.h>
	#include <windows.h>
#else
	#include <unistd.h>
#endif

#ifndef O_BINARY
	#define O_BINARY 0
#endif

/* Return the system page size (cached). */
__decl_static_inline__ sz page_size (u0) {
	static sz cached = 0;
	if (ARK_UNLIKELY (!cached)) {
#if ARKLIB_HAS_INCLUDE(<unistd.h>)
		cached = sysconf (_SC_PAGESIZE);
#elif ARKLIB_HAS_INCLUDE(<windows.h>)
		SYSTEM_INFO si;
		GetSystemInfo (&si);
		cached = si.dwPageSize;
#endif
	}
	return cached;
}

static int parse_flags (CString __noalias__ mode_) {
	if (*mode_ == '\0') return -1;
	int flags = 0;
	switch (mode_[0]) {
	case 'r':
		flags |= O_RDONLY;
		break;
	case 'w':
		flags |= O_WRONLY | O_CREAT | O_TRUNC;
		break;
	case 'a':
		flags |= O_WRONLY | O_CREAT | O_APPEND;
		break;
	default:
		return -1;
	}

	for (i8 i = 1; mode_[i] != '\0' && i < 3; ++i) {
		if (mode_[i] == 'b') {
			flags |= O_BINARY;
		} else if (mode_[i] == '+') {
			flags |= (flags & ~O_ACCMODE) | O_RDWR;
		}
	}
	return flags;
}

ARKEXPORT ark_file*
ark_fstream_open (CString __noalias__ fpath_, CString __noalias__ mode_) {
	ark_file* f = ARK_MALLOC (sizeof (ark_file));
	if (!f) return nullptr;

	int flags = parse_flags (mode_);
	if (flags == -1) {
		ARK_PANIC ("failed to resolve mode");
		ark_dealloc (f);
		return nullptr;
	}
	int perms = 0;
	if (flags & O_CREAT) {
		perms |= S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
	}

	f->_fd = open (fpath_, flags, perms);
	if (f->_fd == -1) {
		ARK_WARN ("failed to open file: %s", fpath_);
		ark_dealloc (f);
		return nullptr;
	}

	if (stat (fpath_, &f->_sb) == -1) {
		ARK_WARN ("stat failed");
		ark_dealloc (f);
		return nullptr;
	}

	f->_path = fpath_;
	f->_data = MAP_FAILED;

	if (f->_sb.st_size <= (__off_t)page_size ()) {
		int mprot  = 0;
		int mflags = 0;
		if (flags & O_RDWR) {
			mprot  = PROT_READ | PROT_WRITE;
			mflags = MAP_SHARED;
		} else if (flags & O_RDONLY) {
			mprot  = PROT_READ;
			mflags = MAP_PRIVATE;
		} else if (flags & O_WRONLY) {
			mprot  = PROT_WRITE;
			mflags = MAP_SHARED;
		}
		f->_data = mmap (nullptr, page_size (), mprot, mflags, f->_fd, 0);
		if (f->_data == MAP_FAILED) {
			ARK_WARN ("failed to map file into memory");
		}
	}

	return f;
}

ARKEXPORT u0 ark_fstream_close (ark_file* __restrict__ f_) {
	if (!f_) return;

	close (f_->_fd);
	if (f_->_data != MAP_FAILED) munmap (f_->_data, f_->_sb.st_size);
	ark_dealloc (f_);
}

ARKEXPORT __decl_overloadable__ u0
ark_fstream_write (ark_file* __noalias__ f_, ark_string_view buff_) {
	if (f_->_data != MAP_FAILED) {
		ARK_MEMCPY (f_->_data, buff_._data, buff_._len);
	}
	if (stat (f_->_path, &f_->_sb) == -1) {
		ARK_WARN ("stat failed after write");
	}
}

ARKEXPORT __decl_overloadable__ u0
ark_fstream_write (ark_file* __noalias__ f_, CString buff_) {
	return ark_fstream_write (f_, ark_make_string_view (buff_));
}

ARKEXPORT ssz ark_fstream_read (ark_file*, u0* __noalias__ buff_, sz nbytes_);

ARK_END
