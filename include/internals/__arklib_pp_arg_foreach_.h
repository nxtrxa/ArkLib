#ifndef ARKLIB_INTERNAS_PP_ARG_FOREACH_H_
#define ARKLIB_INTERNAS_PP_ARG_FOREACH_H_

#define __ARKLIB_PP_ARG_FOREACH_1(Macro_, Arg_, ...) Macro_ (Arg_, ark_pp_va_arg_1 (__VA_ARGS__))

#define __ARKLIB_PP_ARG_FOREACH_2(Macro_, Arg_, ...)                                                                   \
	__ARKLIB_PP_ARG_FOREACH_1 (Macro_, Arg_, __VA_ARGS__)                                                              \
	Macro_ (Arg_, ark_pp_va_arg_2 (__VA_ARGS__))

#define __ARKLIB_PP_ARG_FOREACH_3(Macro_, Arg_, ...)                                                                   \
	__ARKLIB_PP_ARG_FOREACH_2 (Macro_, Arg_, __VA_ARGS__)                                                              \
	Macro_ (Arg_, ark_pp_va_arg_3 (__VA_ARGS__))

#define __ARKLIB_PP_ARG_FOREACH_4(Macro_, Arg_, ...)                                                                   \
	__ARKLIB_PP_ARG_FOREACH_3 (Macro_, Arg_, __VA_ARGS__)                                                              \
	Macro_ (Arg_, ark_pp_va_arg_4 (__VA_ARGS__))

#define __ARKLIB_PP_ARG_FOREACH_5(Macro_, Arg_, ...)                                                                   \
	__ARKLIB_PP_ARG_FOREACH_4 (Macro_, Arg_, __VA_ARGS__) Macro_ (Arg_, ark_pp_va_arg_5 (__VA_ARGS__))

#define __ARKLIB_PP_ARG_FOREACH_6(Macro_, Arg_, ...)                                                                   \
	__ARKLIB_PP_ARG_FOREACH_5 (Macro_, Arg_, __VA_ARGS__) Macro_ (Arg_, ark_pp_va_arg_6 (__VA_ARGS__))

#define __ARKLIB_PP_ARG_FOREACH_7(Macro_, Arg_, ...)                                                                   \
	__ARKLIB_PP_ARG_FOREACH_6 (Macro_, Arg_, __VA_ARGS__) Macro_ (Arg_, ark_pp_va_arg_7 (__VA_ARGS__))

#define __ARKLIB_PP_ARG_FOREACH_8(Macro_, Arg_, ...)                                                                   \
	__ARKLIB_PP_ARG_FOREACH_7 (Macro_, Arg_, __VA_ARGS__) Macro_ (Arg_, ark_pp_va_arg_8 (__VA_ARGS__))

#define __ARKLIB_PP_ARG_FOREACH_9(Macro_, Arg_, ...)                                                                   \
	__ARKLIB_PP_ARG_FOREACH_8 (Macro_, Arg_, __VA_ARGS__) Macro_ (Arg_, ark_pp_va_arg_9 (__VA_ARGS__))

#define __ARKLIB_PP_ARG_FOREACH_10(Macro_, Arg_, ...)                                                                  \
	__ARKLIB_PP_ARG_FOREACH_9 (Macro_, Arg_, __VA_ARGS__) Macro_ (Arg_, ark_pp_va_arg_10 (__VA_ARGS__))

#define __ARKLIB_PP_ARG_FOREACH_11(Macro_, Arg_, ...)                                                                  \
	__ARKLIB_PP_ARG_FOREACH_10 (Macro_, Arg_, __VA_ARGS__) Macro_ (Arg_, ark_pp_va_arg_11 (__VA_ARGS__))

#define __ARKLIB_PP_ARG_FOREACH_12(Macro_, Arg_, ...)                                                                  \
	__ARKLIB_PP_ARG_FOREACH_11 (Macro_, Arg_, __VA_ARGS__) Macro_ (Arg_, ark_pp_va_arg_12 (__VA_ARGS__))

#define __ARKLIB_PP_ARG_FOREACH_13(Macro_, Arg_, ...)                                                                  \
	__ARKLIB_PP_ARG_FOREACH_12 (Macro_, Arg_, __VA_ARGS__) Macro_ (Arg_, ark_pp_va_arg_13 (__VA_ARGS__))

#define __ARKLIB_PP_ARG_FOREACH_14(Macro_, Arg_, ...)                                                                  \
	__ARKLIB_PP_ARG_FOREACH_13 (Macro_, Arg_, __VA_ARGS__) Macro_ (Arg_, ark_pp_va_arg_14 (__VA_ARGS__))

#define __ARKLIB_PP_ARG_FOREACH_15(Macro_, Arg_, ...)                                                                  \
	__ARKLIB_PP_ARG_FOREACH_14 (Macro_, Arg_, __VA_ARGS__) Macro_ (Arg_, ark_pp_va_arg_15 (__VA_ARGS__))

#define __ARKLIB_PP_ARG_FOREACH_16(Macro_, Arg_, ...)                                                                  \
	__ARKLIB_PP_ARG_FOREACH_15 (Macro_, Arg_, __VA_ARGS__) Macro_ (Arg_, ark_pp_va_arg_16 (__VA_ARGS__))

#define __ARKLIB_PP_ARG_FOREACH_17(Macro_, Arg_, ...)                                                                  \
	__ARKLIB_PP_ARG_FOREACH_16 (Macro_, Arg_, __VA_ARGS__) Macro_ (Arg_, ark_pp_va_arg_17 (__VA_ARGS__))

#define __ARKLIB_PP_ARG_FOREACH_18(Macro_, Arg_, ...)                                                                  \
	__ARKLIB_PP_ARG_FOREACH_17 (Macro_, Arg_, __VA_ARGS__) Macro_ (Arg_, ark_pp_va_arg_18 (__VA_ARGS__))

#define __ARKLIB_PP_ARG_FOREACH_19(Macro_, Arg_, ...)                                                                  \
	__ARKLIB_PP_ARG_FOREACH_18 (Macro_, Arg_, __VA_ARGS__) Macro_ (Arg_, ark_pp_va_arg_19 (__VA_ARGS__))

#define __ARKLIB_PP_ARG_FOREACH_20(Macro_, Arg_, ...)                                                                  \
	__ARKLIB_PP_ARG_FOREACH_19 (Macro_, Arg_, __VA_ARGS__) Macro_ (Arg_, ark_pp_va_arg_20 (__VA_ARGS__))

#define __ARKLIB_PP_ARG_FOREACH_21(Macro_, Arg_, ...)                                                                  \
	__ARKLIB_PP_ARG_FOREACH_20 (Macro_, Arg_, __VA_ARGS__) Macro_ (Arg_, ark_pp_va_arg_21 (__VA_ARGS__))

#define __ARKLIB_PP_ARG_FOREACH_22(Macro_, Arg_, ...)                                                                  \
	__ARKLIB_PP_ARG_FOREACH_21 (Macro_, Arg_, __VA_ARGS__) Macro_ (Arg_, ark_pp_va_arg_22 (__VA_ARGS__))

#define __ARKLIB_PP_ARG_FOREACH_23(Macro_, Arg_, ...)                                                                  \
	__ARKLIB_PP_ARG_FOREACH_22 (Macro_, Arg_, __VA_ARGS__) Macro_ (Arg_, ark_pp_va_arg_23 (__VA_ARGS__))

#define __ARKLIB_PP_ARG_FOREACH_24(Macro_, Arg_, ...)                                                                  \
	__ARKLIB_PP_ARG_FOREACH_23 (Macro_, Arg_, __VA_ARGS__) Macro_ (Arg_, ark_pp_va_arg_24 (__VA_ARGS__))

#define __ARKLIB_PP_ARG_FOREACH_25(Macro_, Arg_, ...)                                                                  \
	__ARKLIB_PP_ARG_FOREACH_24 (Macro_, Arg_, __VA_ARGS__) Macro_ (Arg_, ark_pp_va_arg_25 (__VA_ARGS__))

#define __ARKLIB_PP_ARG_FOREACH_26(Macro_, Arg_, ...)                                                                  \
	__ARKLIB_PP_ARG_FOREACH_25 (Macro_, Arg_, __VA_ARGS__) Macro_ (Arg_, ark_pp_va_arg_26 (__VA_ARGS__))

#define __ARKLIB_PP_ARG_FOREACH_27(Macro_, Arg_, ...)                                                                  \
	__ARKLIB_PP_ARG_FOREACH_26 (Macro_, Arg_, __VA_ARGS__) Macro_ (Arg_, ark_pp_va_arg_27 (__VA_ARGS__))

#define __ARKLIB_PP_ARG_FOREACH_28(Macro_, Arg_, ...)                                                                  \
	__ARKLIB_PP_ARG_FOREACH_27 (Macro_, Arg_, __VA_ARGS__) Macro_ (Arg_, ark_pp_va_arg_28 (__VA_ARGS__))

#define __ARKLIB_PP_ARG_FOREACH_29(Macro_, Arg_, ...)                                                                  \
	__ARKLIB_PP_ARG_FOREACH_28 (Macro_, Arg_, __VA_ARGS__) Macro_ (Arg_, ark_pp_va_arg_29 (__VA_ARGS__))

#define __ARKLIB_PP_ARG_FOREACH_30(Macro_, Arg_, ...)                                                                  \
	__ARKLIB_PP_ARG_FOREACH_29 (Macro_, Arg_, __VA_ARGS__) Macro_ (Arg_, ark_pp_va_arg_30 (__VA_ARGS__))

#define __ARKLIB_PP_ARG_FOREACH_31(Macro_, Arg_, ...)                                                                  \
	__ARKLIB_PP_ARG_FOREACH_30 (Macro_, Arg_, __VA_ARGS__) Macro_ (Arg_, ark_pp_va_arg_31 (__VA_ARGS__))

#define __ARKLIB_PP_ARG_FOREACH_32(Macro_, Arg_, ...)                                                                  \
	__ARKLIB_PP_ARG_FOREACH_31 (Macro_, Arg_, __VA_ARGS__) Macro_ (Arg_, ark_pp_va_arg_32 (__VA_ARGS__))

#define __ARKLIB_PP_ARG_FOREACH_33(Macro_, Arg_, ...)                                                                  \
	__ARKLIB_PP_ARG_FOREACH_32 (Macro_, Arg_, __VA_ARGS__) Macro_ (Arg_, ark_pp_va_arg_33 (__VA_ARGS__))

#define __ARKLIB_PP_ARG_FOREACH_34(Macro_, Arg_, ...)                                                                  \
	__ARKLIB_PP_ARG_FOREACH_33 (Macro_, Arg_, __VA_ARGS__) Macro_ (Arg_, ark_pp_va_arg_34 (__VA_ARGS__))

#define __ARKLIB_PP_ARG_FOREACH_35(Macro_, Arg_, ...)                                                                  \
	__ARKLIB_PP_ARG_FOREACH_34 (Macro_, Arg_, __VA_ARGS__) Macro_ (Arg_, ark_pp_va_arg_35 (__VA_ARGS__))

#define __ARKLIB_PP_ARG_FOREACH_36(Macro_, Arg_, ...)                                                                  \
	__ARKLIB_PP_ARG_FOREACH_35 (Macro_, Arg_, __VA_ARGS__) Macro_ (Arg_, ark_pp_va_arg_36 (__VA_ARGS__))

#define __ARKLIB_PP_ARG_FOREACH_37(Macro_, Arg_, ...)                                                                  \
	__ARKLIB_PP_ARG_FOREACH_36 (Macro_, Arg_, __VA_ARGS__) Macro_ (Arg_, ark_pp_va_arg_37 (__VA_ARGS__))

#define __ARKLIB_PP_ARG_FOREACH_38(Macro_, Arg_, ...)                                                                  \
	__ARKLIB_PP_ARG_FOREACH_37 (Macro_, Arg_, __VA_ARGS__) Macro_ (Arg_, ark_pp_va_arg_38 (__VA_ARGS__))

#define __ARKLIB_PP_ARG_FOREACH_39(Macro_, Arg_, ...)                                                                  \
	__ARKLIB_PP_ARG_FOREACH_38 (Macro_, Arg_, __VA_ARGS__) Macro_ (Arg_, ark_pp_va_arg_39 (__VA_ARGS__))

#define __ARKLIB_PP_ARG_FOREACH_40(Macro_, Arg_, ...)                                                                  \
	__ARKLIB_PP_ARG_FOREACH_39 (Macro_, Arg_, __VA_ARGS__) Macro_ (Arg_, ark_pp_va_arg_40 (__VA_ARGS__))

#define __ARKLIB_PP_ARG_FOREACH_41(Macro_, Arg_, ...)                                                                  \
	__ARKLIB_PP_ARG_FOREACH_40 (Macro_, Arg_, __VA_ARGS__) Macro_ (Arg_, ark_pp_va_arg_41 (__VA_ARGS__))

#define __ARKLIB_PP_ARG_FOREACH_42(Macro_, Arg_, ...)                                                                  \
	__ARKLIB_PP_ARG_FOREACH_41 (Macro_, Arg_, __VA_ARGS__) Macro_ (Arg_, ark_pp_va_arg_42 (__VA_ARGS__))

#define __ARKLIB_PP_ARG_FOREACH_43(Macro_, Arg_, ...)                                                                  \
	__ARKLIB_PP_ARG_FOREACH_42 (Macro_, Arg_, __VA_ARGS__) Macro_ (Arg_, ark_pp_va_arg_43 (__VA_ARGS__))

#define __ARKLIB_PP_ARG_FOREACH_44(Macro_, Arg_, ...)                                                                  \
	__ARKLIB_PP_ARG_FOREACH_43 (Macro_, Arg_, __VA_ARGS__) Macro_ (Arg_, ark_pp_va_arg_44 (__VA_ARGS__))

#define __ARKLIB_PP_ARG_FOREACH_45(Macro_, Arg_, ...)                                                                  \
	__ARKLIB_PP_ARG_FOREACH_44 (Macro_, Arg_, __VA_ARGS__) Macro_ (Arg_, ark_pp_va_arg_45 (__VA_ARGS__))

#define __ARKLIB_PP_ARG_FOREACH_46(Macro_, Arg_, ...)                                                                  \
	__ARKLIB_PP_ARG_FOREACH_45 (Macro_, Arg_, __VA_ARGS__) Macro_ (Arg_, ark_pp_va_arg_46 (__VA_ARGS__))

#define __ARKLIB_PP_ARG_FOREACH_47(Macro_, Arg_, ...)                                                                  \
	__ARKLIB_PP_ARG_FOREACH_46 (Macro_, Arg_, __VA_ARGS__) Macro_ (Arg_, ark_pp_va_arg_47 (__VA_ARGS__))

#define __ARKLIB_PP_ARG_FOREACH_48(Macro_, Arg_, ...)                                                                  \
	__ARKLIB_PP_ARG_FOREACH_47 (Macro_, Arg_, __VA_ARGS__) Macro_ (Arg_, ark_pp_va_arg_48 (__VA_ARGS__))

#define __ARKLIB_PP_ARG_FOREACH_49(Macro_, Arg_, ...)                                                                  \
	__ARKLIB_PP_ARG_FOREACH_48 (Macro_, Arg_, __VA_ARGS__) Macro_ (Arg_, ark_pp_va_arg_49 (__VA_ARGS__))

#define __ARKLIB_PP_ARG_FOREACH_50(Macro_, Arg_, ...)                                                                  \
	__ARKLIB_PP_ARG_FOREACH_49 (Macro_, Arg_, __VA_ARGS__) Macro_ (Arg_, ark_pp_va_arg_50 (__VA_ARGS__))

#define __ARKLIB_PP_ARG_FOREACH_51(Macro_, Arg_, ...)                                                                  \
	__ARKLIB_PP_ARG_FOREACH_50 (Macro_, Arg_, __VA_ARGS__) Macro_ (Arg_, ark_pp_va_arg_51 (__VA_ARGS__))

#define __ARKLIB_PP_ARG_FOREACH_52(Macro_, Arg_, ...)                                                                  \
	__ARKLIB_PP_ARG_FOREACH_51 (Macro_, Arg_, __VA_ARGS__) Macro_ (Arg_, ark_pp_va_arg_52 (__VA_ARGS__))

#define __ARKLIB_PP_ARG_FOREACH_53(Macro_, Arg_, ...)                                                                  \
	__ARKLIB_PP_ARG_FOREACH_52 (Macro_, Arg_, __VA_ARGS__) Macro_ (Arg_, ark_pp_va_arg_53 (__VA_ARGS__))

#define __ARKLIB_PP_ARG_FOREACH_54(Macro_, Arg_, ...)                                                                  \
	__ARKLIB_PP_ARG_FOREACH_53 (Macro_, Arg_, __VA_ARGS__) Macro_ (Arg_, ark_pp_va_arg_54 (__VA_ARGS__))

#define __ARKLIB_PP_ARG_FOREACH_55(Macro_, Arg_, ...)                                                                  \
	__ARKLIB_PP_ARG_FOREACH_54 (Macro_, Arg_, __VA_ARGS__) Macro_ (Arg_, ark_pp_va_arg_55 (__VA_ARGS__))

#define __ARKLIB_PP_ARG_FOREACH_56(Macro_, Arg_, ...)                                                                  \
	__ARKLIB_PP_ARG_FOREACH_55 (Macro_, Arg_, __VA_ARGS__) Macro_ (Arg_, ark_pp_va_arg_56 (__VA_ARGS__))

#define __ARKLIB_PP_ARG_FOREACH_57(Macro_, Arg_, ...)                                                                  \
	__ARKLIB_PP_ARG_FOREACH_56 (Macro_, Arg_, __VA_ARGS__) Macro_ (Arg_, ark_pp_va_arg_57 (__VA_ARGS__))

#define __ARKLIB_PP_ARG_FOREACH_58(Macro_, Arg_, ...)                                                                  \
	__ARKLIB_PP_ARG_FOREACH_57 (Macro_, Arg_, __VA_ARGS__) Macro_ (Arg_, ark_pp_va_arg_58 (__VA_ARGS__))

#define __ARKLIB_PP_ARG_FOREACH_59(Macro_, Arg_, ...)                                                                  \
	__ARKLIB_PP_ARG_FOREACH_58 (Macro_, Arg_, __VA_ARGS__) Macro_ (Arg_, ark_pp_va_arg_59 (__VA_ARGS__))

#define __ARKLIB_PP_ARG_FOREACH_60(Macro_, Arg_, ...)                                                                  \
	__ARKLIB_PP_ARG_FOREACH_59 (Macro_, Arg_, __VA_ARGS__) Macro_ (Arg_, ark_pp_va_arg_60 (__VA_ARGS__))

#define __ARKLIB_PP_ARG_FOREACH_61(Macro_, Arg_, ...)                                                                  \
	__ARKLIB_PP_ARG_FOREACH_60 (Macro_, Arg_, __VA_ARGS__) Macro_ (Arg_, ark_pp_va_arg_61 (__VA_ARGS__))

#define __ARKLIB_PP_ARG_FOREACH_62(Macro_, Arg_, ...)                                                                  \
	__ARKLIB_PP_ARG_FOREACH_61 (Macro_, Arg_, __VA_ARGS__) Macro_ (Arg_, ark_pp_va_arg_62 (__VA_ARGS__))

#define __ARKLIB_PP_ARG_FOREACH_63(Macro_, Arg_, ...)                                                                  \
	__ARKLIB_PP_ARG_FOREACH_62 (Macro_, Arg_, __VA_ARGS__) Macro_ (Arg_, ark_pp_va_arg_63 (__VA_ARGS__))

#define __ARKLIB_PP_ARG_FOREACH_64(Macro_, Arg_, ...)                                                                  \
	__ARG_FOREACH_63 (Macro_, Arg_, __VA_ARGS__) Macro_ (Arg_, ark_pp_va_arg_64 (__VA_ARGS__))

#endif /* ARKLIB_INTERNAS_PP_ARG_FOREACH_H_ */
