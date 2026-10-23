#ifndef ARK_INTERNAS_PP_FOREACH_H_
#define ARK_INTERNAS_PP_FOREACH_H_

#define __ARKLIB_PP_FOREACH_1(Macro_, ...) Macro_ (ark_pp_va_arg_1 (__VA_ARGS__))

#define __ARKLIB_PP_FOREACH_2(Macro_, ...)                                                                             \
	__ARKLIB_PP_FOREACH_1 (Macro_, __VA_ARGS__) Macro_ (ark_pp_va_arg_2 (__VA_ARGS__))

#define __ARKLIB_PP_FOREACH_3(Macro_, ...)                                                                             \
	__ARKLIB_PP_FOREACH_2 (Macro_, __VA_ARGS__) Macro_ (ark_pp_va_arg_3 (__VA_ARGS__))

#define __ARKLIB_PP_FOREACH_4(Macro_, ...)                                                                             \
	__ARKLIB_PP_FOREACH_3 (Macro_, __VA_ARGS__) Macro_ (ark_pp_va_arg_4 (__VA_ARGS__))

#define __ARKLIB_PP_FOREACH_5(Macro_, ...)                                                                             \
	__ARKLIB_PP_FOREACH_4 (Macro_, __VA_ARGS__) Macro_ (ark_pp_va_arg_5 (__VA_ARGS__))

#define __ARKLIB_PP_FOREACH_6(Macro_, ...)                                                                             \
	__ARKLIB_PP_FOREACH_5 (Macro_, __VA_ARGS__) Macro_ (ark_pp_va_arg_6 (__VA_ARGS__))

#define __ARKLIB_PP_FOREACH_7(Macro_, ...)                                                                             \
	__ARKLIB_PP_FOREACH_6 (Macro_, __VA_ARGS__) Macro_ (ark_pp_va_arg_7 (__VA_ARGS__))

#define __ARKLIB_PP_FOREACH_8(Macro_, ...)                                                                             \
	__ARKLIB_PP_FOREACH_7 (Macro_, __VA_ARGS__) Macro_ (ark_pp_va_arg_8 (__VA_ARGS__))

#define __ARKLIB_PP_FOREACH_9(Macro_, ...)                                                                             \
	__ARKLIB_PP_FOREACH_8 (Macro_, __VA_ARGS__) Macro_ (ark_pp_va_arg_9 (__VA_ARGS__))

#define __ARKLIB_PP_FOREACH_10(Macro_, ...)                                                                            \
	__ARKLIB_PP_FOREACH_9 (Macro_, __VA_ARGS__) Macro_ (ark_pp_va_arg_10 (__VA_ARGS__))

#define __ARKLIB_PP_FOREACH_11(Macro_, ...)                                                                            \
	__ARKLIB_PP_FOREACH_10 (Macro_, __VA_ARGS__) Macro_ (ark_pp_va_arg_11 (__VA_ARGS__))

#define __ARKLIB_PP_FOREACH_12(Macro_, ...)                                                                            \
	__ARKLIB_PP_FOREACH_11 (Macro_, __VA_ARGS__) Macro_ (ark_pp_va_arg_12 (__VA_ARGS__))

#define __ARKLIB_PP_FOREACH_13(Macro_, ...)                                                                            \
	__ARKLIB_PP_FOREACH_12 (Macro_, __VA_ARGS__) Macro_ (ark_pp_va_arg_13 (__VA_ARGS__))

#define __ARKLIB_PP_FOREACH_14(Macro_, ...)                                                                            \
	__ARKLIB_PP_FOREACH_13 (Macro_, __VA_ARGS__) Macro_ (ark_pp_va_arg_14 (__VA_ARGS__))

#define __ARKLIB_PP_FOREACH_15(Macro_, ...)                                                                            \
	__ARKLIB_PP_FOREACH_14 (Macro_, __VA_ARGS__) Macro_ (ark_pp_va_arg_15 (__VA_ARGS__))

#define __ARKLIB_PP_FOREACH_16(Macro_, ...)                                                                            \
	__ARKLIB_PP_FOREACH_15 (Macro_, __VA_ARGS__) Macro_ (ark_pp_va_arg_16 (__VA_ARGS__))

#define __ARKLIB_PP_FOREACH_17(Macro_, ...)                                                                            \
	__ARKLIB_PP_FOREACH_16 (Macro_, __VA_ARGS__) Macro_ (ark_pp_va_arg_17 (__VA_ARGS__))

#define __ARKLIB_PP_FOREACH_18(Macro_, ...)                                                                            \
	__ARKLIB_PP_FOREACH_17 (Macro_, __VA_ARGS__) Macro_ (ark_pp_va_arg_18 (__VA_ARGS__))

#define __ARKLIB_PP_FOREACH_19(Macro_, ...)                                                                            \
	__ARKLIB_PP_FOREACH_18 (Macro_, __VA_ARGS__) Macro_ (ark_pp_va_arg_19 (__VA_ARGS__))

#define __ARKLIB_PP_FOREACH_20(Macro_, ...)                                                                            \
	__ARKLIB_PP_FOREACH_19 (Macro_, __VA_ARGS__) Macro_ (ark_pp_va_arg_20 (__VA_ARGS__))

#define __ARKLIB_PP_FOREACH_21(Macro_, ...)                                                                            \
	__ARKLIB_PP_FOREACH_20 (Macro_, __VA_ARGS__) Macro_ (ark_pp_va_arg_21 (__VA_ARGS__))

#define __ARKLIB_PP_FOREACH_22(Macro_, ...)                                                                            \
	__ARKLIB_PP_FOREACH_21 (Macro_, __VA_ARGS__) Macro_ (ark_pp_va_arg_22 (__VA_ARGS__))

#define __ARKLIB_PP_FOREACH_23(Macro_, ...)                                                                            \
	__ARKLIB_PP_FOREACH_22 (Macro_, __VA_ARGS__) Macro_ (ark_pp_va_arg_23 (__VA_ARGS__))

#define __ARKLIB_PP_FOREACH_24(Macro_, ...)                                                                            \
	__ARKLIB_PP_FOREACH_23 (Macro_, __VA_ARGS__) Macro_ (ark_pp_va_arg_24 (__VA_ARGS__))

#define __ARKLIB_PP_FOREACH_25(Macro_, ...)                                                                            \
	__ARKLIB_PP_FOREACH_24 (Macro_, __VA_ARGS__) Macro_ (ark_pp_va_arg_25 (__VA_ARGS__))

#define __ARKLIB_PP_FOREACH_26(Macro_, ...)                                                                            \
	__ARKLIB_PP_FOREACH_25 (Macro_, __VA_ARGS__) Macro_ (ark_pp_va_arg_26 (__VA_ARGS__))

#define __ARKLIB_PP_FOREACH_27(Macro_, ...)                                                                            \
	__ARKLIB_PP_FOREACH_26 (Macro_, __VA_ARGS__) Macro_ (ark_pp_va_arg_27 (__VA_ARGS__))

#define __ARKLIB_PP_FOREACH_28(Macro_, ...)                                                                            \
	__ARKLIB_PP_FOREACH_27 (Macro_, __VA_ARGS__) Macro_ (ark_pp_va_arg_28 (__VA_ARGS__))

#define __ARKLIB_PP_FOREACH_29(Macro_, ...)                                                                            \
	__ARKLIB_PP_FOREACH_28 (Macro_, __VA_ARGS__) Macro_ (ark_pp_va_arg_29 (__VA_ARGS__))

#define __ARKLIB_PP_FOREACH_30(Macro_, ...)                                                                            \
	__ARKLIB_PP_FOREACH_29 (Macro_, __VA_ARGS__) Macro_ (ark_pp_va_arg_30 (__VA_ARGS__))

#endif /* ARK_INTERNAS_PP_FOREACH_H_ */
