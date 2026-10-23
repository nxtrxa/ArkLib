#ifndef ARK_FSYS_H_
#define ARK_FSYS_H_

#import <fcntl.h>
#import <sys/mman.h>
#import <sys/stat.h>
#import <sys/types.h>
#import <unistd.h>

#import "internals/arkdefs.h"

ARK_BEGIN

ARK_ENUM (ARK_FSYS_DIR_MODES) {
	ARK_FSYS_DIR_DEFAULT_MODE = (S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH), // 0o755
#define ARK_FSYS_DIR_DEFAULT_MODE ARK_FSYS_DIR_DEFAULT_MODE
	ARK_FSYS_FILE_DEFAULT_MODE = (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH), // 0o644
#define ARK_FSYS_FILE_DEFAULT_MODE ARK_FSYS_FILE_DEFAULT_MODE
};

typedef struct path ark_fsys_path;

typedef struct file ark_fsys_file;

typedef struct directory ark_fsys_directory;

typedef struct tree ark_fsys_tree;

#define ark_fsys_mktree(Parent_, Child1_, ...)                                                                         \
	ark_internal_symbol (fsys_mktree_fn) (Parent_, Child1_, ##__VA_ARGS__, nullptr)


ARKAPI __decl_overloadable__ int ark_fsys_mkdir (CString __noalias__ path_, mode_t mode_);

__decl_static_inline__ __decl_overloadable__ int ark_fsys_mkdir (CString __noalias__ path_) {
	return ark_fsys_mkdir (path_, ARK_FSYS_FILE_DEFAULT_MODE);
}

ARKAPI __decl_overloadable__ int
ark_fsys_mkfile (CString __noalias__ path_, CString __noalias__ mode_, mode_t dir_mode_);

__decl_static_inline__ __decl_overloadable__ int
ark_fsys_mkfile (CString __noalias__ path_, CString __noalias__ modes_) {
	return ark_fsys_mkfile (path_, modes_, ARK_FSYS_DIR_DEFAULT_MODE);
}

ARKAPI int ark_fsys_rmrf (CString path_);

ARKAPI __decl_sentinel__ int ark_fsys_mktree_fn (CString __noalias__ parent_, CString __noalias__ child1_, ...);

ARKAPI FILE* ark_fsys_tree_join (CString __noalias__ parent_, CString __noalias__ child_);


ARK_END

#endif /* ARK_FSYS_H_ */
