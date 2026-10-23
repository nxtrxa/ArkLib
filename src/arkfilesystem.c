#define _XOPEN_SOURCE 700
#import "allocators/arkarena_allocator.h"
#import "allocators/arkmonotonic_buffer.h"
#import "core/arkbstring.h"
#import "core/arkstring.h"
#import "core/arkstring_view.h"
#import "ftw.h"
#import "io/arkfilesystem.h"
#import "math/arkpair.h"

ARK_BEGIN

static constexpr u16 MAX_PATH_LEN = 4096;

ARK_ENUM (path_intents) {
	PATH_INTENT_FILE,
	PATH_INTENT_DIRECTORY,
};

struct path {
	CString _raw;
	int		_fd;
};

struct file {
	struct directory* _parent;
	struct path		  _path;
	ark_string_view	  _extension;
	u0*				  _data;
	FILE*			  _handler;
};

struct directory {
	struct path		  _path;
	struct directory* _parent;
	sz				  children_count;
	struct file*	  children[] __attr_counted_by__ (children_count);
};

struct tree {
	struct path root;
	sz			dir_count;
	struct path children[] __attr_counted_by__ (dir_count);
};

ARKINTERNAL int unlink_callback (
	CString path_, struct stat* sb_, int type_flag_, struct FTW* ftw_buff_
) {
	return remove (path_);
}

ARKEXPORT __decl_overloadable__ int
ark_fsys_mkdir (CString __noalias__ path, mode_t mode) {
	if (!path || *path == '\0') {
		errno = EINVAL;
		return -1;
	}

	ark_monotonic_buffer (mbuff);
	ark_string tmp
		= ark_make_string (ARK_MONOTONIC_BUFFER_ALLOCATOR (mbuff), path);
	if (ark_string_back (tmp) == '/') {
		while (tmp._len > 1 && ark_string_back (tmp) == '/')
			tmp._data[--tmp._len] = '\0';
	}

	$foreach (p $in_string (tmp)) {
		if (*p == '/') {
			*p = '\0';
			if (mkdir (tmp._data, mode) != 0) {
				if (errno != EEXIST) {
					return -1;
				}
				struct stat st;
				if (stat (tmp._data, &st) != 0 || !S_ISDIR (st.st_mode)) {
					errno = ENOTDIR;
					return -1;
				}
			}
			*p = '/';
		}
	}
	if (mkdir (tmp._data, mode) != 0) {
		if (errno != EEXIST) {
			return -1;
		}
		struct stat st;
		if (stat (tmp._data, &st) != 0 || !S_ISDIR (st.st_mode)) {
			errno = ENOTDIR;
			return -1;
		}
	}
	return 0;
}

ARKEXPORT __decl_overloadable__ int ark_fsys_mkfile (
	CString __noalias__ path_, CString __noalias__ modes_, mode_t dir_mode_
) {
	sz path_len = ARK_STRLEN (path_);
	if (path_[path_len - 1] == '/') {
		ARK_PANIC ("passing directory path (%s) to mkfile", path_);
	}
	if (ARK_STRRCHR (path_, '/')) {
		ark_monotonic_buffer (mbuff);
		ark_string path = ark_make_string (
			ARK_MONOTONIC_BUFFER_ALLOCATOR (mbuff), path_, path_len
		);
		ark_pair (ark_string)
			split = ark_strsplit_at (path, ark_strflo (path, "/"));
		if (ark_fsys_mkdir (
				split.first._data,
				(dir_mode_) ? dir_mode_ : ARK_FSYS_DIR_DEFAULT_MODE
			)
			!= 0) {
			ARK_WARN ("failed to create directory: %s", &split.first);
			return -1;
		}
	}
	FILE* f = fopen (path_, modes_);
	if (!f) {
		ARK_WARN ("failed to crate file: < %s >", path_);
		return -1;
	} else {
		fclose (f);
		return 0;
	}
}

ARKEXPORT int ark_fsys_rmrf (CString path_) {
	int flags = FTW_DEPTH | FTW_PHYS;
	if (nftw (path_, (__nftw_func_t)unlink_callback, 64, flags) != 0) return -1;
	else
		return 0;
}

ARKEXPORT int
ark_fsys_mktree_fn (CString __noalias__ parent_, CString child1_, ...) {
	if (ark_fsys_mkdir (parent_, ARK_FSYS_DIR_DEFAULT_MODE) != 0) {
		return -1;
	}

	ark_monotonic_buffer (mbuff);

	ark_darray (CString) children
		= ark_make_darray (CString, ARK_MONOTONIC_BUFFER_ALLOCATOR (mbuff), 2)
			  .unwrap;
	ark_darray_push_back (children, child1_);

	va_list args;
	va_start (args);
	CString nth_arg = va_arg (args, CString);
	while (nth_arg) {
		ark_darray_push_back (children, nth_arg);
		nth_arg = va_arg (args, CString);
	}
	va_end (args);

	ark_string _parent
		= ark_make_string (ARK_MONOTONIC_BUFFER_ALLOCATOR (mbuff), parent_);
	if (ark_string_back (_parent) != '/') ark_strrappend (&_parent, '/');
	ark_string nth_child;
	int		   ok;

	$foreach (child $in_darray (children)) {
		nth_child = ark_strconcat (
			ARK_MONOTONIC_BUFFER_ALLOCATOR (mbuff), _parent, *child
		);
		if (ark_string_back (nth_child) == '/') {
			ok = ark_fsys_mkdir (nth_child._data);
		} else {
			ok = ark_fsys_mkfile (nth_child._data, "w");
		}
		if (ok != 0) return -1;
	}
	return 0;
}

ARKEXPORT FILE*
ark_fsys_tree_join (CString parent_, CString __noalias__ child_) { }


ARK_END
