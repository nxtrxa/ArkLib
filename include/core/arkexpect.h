/*
 * Copyright (c) 2026 Murilo Gonzalez. All Rights Reserved.
 *
 *                      ARKLIB EXPECT
 *
 *  Declare a function that expects to return a specific
 *  type or an enum type fault. use `ark_faultdefs` to declare
 *
 *  Zero-overhead model
 *  -------------------
 *  An ark_expect (T) is just { T unwrap; int _fault; }: the payload plus a 4-byte code
 *  (0 == success). For a pointer payload that is 16 bytes, returned in two registers; nothing is
 *  built, copied or stored on the success path.
 *
 *  Everything else (fault name, message, context, file, line, errno) is diagnostics:
 *    - the strings are `static const` data, one record per throw site (rodata, no runtime cost);
 *    - throwing makes ONE cold call that remembers the site and errno in a thread-local slot;
 *    - the full ark_exception object is only materialised on demand, in cold code
 *      (ark_expect_exception / ark_catch).
 *
 *  Define ARK_FAULT_MINIMAL to drop even that: throw sites then emit no strings and no call,
 *  the code is all that exists (ark_expect_exception then reports only the numeric value).
 *
 *  The thread-local slot behaves like errno: read it right after the failure, before another
 *  throw on the same thread overwrites it. Messages must be string literals.
 */

#ifndef ARKLIB_CORE_EXPECT_H_
#define ARKLIB_CORE_EXPECT_H_

#import "internals/arkdefs.h"

ARK_BEGIN

#ifdef ARK_NAMESPACE
	#define FAULT	   ARK_FAULT
	#define fault	   ark_fault
	#define faultdefs  ark_faultdefs
	#define expect	   ark_expect
	#define expected   ark_expected
	#define unexpected ark_unexpected
	#define unwrap_or  ark_unwrap_or
#endif

#define ARK_FAULT_ID(Fault_) ark_pp_concat (ARK_EXPECT_CONTEXT, _, Fault_)

#define ARK_DECL_FAULT_SEQ(Fault_) ARK_FAULT_ID (Fault_) = __COUNTER__ + 1,

#define ARK_DECL_FAULT_TABLE(Fault_) [ARK_FAULT_ID (Fault_)] = ark_pp_stringfy (ARK_FAULT_ID (Fault_)),


#define ark_expect_typename struct ark_pp_concat (ARK_EXPECT_, ARK_EXPECT_CONTEXT)

#define ark_expect_ptr_typename struct ark_pp_concat (ARK_EXPECT_PTR_, ARK_EXPECT_CONTEXT)

#define ark_fault_table ark_pp_concat (ARK_EXPECT_CONTEXT, _FAULTS_TABLE)

#define ark_fault_t enum ark_pp_concat (ARK_EXPECT_CONTEXT, _FAULTS)

/*
 * Declare an enum of fault values that belong to a specific context.
 * The final symbol name is prefixed by the context. Therefore ARK_EXPECT_CONTEXT
 * macro must be defined before this macro is used.
 */
#define ark_faultdefs(...)                                                                                             \
	enum ark_pp_concat (ARK_EXPECT_CONTEXT, _FAULTS) { ark_pp_foreach (ARK_DECL_FAULT_SEQ, __VA_ARGS__) }

// static CString ark_expect_table[] = {ark_pp_foreach (ARK_DECL_FAULT_TABLE, __VA_ARGS__)}

static constexpr int ARK_FAULT_SUCCESS = 0;

ARK_CLASS (ark_fault) {
	CString to_str;
	CString _file;
	CString _func;
	CString _ctx;
	CString _msg;
	u32		_line;
	int		_errno;
	int		_value;
};

/*
 * Declare a struct type with an expected return type + an error type.
 */
#define ark_expect(Expected_Type_)                                                                                     \
	ark_expect_typename {                                                                                              \
		Expected_Type_ unwrap;                                                                                         \
		ark_fault_t	   _fault;                                                                                         \
	}

#define ark_expect_ptr(PtrType_)                                                                                       \
	ark_expect_ptr_typename {                                                                                          \
		PtrType_* unwrap;                                                                                              \
	}

#define ark_expected(Expected_)                                                                                        \
	(ark_expect_typename) {                                                                                            \
		.unwrap = (Expected_),                                                                                         \
	}

#define ark_unexpected(...) ark_pp_concat (ark_unexpected_, ark_pp_nargs (__VA_ARGS__)) (__VA_ARGS__)

#define ark_unexpected_1(Fault_)                                                                                       \
	({                                                                                                                 \
		ARK_FAULT = (ark_fault) {                                                                                      \
			.to_str = #Fault_,                                                                                         \
			._file	= $file,                                                                                           \
			._func	= $fnsig,                                                                                          \
			._line	= $line,                                                                                           \
			._ctx	= ark_pp_stringfy (ARK_EXPECT_CONTEXT),                                                            \
			._errno = errno,                                                                                           \
			._value = ARK_FAULT_ID (Fault_),                                                                           \
		};                                                                                                             \
		(ark_expect_typename) {                                                                                        \
			._fault = ARK_FAULT_ID (Fault_),                                                                           \
		};                                                                                                             \
	})

#define ark_unexpected_2(Fault_, Msg_)                                                                                 \
	({                                                                                                                 \
		ARK_FAULT = (ark_fault) {                                                                                      \
			.to_str = #Fault_,                                                                                         \
			._file	= $file,                                                                                           \
			._func	= $fnsig,                                                                                          \
			._line	= $line,                                                                                           \
			._ctx	= ark_pp_stringfy (ARK_EXPECT_CONTEXT),                                                            \
			._msg	= (Msg_),                                                                                          \
			._errno = errno,                                                                                           \
			._value = ARK_FAULT_ID (Fault_),                                                                           \
		};                                                                                                             \
		(ark_expect_typename) {                                                                                        \
			._fault = ARK_FAULT_ID (Fault_),                                                                           \
		};                                                                                                             \
	})

#define ark_expect_on_succes(Fault_) $likely ((Fault_) == ARK_FAULT_SUCCESS)
#define ark_expect_on_fault(Fault_)	 $unlikely ((Fault_) == ARK_FAULT_SUCCESS)

#define $catch(FnCall_, ExceptObj_, ...) __ark_catch_ (ark_pp_unique_id (__result_), FnCall_, (ExceptObj_), __VA_ARGS__)

#define __ark_catch_(R_, FnCall_, ExceptObj_, ...)                                                                     \
	({                                                                                                                 \
		auto R_ = (FnCall_);                                                                                           \
		ark_expect_on_fault (R_._fault) {                                                                              \
			__decl_unused__ auto ExceptObj_ = ARK_FAULT;                                                               \
			__VA_ARGS__;                                                                                               \
			ARK_TRAP ();                                                                                               \
		}                                                                                                              \
		R_.unwrap;                                                                                                     \
	})

ARKAPI thread_local ark_fault ARK_FAULT;

// __decl_nodiscard__ __decl_cold__ __decl_pure__ __decl_static_inline__ ark_fault ark_raise_fault (int value_) {
// 	return ARK_FAULT;
// }


ARK_END


#endif /* ARKLIB_CORE_EXPECT_H_ */
