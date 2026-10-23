#ifndef ARK_INTERNALS_DIAG_REPORT_H_
#define ARK_INTERNALS_DIAG_REPORT_H_

#import <errno.h>
#import <stdarg.h>
#import <stdio.h>

#import "__arklib_sanitizer_.h"

ARK_BEGIN

#ifdef ARK_NAMESPACE
	#define INFO				ARK_INFO
	#define WARN				ARK_WARN
	#define PANIC				ARK_PANIC
	#define report_allocation	ark_report_allocation
	#define report_reallocation ark_report_reallocation
	#define report_deallocation ark_report_deallocation
#endif

/* Assertions */
#ifndef $static_assert
	#define $static_assert __extension__ static_assert
#endif

#define $assert(Assertion_) ((Assertion_) ? nullptr : ___ark__assert_fail__ (#Assertion_, $file, $function, $line))

#if ARKLIB_DEBUG_BUILD
	#define $assert_internal $assert
#else
	#define $assert_internal(...)
#endif

#if ARKLIB_DEBUG_BUILD
	#define ARK_DCHECK(Check_, ...)                                                                                    \
		if (Check_) __VA_ARGS__
#else
	#define ARK_DCHECK(...)
#endif

/* Diagnostic Logging Macros */
#if ARKLIB_ENABLED_INFO
	#define ARK_INFO(...) ___ark__info__ ($file_path, $fnsig, $line, __VA_ARGS__);
#else
	#define ARK_INFO(...)
#endif

#if ARKLIB_ENABLED_WARNINGS
	#define ARK_WARN(...) ___ark__warn__ ($file_path, $fnsig, $line, __VA_ARGS__)
#else
	#define ARK_WARN(...)
#endif

#define ARK_PANIC(...) ___ark__panic__ ($file_path, $fnsig, $line, __VA_ARGS__)

#define ARK_LOG(...) ___ark__log__ ($file_path, $fnsig, $line, __VA_ARGS__)

#define ark_internal_symbol(Sym_) ark_pp_concat (___ark__, Sym_, __)

/* Allocation Tracing Definitions */
#define ARK_HEAP_ALLOCATION	 1
#define ARK_STACK_ALLOCATION 2

#if ARKLIB_REPORT_ALLOCATIONS
	#define ark_report_allocation(AllocType_, Ptr_, Alloc_Sz, Align_, ...)                                             \
		___ark__report_allocation__ (                                                                                  \
			(AllocType_), (Ptr_), (Alloc_Sz), (Align_), $file, $function, $line __VA_OPT__ (, ) __VA_ARGS__            \
		)

	#define ark_report_reallocation(AllocType_, Old_Ptr_, New_Ptr_, Old_Sz_, New_Sz_, Align_, ...)                     \
		___ark__report_reallocation__ (                                                                                \
			(AllocType_),                                                                                              \
			(Old_Ptr_),                                                                                                \
			(New_Ptr_),                                                                                                \
			(Old_Sz_),                                                                                                 \
			(New_Sz_),                                                                                                 \
			(Align_),                                                                                                  \
			$file,                                                                                                     \
			$function,                                                                                                 \
			$line __VA_OPT__ (, ) __VA_ARGS__                                                                          \
		)

	#define ark_report_deallocation(Ptr_, ...)                                                                         \
		___ark__report_deallocation__ ((Ptr_), $file, $function, $line __VA_OPT__ (, ) __VA_ARGS__)
#else
	#define ark_report_allocation(...)
	#define ark_report_reallocation(...)
	#define ark_report_deallocation(...)
#endif

/* Inline Reporting & Diagnostics Handlers */
__decl_noreturn__ __decl_cold__ __decl_noinline__ static inline u0* __nonnull__ ___ark__panic__ (
	CString __noalias__ file_, CString __noalias__ func_, unsigned line_, CString __noalias__ fmt_, ...
) __attr_format__ (printf, 4, 5) {
	fflush (stderr);
	fprintf (stderr, "\n[PANIC] %s:%d in function %s:\n\t", file_, line_, func_);
	va_list args;
	va_start (args);
	vfprintf (stderr, fmt_, args);
	va_end (args);
	(errno != 0) ? perror ("\n\terrno") : (u0)putchar ('\n');
	putchar ('\n');
	abort ();
}

__decl_cold__ __decl_noinline__ static inline u0* __nullable__ ___ark__warn__ (
	CString __noalias__ file_, CString __noalias__ func_, unsigned line_, CString __noalias__ fmt_, ...
) __attr_format__ (printf, 4, 5) {
	fflush (stderr);
	fprintf (stderr, "\n[WARNING] %s:%d in function %s:\n\t  ", file_, line_, func_);
	va_list args;
	va_start (args);
	vfprintf (stderr, fmt_, args);
	va_end (args);
	(errno != 0) ? perror ("\n\t  errno") : (u0)puts ("\n\t  errno not set");
	putchar ('\n');
	return nullptr;
}

__decl_cold__ __decl_noinline__ static inline u0* __nullable__ ___ark__info__ (
	CString __noalias__ file_, CString __noalias__ func_, unsigned line_, CString __noalias__ fmt_, ...
) __attr_format__ (printf, 4, 5) {
	fflush (stdout);
	printf ("\n[INFO] %s:%d in function %s:\n\t  ", file_, line_, func_);
	va_list args;
	va_start (args);
	vfprintf (stderr, fmt_, args);
	va_end (args);
	putchar ('\n');
	return nullptr;
}

__decl_cold__ __decl_noinline__ static inline u0* __nullable__ ___ark__log__ (
	CString __noalias__ file_, CString __noalias__ func_, unsigned line_, CString __noalias__ fmt_, ...
) __attr_format__ (printf, 4, 5) {
	fflush (stdout);
	printf ("\n[INFO] %s:%d in function %s:\n\t  ", file_, line_, func_);
	va_list args;
	va_start (args);
	vfprintf (stderr, fmt_, args);
	va_end (args);
	putchar ('\n');
	return nullptr;
}

__decl_noreturn__ __decl_cold__ __decl_noinline__ static inline u0* __nonnull__ ___ark__assert_fail__ (
	CString __noalias__ assertion_, CString __noalias__ file, CString __noalias__ func_, unsigned line_
) {
	fprintf (stderr, "assertion: %s failed in: %s:%u in function %s\n", assertion_, file, line_, func_);
	fflush (stderr);
	abort ();
}

__decl_overloadable__ __decl_noinline__ static inline u0 ___ark__report_allocation__ (
	int					alloc_type_,
	u0* __noalias__		ptr_,
	sz					alloc_sz_,
	sz					alignment_,
	CString __noalias__ file_,
	CString __noalias__ function_,
	u32					line_,
	CString __noalias__ fmt_,
	...
) {
	if (alloc_type_ == ARK_HEAP_ALLOCATION) {
		printf ("\n[HEAP ALLOCATION REPORT] %s:%d in function %s:\n", file_, line_, function_);
	} else if (alloc_type_ == ARK_STACK_ALLOCATION) {
		printf ("\n[STACK ALLOCATION REPORT] %s:%d in function %s:\n", file_, line_, function_);
	} else {
		printf (
			"\n[ALLOCATION REPORT (ALLOCATION TYPE NOT SPECIFIED)] %s:%d in function %s:\n", file_, line_, function_
		);
	}
	printf ("Allocation of %zu bytes with alignment of %zu at %p\n", (size_t)alloc_sz_, (size_t)alignment_, ptr_);
	va_list args;
	va_start (args);
	vprintf (fmt_, args);
	putchar ('\n');
}

__decl_overloadable__ __decl_noinline__ static inline u0 ___ark__report_allocation__ (
	int					alloc_type_,
	u0* __noalias__		ptr_,
	sz					alloc_sz_,
	sz					alignment_,
	CString __noalias__ file_,
	CString __noalias__ function_,
	u32					line_
) {
	return ___ark__report_allocation__ (alloc_type_, ptr_, alloc_sz_, alignment_, file_, function_, line_, "");
}

__decl_overloadable__ __decl_noinline__ static inline u0 ___ark__report_reallocation__ (
	int					alloc_type_,
	u0* __noalias__		old_ptr_,
	u0* __noalias__		new_ptr_,
	sz					old_sz_,
	sz					new_sz_,
	sz					alignment_,
	CString __noalias__ file_,
	CString __noalias__ function_,
	u32					line_,
	CString __noalias__ fmt_,
	...
) {
	if (alloc_type_ == ARK_HEAP_ALLOCATION) {
		printf ("\n[HEAP REALLOCATION REPORT] %s:%d in function %s:\n", file_, line_, function_);
	} else if (alloc_type_ == ARK_STACK_ALLOCATION) {
		printf ("\n[STACK REALLOCATION REPORT] %s:%d in function %s:\n", file_, line_, function_);
	} else {
		printf (
			"\n[ALLOCATION REPORT (ALLOCATION TYPE NOT SPECIFIED)] %s:%d in function %s:\n", file_, line_, function_
		);
	}
	printf (
		"Reallocation of block of %zu bytes to %zu bytes with alignment of %zu from %p to "
		"%p\n",
		(size_t)old_sz_,
		(size_t)new_sz_,
		(size_t)alignment_,
		(u0*)old_ptr_,
		(u0*)new_ptr_
	);
	va_list args;
	va_start (args);
	vprintf (fmt_, args);
	putchar ('\n');
}

__decl_overloadable__ __decl_noinline__ static inline u0 ___ark__report_reallocation__ (
	int					alloc_type_,
	u0* __noalias__		old_ptr_,
	u0* __noalias__		new_ptr_,
	sz					old_sz_,
	sz					new_sz_,
	sz					alignment_,
	CString __noalias__ file_,
	CString __noalias__ function_,
	u32					line_
) {
	return ___ark__report_reallocation__ (
		alloc_type_, old_ptr_, new_ptr_, old_sz_, new_sz_, alignment_, file_, function_, line_, ""
	);
}

__decl_overloadable__ __decl_noinline__ static inline u0 ___ark__report_deallocation__ (
	u0* __noalias__		ptr_,
	CString __noalias__ file_,
	CString __noalias__ function_,
	u32					line_,
	CString __noalias__ fmt_,
	...
) {
	printf ("\n[DEALLOCATION REPORT] %s:%d in function %s:\n", file_, line_, function_);
	printf ("Pointer: %p freed\n", ptr_);
	va_list args;
	va_start (args);
	vprintf (fmt_, args);
	putchar ('\n');
}

__decl_overloadable__ __decl_noinline__ static inline u0 ___ark__report_deallocation__ (
	u0* __noalias__ ptr_, CString __noalias__ file_, CString __noalias__ function_, u32 line_
) {
	return ___ark__report_deallocation__ (ptr_, file_, function_, line_, "");
}

ARK_END

#endif /* ARK_INTERNALS_DIAG_REPORT_H_ */
