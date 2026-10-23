/*
 * Copyright (c) 2026 Murilo Gonzalez. All Rights Reserved.
 *
 *              ARKLIB INTERNALS MACRO MANIP
 *
 *  Helper macros to concatenation, argument count and macro overloading
 *
 */
#ifndef ARKLIB_INTERNALS_MACRO_MANIP_H_
#define ARKLIB_INTERNALS_MACRO_MANIP_H_

#include "__arklib_pp_arg_foreach_.h"
#include "__arklib_pp_concat_.h"
#include "__arklib_pp_foreach.h"
#include "__arklib_pp_if_else_.h"
#include "__arklib_pp_nargs_.h"
#include "__arklib_pp_stringfy_.h"
#include "__arklib_pp_unique_id_.h"
#include "__arklib_pp_vaarg_n_.h"

#define ark_pp_stringfy(X_) __STRINGFY_ (X_)

#define ark_pp_concat(...) __CONCAT_ (__CONCAT_, ark_pp_nargs (__VA_ARGS__)) (__VA_ARGS__)

#define ark_pp_unwrap(...) __VA_ARGS__

#define ark_pp_nargs(...) __NARGS_ (__VA_ARGS__ __VA_OPT__ (, ) __RSEQ_N_ ())

#define ark_pp_if_else(Cond_, Then_, Else_) ark_pp_concat (__IF_ELSE_, Cond_) (Then_, Else_)

#define ark_pp_foreach(Macro_, ...)                                                                                    \
	ark_pp_concat (__ARKLIB_PP_FOREACH_, ark_pp_nargs (__VA_ARGS__)) (Macro_, __VA_ARGS__)

#define ark_pp_unique_id(Base_) __UNIQUE_ID_ (Base_, __COUNTER__)

#define ark_pp_arg_foreach(Macro_, Arg_, ...)                                                                          \
	ark_pp_concat (__ARKLIB_PP_ARG_FOREACH_, ark_pp_nargs (__VA_ARGS__)) (Macro_, Arg_, __VA_ARGS__)

#endif /* ARKLIB_INTERNALS_MACRO_MANIP_H_ */
