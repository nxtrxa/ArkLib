#ifndef ARKLIB_INTERNALS_VAARGS_N_H_
#define ARKLIB_INTERNALS_VAARGS_N_H_

#define ark_pp_va_arg_1(Arg1_, ...)																			 Arg1_
#define ark_pp_va_arg_2(Arg1_, Arg2_, ...)																	 Arg2_
#define ark_pp_va_arg_3(Arg1_, Arg2_, Arg3_, ...)															 Arg3_
#define ark_pp_va_arg_4(Arg1_, Arg2_, Arg3_, Arg4_, ...)													 Arg4_
#define ark_pp_va_arg_5(Arg1_, Arg2_, Arg3_, Arg4_, Arg5_, ...)												 Arg5_
#define ark_pp_va_arg_6(Arg1_, Arg2_, Arg3_, Arg4_, Arg5_, Arg6_, ...)										 Arg6_
#define ark_pp_va_arg_7(Arg1_, Arg2_, Arg3_, Arg4_, Arg5_, Arg6_, Arg7_, ...)								 Arg7_
#define ark_pp_va_arg_8(Arg1_, Arg2_, Arg3_, Arg4_, Arg5_, Arg6_, Arg7_, Arg8_, ...)						 Arg8_
#define ark_pp_va_arg_9(Arg1_, Arg2_, Arg3_, Arg4_, Arg5_, Arg6_, Arg7_, Arg8_, Arg9_, ...)					 Arg9_
#define ark_pp_va_arg_10(Arg1_, Arg2_, Arg3_, Arg4_, Arg5_, Arg6_, Arg7_, Arg8_, Arg9_, Arg10_, ...)		 Arg10_
#define ark_pp_va_arg_11(Arg1_, Arg2_, Arg3_, Arg4_, Arg5_, Arg6_, Arg7_, Arg8_, Arg9_, Arg10_, Arg11_, ...) Arg11_
#define ark_pp_va_arg_12(Arg1_, Arg2_, Arg3_, Arg4_, Arg5_, Arg6_, Arg7_, Arg8_, Arg9_, Arg10_, Arg11_, Arg12_, ...)   \
	Arg12_
#define ark_pp_va_arg_13(                                                                                              \
	Arg1_, Arg2_, Arg3_, Arg4_, Arg5_, Arg6_, Arg7_, Arg8_, Arg9_, Arg10_, Arg11_, Arg12_, Arg13_, ...                 \
)                                                                                                                      \
	Arg13_
#define ark_pp_va_arg_14(                                                                                              \
	Arg1_, Arg2_, Arg3_, Arg4_, Arg5_, Arg6_, Arg7_, Arg8_, Arg9_, Arg10_, Arg11_, Arg12_, Arg13_, Arg14_, ...         \
)                                                                                                                      \
	Arg14_

#define ark_pp_va_arg_15(                                                                                              \
	Arg1_, Arg2_, Arg3_, Arg4_, Arg5_, Arg6_, Arg7_, Arg8_, Arg9_, Arg10_, Arg11_, Arg12_, Arg13_, Arg14_, Arg15_, ... \
)                                                                                                                      \
	Arg15_

#define ark_pp_va_arg_16(                                                                                              \
	Arg1_,                                                                                                             \
	Arg2_,                                                                                                             \
	Arg3_,                                                                                                             \
	Arg4_,                                                                                                             \
	Arg5_,                                                                                                             \
	Arg6_,                                                                                                             \
	Arg7_,                                                                                                             \
	Arg8_,                                                                                                             \
	Arg9_,                                                                                                             \
	Arg10_,                                                                                                            \
	Arg11_,                                                                                                            \
	Arg12_,                                                                                                            \
	Arg13_,                                                                                                            \
	Arg14_,                                                                                                            \
	Arg15_,                                                                                                            \
	Arg16_,                                                                                                            \
	...                                                                                                                \
)                                                                                                                      \
	Arg16_
#define ark_pp_va_arg_17(                                                                                              \
	Arg1_,                                                                                                             \
	Arg2_,                                                                                                             \
	Arg3_,                                                                                                             \
	Arg4_,                                                                                                             \
	Arg5_,                                                                                                             \
	Arg6_,                                                                                                             \
	Arg7_,                                                                                                             \
	Arg8_,                                                                                                             \
	Arg9_,                                                                                                             \
	Arg10_,                                                                                                            \
	Arg11_,                                                                                                            \
	Arg12_,                                                                                                            \
	Arg13_,                                                                                                            \
	Arg14_,                                                                                                            \
	Arg15_,                                                                                                            \
	Arg16_,                                                                                                            \
	Arg17_,                                                                                                            \
	...                                                                                                                \
)                                                                                                                      \
	Arg17_
#define ark_pp_va_arg_18(                                                                                              \
	Arg1_,                                                                                                             \
	Arg2_,                                                                                                             \
	Arg3_,                                                                                                             \
	Arg4_,                                                                                                             \
	Arg5_,                                                                                                             \
	Arg6_,                                                                                                             \
	Arg7_,                                                                                                             \
	Arg8_,                                                                                                             \
	Arg9_,                                                                                                             \
	Arg10_,                                                                                                            \
	Arg11_,                                                                                                            \
	Arg12_,                                                                                                            \
	Arg13_,                                                                                                            \
	Arg14_,                                                                                                            \
	Arg15_,                                                                                                            \
	Arg16_,                                                                                                            \
	Arg17_,                                                                                                            \
	Arg18_,                                                                                                            \
	...                                                                                                                \
)                                                                                                                      \
	Arg18_
#define ark_pp_va_arg_19(                                                                                              \
	Arg1_,                                                                                                             \
	Arg2_,                                                                                                             \
	Arg3_,                                                                                                             \
	Arg4_,                                                                                                             \
	Arg5_,                                                                                                             \
	Arg6_,                                                                                                             \
	Arg7_,                                                                                                             \
	Arg8_,                                                                                                             \
	Arg9_,                                                                                                             \
	Arg10_,                                                                                                            \
	Arg11_,                                                                                                            \
	Arg12_,                                                                                                            \
	Arg13_,                                                                                                            \
	Arg14_,                                                                                                            \
	Arg15_,                                                                                                            \
	Arg16_,                                                                                                            \
	Arg17_,                                                                                                            \
	Arg18_,                                                                                                            \
	Arg19_,                                                                                                            \
	...                                                                                                                \
)                                                                                                                      \
	Arg19_
#define ark_pp_va_arg_20(                                                                                              \
	Arg1_,                                                                                                             \
	Arg2_,                                                                                                             \
	Arg3_,                                                                                                             \
	Arg4_,                                                                                                             \
	Arg5_,                                                                                                             \
	Arg6_,                                                                                                             \
	Arg7_,                                                                                                             \
	Arg8_,                                                                                                             \
	Arg9_,                                                                                                             \
	Arg10_,                                                                                                            \
	Arg11_,                                                                                                            \
	Arg12_,                                                                                                            \
	Arg13_,                                                                                                            \
	Arg14_,                                                                                                            \
	Arg15_,                                                                                                            \
	Arg16_,                                                                                                            \
	Arg17_,                                                                                                            \
	Arg18_,                                                                                                            \
	Arg19_,                                                                                                            \
	Arg20_,                                                                                                            \
	...                                                                                                                \
)                                                                                                                      \
	Arg20_
#define ark_pp_va_arg_21(                                                                                              \
	Arg1_,                                                                                                             \
	Arg2_,                                                                                                             \
	Arg3_,                                                                                                             \
	Arg4_,                                                                                                             \
	Arg5_,                                                                                                             \
	Arg6_,                                                                                                             \
	Arg7_,                                                                                                             \
	Arg8_,                                                                                                             \
	Arg9_,                                                                                                             \
	Arg10_,                                                                                                            \
	Arg11_,                                                                                                            \
	Arg12_,                                                                                                            \
	Arg13_,                                                                                                            \
	Arg14_,                                                                                                            \
	Arg15_,                                                                                                            \
	Arg16_,                                                                                                            \
	Arg17_,                                                                                                            \
	Arg18_,                                                                                                            \
	Arg19_,                                                                                                            \
	Arg20_,                                                                                                            \
	Arg21_,                                                                                                            \
	...                                                                                                                \
)                                                                                                                      \
	Arg21_
#define ark_pp_va_arg_22(                                                                                              \
	Arg1_,                                                                                                             \
	Arg2_,                                                                                                             \
	Arg3_,                                                                                                             \
	Arg4_,                                                                                                             \
	Arg5_,                                                                                                             \
	Arg6_,                                                                                                             \
	Arg7_,                                                                                                             \
	Arg8_,                                                                                                             \
	Arg9_,                                                                                                             \
	Arg10_,                                                                                                            \
	Arg11_,                                                                                                            \
	Arg12_,                                                                                                            \
	Arg13_,                                                                                                            \
	Arg14_,                                                                                                            \
	Arg15_,                                                                                                            \
	Arg16_,                                                                                                            \
	Arg17_,                                                                                                            \
	Arg18_,                                                                                                            \
	Arg19_,                                                                                                            \
	Arg20_,                                                                                                            \
	Arg21_,                                                                                                            \
	Arg22_,                                                                                                            \
	...                                                                                                                \
)                                                                                                                      \
	Arg22_
#define ark_pp_va_arg_23(                                                                                              \
	Arg1_,                                                                                                             \
	Arg2_,                                                                                                             \
	Arg3_,                                                                                                             \
	Arg4_,                                                                                                             \
	Arg5_,                                                                                                             \
	Arg6_,                                                                                                             \
	Arg7_,                                                                                                             \
	Arg8_,                                                                                                             \
	Arg9_,                                                                                                             \
	Arg10_,                                                                                                            \
	Arg11_,                                                                                                            \
	Arg12_,                                                                                                            \
	Arg13_,                                                                                                            \
	Arg14_,                                                                                                            \
	Arg15_,                                                                                                            \
	Arg16_,                                                                                                            \
	Arg17_,                                                                                                            \
	Arg18_,                                                                                                            \
	Arg19_,                                                                                                            \
	Arg20_,                                                                                                            \
	Arg21_,                                                                                                            \
	Arg22_,                                                                                                            \
	Arg23_,                                                                                                            \
	...                                                                                                                \
)                                                                                                                      \
	Arg23_
#define ark_pp_va_arg_24(                                                                                              \
	Arg1_,                                                                                                             \
	Arg2_,                                                                                                             \
	Arg3_,                                                                                                             \
	Arg4_,                                                                                                             \
	Arg5_,                                                                                                             \
	Arg6_,                                                                                                             \
	Arg7_,                                                                                                             \
	Arg8_,                                                                                                             \
	Arg9_,                                                                                                             \
	Arg10_,                                                                                                            \
	Arg11_,                                                                                                            \
	Arg12_,                                                                                                            \
	Arg13_,                                                                                                            \
	Arg14_,                                                                                                            \
	Arg15_,                                                                                                            \
	Arg16_,                                                                                                            \
	Arg17_,                                                                                                            \
	Arg18_,                                                                                                            \
	Arg19_,                                                                                                            \
	Arg20_,                                                                                                            \
	Arg21_,                                                                                                            \
	Arg22_,                                                                                                            \
	Arg23_,                                                                                                            \
	Arg24_,                                                                                                            \
	...                                                                                                                \
)                                                                                                                      \
	Arg24_
#define ark_pp_va_arg_25(                                                                                              \
	Arg1_,                                                                                                             \
	Arg2_,                                                                                                             \
	Arg3_,                                                                                                             \
	Arg4_,                                                                                                             \
	Arg5_,                                                                                                             \
	Arg6_,                                                                                                             \
	Arg7_,                                                                                                             \
	Arg8_,                                                                                                             \
	Arg9_,                                                                                                             \
	Arg10_,                                                                                                            \
	Arg11_,                                                                                                            \
	Arg12_,                                                                                                            \
	Arg13_,                                                                                                            \
	Arg14_,                                                                                                            \
	Arg15_,                                                                                                            \
	Arg16_,                                                                                                            \
	Arg17_,                                                                                                            \
	Arg18_,                                                                                                            \
	Arg19_,                                                                                                            \
	Arg20_,                                                                                                            \
	Arg21_,                                                                                                            \
	Arg22_,                                                                                                            \
	Arg23_,                                                                                                            \
	Arg24_,                                                                                                            \
	Arg25_,                                                                                                            \
	...                                                                                                                \
)                                                                                                                      \
	Arg25_
#define ark_pp_va_arg_26(                                                                                              \
	Arg1_,                                                                                                             \
	Arg2_,                                                                                                             \
	Arg3_,                                                                                                             \
	Arg4_,                                                                                                             \
	Arg5_,                                                                                                             \
	Arg6_,                                                                                                             \
	Arg7_,                                                                                                             \
	Arg8_,                                                                                                             \
	Arg9_,                                                                                                             \
	Arg10_,                                                                                                            \
	Arg11_,                                                                                                            \
	Arg12_,                                                                                                            \
	Arg13_,                                                                                                            \
	Arg14_,                                                                                                            \
	Arg15_,                                                                                                            \
	Arg16_,                                                                                                            \
	Arg17_,                                                                                                            \
	Arg18_,                                                                                                            \
	Arg19_,                                                                                                            \
	Arg20_,                                                                                                            \
	Arg21_,                                                                                                            \
	Arg22_,                                                                                                            \
	Arg23_,                                                                                                            \
	Arg24_,                                                                                                            \
	Arg25_,                                                                                                            \
	Arg26_,                                                                                                            \
	...                                                                                                                \
)                                                                                                                      \
	Arg26_
#define ark_pp_va_arg_27(                                                                                              \
	Arg1_,                                                                                                             \
	Arg2_,                                                                                                             \
	Arg3_,                                                                                                             \
	Arg4_,                                                                                                             \
	Arg5_,                                                                                                             \
	Arg6_,                                                                                                             \
	Arg7_,                                                                                                             \
	Arg8_,                                                                                                             \
	Arg9_,                                                                                                             \
	Arg10_,                                                                                                            \
	Arg11_,                                                                                                            \
	Arg12_,                                                                                                            \
	Arg13_,                                                                                                            \
	Arg14_,                                                                                                            \
	Arg15_,                                                                                                            \
	Arg16_,                                                                                                            \
	Arg17_,                                                                                                            \
	Arg18_,                                                                                                            \
	Arg19_,                                                                                                            \
	Arg20_,                                                                                                            \
	Arg21_,                                                                                                            \
	Arg22_,                                                                                                            \
	Arg23_,                                                                                                            \
	Arg24_,                                                                                                            \
	Arg25_,                                                                                                            \
	Arg26_,                                                                                                            \
	Arg27_,                                                                                                            \
	...                                                                                                                \
)                                                                                                                      \
	Arg27_
#define ark_pp_va_arg_28(                                                                                              \
	Arg1_,                                                                                                             \
	Arg2_,                                                                                                             \
	Arg3_,                                                                                                             \
	Arg4_,                                                                                                             \
	Arg5_,                                                                                                             \
	Arg6_,                                                                                                             \
	Arg7_,                                                                                                             \
	Arg8_,                                                                                                             \
	Arg9_,                                                                                                             \
	Arg10_,                                                                                                            \
	Arg11_,                                                                                                            \
	Arg12_,                                                                                                            \
	Arg13_,                                                                                                            \
	Arg14_,                                                                                                            \
	Arg15_,                                                                                                            \
	Arg16_,                                                                                                            \
	Arg17_,                                                                                                            \
	Arg18_,                                                                                                            \
	Arg19_,                                                                                                            \
	Arg20_,                                                                                                            \
	Arg21_,                                                                                                            \
	Arg22_,                                                                                                            \
	Arg23_,                                                                                                            \
	Arg24_,                                                                                                            \
	Arg25_,                                                                                                            \
	Arg26_,                                                                                                            \
	Arg27_,                                                                                                            \
	Arg28_,                                                                                                            \
	...                                                                                                                \
)                                                                                                                      \
	Arg28_
#define ark_pp_va_arg_29(                                                                                              \
	Arg1_,                                                                                                             \
	Arg2_,                                                                                                             \
	Arg3_,                                                                                                             \
	Arg4_,                                                                                                             \
	Arg5_,                                                                                                             \
	Arg6_,                                                                                                             \
	Arg7_,                                                                                                             \
	Arg8_,                                                                                                             \
	Arg9_,                                                                                                             \
	Arg10_,                                                                                                            \
	Arg11_,                                                                                                            \
	Arg12_,                                                                                                            \
	Arg13_,                                                                                                            \
	Arg14_,                                                                                                            \
	Arg15_,                                                                                                            \
	Arg16_,                                                                                                            \
	Arg17_,                                                                                                            \
	Arg18_,                                                                                                            \
	Arg19_,                                                                                                            \
	Arg20_,                                                                                                            \
	Arg21_,                                                                                                            \
	Arg22_,                                                                                                            \
	Arg23_,                                                                                                            \
	Arg24_,                                                                                                            \
	Arg25_,                                                                                                            \
	Arg26_,                                                                                                            \
	Arg27_,                                                                                                            \
	Arg28_,                                                                                                            \
	Arg29_,                                                                                                            \
	...                                                                                                                \
)                                                                                                                      \
	Arg29_
#define ark_pp_va_arg_30(                                                                                              \
	Arg1_,                                                                                                             \
	Arg2_,                                                                                                             \
	Arg3_,                                                                                                             \
	Arg4_,                                                                                                             \
	Arg5_,                                                                                                             \
	Arg6_,                                                                                                             \
	Arg7_,                                                                                                             \
	Arg8_,                                                                                                             \
	Arg9_,                                                                                                             \
	Arg10_,                                                                                                            \
	Arg11_,                                                                                                            \
	Arg12_,                                                                                                            \
	Arg13_,                                                                                                            \
	Arg14_,                                                                                                            \
	Arg15_,                                                                                                            \
	Arg16_,                                                                                                            \
	Arg17_,                                                                                                            \
	Arg18_,                                                                                                            \
	Arg19_,                                                                                                            \
	Arg20_,                                                                                                            \
	Arg21_,                                                                                                            \
	Arg22_,                                                                                                            \
	Arg23_,                                                                                                            \
	Arg24_,                                                                                                            \
	Arg25_,                                                                                                            \
	Arg26_,                                                                                                            \
	Arg27_,                                                                                                            \
	Arg28_,                                                                                                            \
	Arg29_,                                                                                                            \
	Arg30_,                                                                                                            \
	...                                                                                                                \
)                                                                                                                      \
	Arg30_

#endif /* ARKLIB_INTERNALS_VAARGS_N_H_ */
