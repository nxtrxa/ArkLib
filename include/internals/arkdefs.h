#ifndef ARKDEFS_H_
#define ARKDEFS_H_

/* Standard Library Prerequisites */
#import <alloca.h>
#import <assert.h>
#import <errno.h>
#import <stdalign.h>
#import <stdarg.h>
#import <stdbool.h>
#import <stdcountof.h>
#import <stddefer.h>
#import <stdlib.h>

#import "__arklib_attributes_.h"
#import "__arklib_bits_.h"
#import "__arklib_builtins_.h"
#import "__arklib_crt_.h"
#import "__arklib_ctl_flow_.h"
#import "__arklib_diag_report_.h"
#import "__arklib_features_.h"
#import "__arklib_pputils_.h"
#import "__arklib_sanitizer_.h"
#import "__arklib_type_reflection_.h"
#import "__arklib_type_traits_.h"
#import "__arklib_types_.h"

// #import "llvm/ptrcheck.h"

ARK_BEGIN

#ifdef ARK_NAMESPACE
	#define BEGIN	 ARK_BEGIN // Without the BEGIN macro, complier will generate a huge amount of warnings.
	#define atomic_t ark_atomic_t
#endif

#define __decls__(...) ark_pp_foreach (__ark__decls_, __VA_ARGS__)
#define __attrs__(...) ark_pp_foreach (__ark__attrs_, __VA_ARGS__)

#define __ark__decls_(Decl_) ark_pp_concat (__decl_, Decl_, __)
#define __ark__attrs_(Attr_) ark_pp_concat (__attr_, Attr_, __)

typedef $fn (int, ark_compar_fn_t) (U0*, U0*);

__decl_cold__ __decl_static_inline__ u0 ark_unused (...) { }

ARK_END

#endif /* ARKDEFS_H_ */
