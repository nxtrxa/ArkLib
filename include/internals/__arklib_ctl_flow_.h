#ifndef ARK_INTERNALS_CTL_FLOW_H_
#define ARK_INTERNALS_CTL_FLOW_H_

#import <stddef.h>

#import "__arklib_features_.h"

ARK_BEGIN

/* Module Un-prefixed Aliases */
#ifdef ARK_NAMESPACE
	#define raii			 ark_raii
	#define initializer_list ark_initializer_list
	#define reinterpret_cast ark_reinterpret_cast
	#define get_member		 ark_get_member
	#define move			 ark_move
	#define unused			 ark_unused
#endif

/* Syntactic Shortcuts & Operators */
#define ark_raii(Type_) __attr_cleanup__ (ark_pp_concat (Type_, _cleanup)) Type_

#define ark_initializer_list(...) ark_pp_unwrap ({__VA_ARGS__})

#define ark_reinterpret_cast(Type_, Expr_) ARK_BIT_CAST (Type_, Expr_)

#define $if(Cond_, Then_, Else_) ARK_CHOOSE_EXPR ((Cond_), (Then_), (Else_))

#define $likely(...) if (ARK_LIKELY (__VA_ARGS__))

#define $unlikely(...) if (ARK_UNLIKELY (__VA_ARGS__))

#define $for(I_, N_, ...) for (typeof_unqual (N_) (I_) = 0; (I_) < N_ __VA_ARGS__; ++(I_))

#define $rfor(I_, N_, ...) for (typeof_unqual (N_) (I_) = (N_); (I_) >= (N_)__VA_ARGS__; --(I_))

#define $llforeach(Node_, Head_, Next_Member_) for (auto(Node_) = (Head_); (Node_); (Node_) = (Node_)->Next_Member_)

#define $fn(Ret_Type, Id_) Ret_Type (*Id_)

/* Member Access */
#define ark_get_member(Class_, Obj_Ptr_, Member_Type_, Member_)                                                        \
	(Member_Type_*)((u8*)(Obj_Ptr_) + offsetof (Class_, Member_))

/* Ownership Transfer */
#define ark_move(Dest_, Src_)                                                                                          \
	do {                                                                                                               \
		$static_assert ($typematch ((Dest_), (Src_)), "ARK_MOVE: DEST and SRC types do not match.");                   \
		(Dest_) = (Src_);                                                                                              \
		(Src_)	= (typeof_unqual (Src_)) {0};                                                                          \
	} while (0)

/* Iteration Protocols */
#define ARK_DECL_FROM_MACRO_(Obj_, Begin_, End_) Begin_ (Obj_), End_ (Obj_)
#define ARK_DECL_FROM_MACRO(Obj_, Type_)                                                                               \
	ARK_DECL_FROM_MACRO_ (Obj_, ark_pp_concat (Type_, _begin), ark_pp_concat (Type_, _end))

#define ARK_DECL_IN_MACRO_(Obj_, Begin_, End_) , Begin_ (Obj_), End_ (Obj_)
#define ARK_DECL_IN_MACRO(Obj_, Type_)                                                                                 \
	ARK_DECL_IN_MACRO_ (Obj_, ark_pp_concat (Type_, _begin), ark_pp_concat (Type_, _end))

#define $foreach(...) ark_pp_concat (ark_foreach_, ark_pp_nargs (__VA_ARGS__)) (__VA_ARGS__)

#define ark_foreach_2(Elem_, CArray_)                                                                                  \
	for (size_t ___index__ = 0, ___keep__ = 1 && ARK_IS_C_ARRAY (CArray_);                                             \
		 ___keep__ && ___index__ < ARK_COUNTOF (CArray_);                                                              \
		 ___keep__ = !___keep__, ___index__++)                                                                         \
		for (auto(Elem_) = (CArray_) + ___index__; ___keep__; ___keep__ = !___keep__)

#define ark_foreach_3(Elem_, Begin_, End_) for (auto(Elem_) = (Begin_); (Elem_) != (End_); ++(Elem_))

#define ark_foreach_4(Idx_, Elem_, Begin_, End_)                                                                       \
	for (ptrdiff_t (Idx_) = 0, ___keep__ = 1; ___keep__ && (Idx_) < (ptrdiff_t)(End_ - Begin_);                        \
		 ___keep__ = !___keep__, ++(Idx_))                                                                             \
		for (auto(Elem_) = (Begin_) + (Idx_); ___keep__; ___keep__ = !___keep__)

#define $rforeach(...) ark_pp_concat (ark_foreach_r_, ark_pp_nargs (__VA_ARGS__)) (__VA_ARGS__)

#define ark_foreach_r_2(Elem_, CArray_)                                                                                \
	for (size_t i = ARK_COUNTOF (CArray_) - 1, ___keep__ = 1 && ARK_IS_C_ARRAY (CArray_); ___keep__ && i > 0;          \
		 ___keep__ = !___keep__, --i)                                                                                  \
		for (auto(Elem_) = (CArray_)[i]; ___keep__; ___keep__ = !___keep__)

#define ark_foreach_r_3(Elem_, Begin_, End_) for (auto(Elem_) = (End_) - 1; (Elem_) != (Begin_); --(Elem_))

#define ark_foreach_r_4(Idx_, Elem_, Begin_, End_)                                                                     \
	for (ptrdiff_t (Idx_) = (ptrdiff_t)(End_ - Begin_) - 1, ___keep__ = 1; ___keep__ && (Idx_) > 0;                    \
		 ___keep__ = !___keep__, --(Idx_))                                                                             \
		for (auto(Elem_) = (Begin_) + (Idx_); ___keep__; ___keep__ = !___keep__)

ARK_END

#endif /* ARK_INTERNALS_CTL_FLOW_H_ */
