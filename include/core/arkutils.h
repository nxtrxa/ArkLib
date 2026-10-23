#ifndef ARK_UTILS_H_
#define ARK_UTILS_H_

#import "internals/arkdefs.h"

ARK_BEGIN

#ifdef ARK_NAMESPACE

#endif

ARK_CLASS (ark_args) {
	char* argv;
	char* env;
	int	  argc;
};

__decl_overloadable__ __decl_static_inline__ ark_args ark_shift_args (int* argc, char*** argv, char*** env) {
	char* arg = nullptr;
	char* e	  = nullptr;
	if (*argc > 0) {
		arg = **argv;
		(*argv)++;
		(*argc)--;
	}
	if (*env) {
		e = (**env);
		(*env)++;
	}
	return (ark_args) {arg, e, *argc};
}

__decl_overloadable__ __decl_static_inline__ char* ark_shift_args (int* argc, char*** argv) {
	if (*argc > 0) {
		char* arg = **argv;
		(*argv)++;
		(*argc)--;
		return arg;
	}
	return nullptr;
}

__decl_static_inline__ sz ark_xstrlen (CString __nonnull__ s_) {
	CString cp = s_;
	while (*cp) {
		cp++;
	}
	return cp - s_;
}

ARK_END

#endif /* ARK_UTILS_H_ */
