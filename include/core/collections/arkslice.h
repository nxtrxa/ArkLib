#ifndef ARK_COLLECTIONS_slice_H_
#define ARK_COLLECTIONS_slice_H_

#include "internals/arkdefs.h"

ARK_BEGIN

#ifdef ARK_NAMESPACE
	#define slice		ark_slice
	#define make_slice	ark_make_slice
	#define slice_front ark_slice_front
	#define slice_back	ark_slice_back
	#define slice_begin ark_slice_begin
	#define slice_end	ark_slice_end
	#define slice_at	ark_slice_at
#endif

#define ark_slice(Type_)                                                                                               \
	struct ark_pp_concat (ark_slice_, Type_) {                                                                         \
		const Type_* data;                                                                                             \
		ssz			 size;                                                                                             \
	}

// typedef ark_slice (u0_p) ark_slice;

#define ark_slice_front(Sp_) (Sp_).data[0]
#define ark_slice_back(Sp_) ((Sp_).data[(Sp_).size ? (Sp_).size - 1 : 0]

#define ark_slice_begin(Sp_) (Sp_).data
#define ark_slice_end(Sp_)	 ((Sp_).data + (Sp_).size)

#define $in_slice(Sp_)	 ARK_DECL_IN_MACRO ((Sp_), ark_slice)
#define $from_slice(Sp_) ARK_DECL_IN_MACRO ((Sp_), ark_slice)

#define ark_make_slice(...) ark_pp_concat (ark_make_slice_, ark_pp_nargs (__VA_ARGS__)) (__VA_ARGS__)

#define ark_make_slice_3(Type_, Buffer_, Buffer_Len_)                                                                  \
	({                                                                                                                 \
		(ark_slice (Type_)) {                                                                                          \
			.data = (Buffer_),                                                                                         \
			.size = (Buffer_Len_),                                                                                     \
		};                                                                                                             \
	})

#define ark_make_slice_4(Type_, Buffer_, Buffer_Len_, Lower_Bound_)                                                    \
	({                                                                                                                 \
		auto ___buffer_len__  = (Buffer_Len_);                                                                         \
		auto ___lower_bound__ = (Lower_Bound_);                                                                        \
		(ark_slice (Type_)) {                                                                                          \
			.data = (Buffer_) + ___lower_bound__,                                                                      \
			.size = ___buffer_len__ - ___lower_bound__,                                                                \
		};                                                                                                             \
	})

#define ark_make_slice_5(Type_, Buffer_, Buffer_Len_, Lower_Bound_, Upper_Bound_)                                      \
	({                                                                                                                 \
		ssz ___lower_bound__ = (Lower_Bound_);                                                                         \
		ssz ___upper_bound__ = (Upper_Bound_);                                                                         \
		ssz ___sliced__		 = ___upper_bound__ - ___lower_bound__ + 1;                                                \
		$assert (___sliced__ <= (ssz)(Buffer_Len_) && "ark_make_slice: slice bounds exceeds buffer size");             \
		(ark_slice (Type_)) {                                                                                          \
			.data = (Buffer_) + ___lower_bound__,                                                                      \
			.size = ___sliced__,                                                                                       \
		};                                                                                                             \
	})

#define ark_slice_at(Sl_, Idx_)                                                                                        \
	({                                                                                                                 \
		ssz	 ___idx__	= Idx_;                                                                                        \
		auto ___slice__ = (Sl_);                                                                                       \
		$assert (___idx__ <= ___slice__.size && "ark_slice_at: index out of bounds");                                  \
		___slice__.data[___idx__];                                                                                     \
	})

ARK_END

#endif /* ARK_COLLECTIONS_slice_H_ */
