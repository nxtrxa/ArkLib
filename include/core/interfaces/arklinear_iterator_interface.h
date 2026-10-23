/*
 * @Notrya Annes - june 27 2026 -  00:23 BRT
 *
 *  Linear iterator interface that cycles throuthout an underlying buffer
 *
 *  To adapt it to a custom datatype:
 *
 *  datatype must have an ARK_ITERATOR member.
 *  you must define both
 *  ARK_OBJECT_DATA_FIELD (iterable contiguous buffer field e.g. T* or a C raw
 *  array) and ARK_OBJECT_SIZE_FIELD (field that holds the buffer size) macros.
 *  Once both defined, call ARK_LINEAR_ITERATOR_INTERFACE(<datatype>, <return
 *  type>). And then undef both.
 *
 *  <return type> is the type that the iterator's functions will return. Must be
 *  compatible with the underlying buffer type
 *
 *  Dont forget that the ARK_ITERATOR members are functions poiters (except by
 *  STATE), therefore you need to assign each field to it correspondent
 *  function.
 *
 * Note:
 *
 *  ARK_LINEAR_ITERATOR_INTERFACE expects a default TARGET_TYPE_.
 *  If the TARGET_TYPE_ have a VLA (variadic array size) it must be the last
 *  field in the struct declaration. Otherwise it may cause UB due to the
 *  difference of field offsets
 */
#ifndef ARK_LINEAR_ITERATOR_H_
#define ARK_LINEAR_ITERATOR_H_

#include "interfaces/iterator.h"
#include "internals/argcount.h"

#define ARK_LINEAR_ITERATOR_ATTRIBUTES __decl_static_inline__ __attr_nodiscard__ __attr_const__

#define ARK_LIT_TARGET_PTR(Target_Type_) Target_Type_* __restrict__ obj

#define ARK_LINEAR_ITERATOR_INTERFACE(Target_Type_, Return_Type_)                                                      \
	ARK_LINEAR_ITERATOR_METHODS (Target_Type_, Return_Type_)

#define ARK_LINEAR_ITERATOR_METHODS(Target_Type_, Return_Type_)                                                        \
	ARK_LINEAR_ITERATOR_INIT_DEF (                                                                                     \
		Target_Type_, __decl_static_inline__, void, (ARK_LIT_TARGET_PTR (Target_Type_)), init)                         \
	ARK_LINEAR_ITERATOR_BEGIN_DEF (                                                                                    \
		Target_Type_, ARK_LINEAR_ITERATOR_ATTRIBUTES, Return_Type_, (ARK_LIT_TARGET_PTR (Target_Type_)), begin)        \
	ARK_LINEAR_ITERATOR_END_DEF (                                                                                      \
		Target_Type_, ARK_LINEAR_ITERATOR_ATTRIBUTES, Return_Type_, (ARK_LIT_TARGET_PTR (Target_Type_)), end)          \
	ARK_LINEAR_ITERATOR_NEXT_DEF (                                                                                     \
		Target_Type_, ARK_LINEAR_ITERATOR_ATTRIBUTES, Return_Type_, (ARK_LIT_TARGET_PTR (Target_Type_)), next)         \
	ARK_LINEAR_ITERATOR_PREV_DEF (                                                                                     \
		Target_Type_, ARK_LINEAR_ITERATOR_ATTRIBUTES, Return_Type_, (ARK_LIT_TARGET_PTR (Target_Type_)), prev)         \
	ARK_LINEAR_ITERATOR_PEEK_DEF (                                                                                     \
		Target_Type_, ARK_LINEAR_ITERATOR_ATTRIBUTES, Return_Type_, (ARK_LIT_TARGET_PTR (Target_Type_)), peek)         \
	ARK_LINEAR_ITERATOR_REWIND_DEF (                                                                                   \
		Target_Type_, __decl_static_inline__, void, (ARK_LIT_TARGET_PTR (Target_Type_)), rewind)                       \
	ARK_LINEAR_ITERATOR_TRAVERSE_DEF (                                                                                 \
		Target_Type_, __decl_static_inline__, void, (ARK_LIT_TARGET_PTR (Target_Type_)), traverse)

#define ___ARK__LINEAR_ITER_FUNC_DEF__(Target_Type_, Attributes_, Return_Type_, Params_, Func_)                        \
	Attributes_ Return_Type_ ARK_CONCAT_3 (Target_Type_, _iter_, Func_) (ARK_UNWRAP Params_)

#define ARK_LINEAR_ITERATOR_INIT_DEF(Target_Type_, Attributes_, Return_Type_, Params_, Func_)                          \
	___ARK__LINEAR_ITER_FUNC_DEF__ (Target_Type_, Attributes_, Return_Type_, Params_, Func_) {                         \
		ark_iter_current (obj) = ark_iter_begin (obj);                                                                 \
		ark_iter_offset (obj)  = 0;                                                                                    \
	}

#define ARK_LINEAR_ITERATOR_BEGIN_DEF(Target_Type_, Attributes_, Return_Type_, Params_, Func_)                         \
	___ARK__LINEAR_ITER_FUNC_DEF__ (Target_Type_, Attributes_, Return_Type_, Params_, Func_) {                         \
		return obj->ARK_OBJECT_DATA_FIELD;                                                                             \
	}

#define ARK_LINEAR_ITERATOR_END_DEF(Target_Type_, Attributes_, Return_Type_, Params_, Func_)                           \
	___ARK__LINEAR_ITER_FUNC_DEF__ (Target_Type_, Attributes_, Return_Type_, Params_, Func_) {                         \
		return (Return_Type_)((u8*)obj->ARK_OBJECT_DATA_FIELD + (obj->ARK_OBJECT_SIZE_FIELD * ark_iter_elemsz (obj))); \
	}

#define ARK_LINEAR_ITERATOR_NEXT_DEF(Target_Type_, Attributes_, Return_Type_, Params_, Func_)                          \
	___ARK__LINEAR_ITER_FUNC_DEF__ (Target_Type_, Attributes_, Return_Type_, Params_, Func_) {                         \
		if (!ark_iter_current (obj) || ark_iter_offset (obj) >= (ptrdiff_t)obj->ARK_OBJECT_SIZE_FIELD) {               \
			ark_iter_rewind (obj);                                                                                     \
		}                                                                                                              \
		ark_iter_current (obj) += ark_iter_elemsz (obj);                                                               \
		ark_iter_offset (obj)	= ((ark_iter_current (obj) - ark_iter_begin (obj)) / ark_iter_elemsz (obj));           \
		return ark_iter_current (obj);                                                                                 \
	}

#define ARK_LINEAR_ITERATOR_PREV_DEF(Target_Type_, Attributes_, Return_Type_, Params_, Func_)                          \
	___ARK__LINEAR_ITER_FUNC_DEF__ (Target_Type_, Attributes_, Return_Type_, Params_, Func_) {                         \
		if (!ark_iter_current (obj) || ark_iter_current (obj) == ark_iter_start (obj)) {                               \
			ark_iter_traverse (obj);                                                                                   \
		}                                                                                                              \
		ark_iter_current (obj) -= ark_iter_elemsz (obj);                                                               \
		ark_iter_offset (obj)	= ((ark_iter_current (obj) - ark_iter_begin (obj)) / ark_iter_elemsz (obj));           \
		return ark_iter_current (obj);                                                                                 \
	}

#define ARK_LINEAR_ITERATOR_PEEK_DEF(Target_Type_, Attributes_, Return_Type_, Params_, Func_)                          \
	___ARK__LINEAR_ITER_FUNC_DEF__ (Target_Type_, Attributes_, Return_Type_, Params_, Func_) {                         \
		if (!ark_iter_current (obj)) ark_iter_rewind (obj);                                                            \
		return ark_iter_current (obj);                                                                                 \
	}

#define ARK_LINEAR_ITERATOR_REWIND_DEF(Target_Type_, Attributes_, Return_Type_, Params_, Func_)                        \
	___ARK__LINEAR_ITER_FUNC_DEF__ (Target_Type_, Attributes_, Return_Type_, Params_, Func_) {                         \
		ark_iter_current (obj) = ark_iter_begin (obj) - ark_iter_elemsz (obj);                                         \
		ark_iter_offset (obj)  = -1;                                                                                   \
	}

#define ARK_LINEAR_ITERATOR_TRAVERSE_DEF(Target_Type_, Attributes_, Return_Type_, Params_, Func_)                      \
	___ARK__LINEAR_ITER_FUNC_DEF__ (Target_Type_, Attributes_, Return_Type_, Params_, Func_) {                         \
		ark_iter_current (obj) = ark_iter_end (obj);                                                                   \
		ark_iter_offset (obj)  = obj->ARK_OBJECT_SIZE_FIELD;                                                           \
	}

#endif /* ARK_LINEAR_ITERATOR_H_ */
