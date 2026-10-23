#ifndef ARKLIB_ARRAY_H_
#define ARKLIB_ARRAY_H_

#include "internals/arkdefs.h"

#ifdef ARK_NAMESPACE
	#define array			 ark_array
	#define array_t			 ark_array_t
	#define class_array		 ark_class_array
	#define make_array		 ark_make_array
	#define array_elemtypeof ark_array_elemtypeof
	#define array_elemszof	 ark_array_elemszof
	#define array_alignof	 ark_array_alignof
	#define array_front		 ark_array_front
	#define array_back		 ark_array_back
	#define array_begin		 ark_array_begin
	#define array_end		 ark_array_end
	#define is_array_empty	 ark_is_array_empty
	#define array_at		 ark_array_at
	#define array_set		 ark_array_set
	#define array_fill		 ark_array_fill
	#define array_swap		 ark_array_swap
	#define array_sort		 ark_array_sort
	#define array_memcpy	 ark_array_memcpy
	#define array_memmove	 ark_array_memmove
#endif

ARK_BEGIN

#define ark_array_typename(Type_, Sz_)                                         \
	struct ark_pp_concat (ark_array_, Type_, _, Sz_)

#define ark_array_def(Type_, Sz_)                                              \
	ark_array_typename (Type_, Sz_) {                                          \
		sz	  _size;                                                           \
		Type_ _data[] __attr_counted_by__ (_size);                             \
	}

#define ark_array(Type_, Sz_) ark_array_def (Type_, Sz_)*

typedef ark_array_def (u0_p, 0) ark_array_t;

typedef ark_array (u0_p, 0) ark_array;

#define ark_class_array(Type_, Sz_, Arr_, ...)                                 \
	ark_array (Type_, Sz_)                                                     \
		Arr_ = ark_make_array (Type_, Sz_ __VA_OPT__ (, ) __VA_ARGS__)

#define ark_array_elemtypeof(Arr_) typeof (*(Arr_)->_data)
#define ark_array_elemszof(Arr_)   sizeof (ark_array_elemtypeof (Arr_))
#define ark_array_alignof(Arr_)	   alignof (ark_array_elemtypeof (Arr_))

#define ark_array_front(Arr_) (Arr_)->_data[0]
#define ark_array_back(Arr_)                                                   \
	(Arr_)->_data[(Arr_)->_size ? (Arr_)->_size - 1 : 0]

#define ark_array_begin(Arr_) (Arr_)->_data
#define ark_array_end(Arr_)	  ((Arr_)->_data + (Arr_)->_size)

#define $in_array(Arr_)	  ARK_DECL_IN_MACRO ((Arr_), ark_array)
#define $from_array(Arr_) ARK_DECL_FROM_MACRO ((Arr_), ark_array)

#define ark_is_array_empty(Arr_) ((Arr_)->_size == 0)

#define ark_make_array(...)                                                    \
	ark_pp_concat (ark_make_array_, ark_pp_nargs (__VA_ARGS__)) (__VA_ARGS__)

#define ark_make_array_2(Type_, Sz_)                                           \
	({                                                                         \
		ark_array (Type_, Sz_) ___arr__                                        \
			= ark_static_alloc (sizeof (ark_array_t) + sizeof (Type_) * Sz_);  \
		*___arr__ = (ark_array_typename (Type_, Sz_)) {                        \
			._size = (Sz_),                                                    \
			._data = {},                                                       \
		};                                                                     \
		___arr__;                                                              \
	})


#define ark_make_array_3(Type_, Sz_, CArray_)                                  \
	({                                                                         \
		$static_assert ($typematch (Type_, typeof (*(CArray_))));              \
		$static_assert ($is_c_array (CArray_));                                \
		auto ___arr__ = ark_make_array_2 (Type_, Sz_);                         \
		ARK_MEMCPY (                                                           \
			___arr__->_data,                                                   \
			CArray_,                                                           \
			(ark_math_min ((Sz_), ARK_COUNTOF (CArray_))) * sizeof (Type_)     \
		);                                                                     \
		___arr__;                                                              \
	})

#define ark_array_at(Arr_, Idx_)                                               \
	({                                                                         \
		sz ___idx__ = (Idx_);                                                  \
		$assert (                                                              \
			___idx__ < (Arr_)->_size                                           \
			&& "ark_array_at(" #Arr_ "): index out of bounds."                 \
		);                                                                     \
		(Arr_)->_data[___idx__];                                               \
	})

#define ark_array_set(Arr_, Idx_, Elem_)                                       \
	do {                                                                       \
		auto ___elem__ = (Elem_);                                              \
		auto ___idx__  = (Idx_);                                               \
		$assert (                                                              \
			ARK_LIKELY (___idx__ < (Arr_)->_size)                              \
			&& "ark_array_set(" #Arr_ "): index out of bounds."                \
		);                                                                     \
		(Arr_)->_data[___idx__] = ___elem__;                                   \
	} while (0)

#define ark_array_fill(...)                                                    \
	ark_pp_concat (ark_array_fill_, ark_pp_nargs (__VA_ARGS__)) (__VA_ARGS__)

#define ark_array_fill_2(Arr_, Val_)                                           \
	do {                                                                       \
		$foreach (e $in_array (Arr_)) {                                        \
			*e = (Val_);                                                       \
		}                                                                      \
	} while (0)

#define ark_array_fill_3(Arr_, From_Val_, To_Val_)                             \
	do {                                                                       \
		$static_assert ($is_integer (*(Arr_)->_data));                         \
		susz ___from_val__ = (From_Val_), ___to_val__ = (To_Val_);             \
		if ((___to_val__ - ___from_val__) >= (susz)(Arr_)->_size)              \
			ARK_PANIC (                                                        \
				"ark_array_fill: offset between values exceeds array _size."   \
			);                                                                 \
		for (susz i = 0, j = ___from_val__;                                    \
			 i < (susz)(Arr_)->_size || j <= ___to_val__;                      \
			 ++i, ++j) {                                                       \
			if (j > ___to_val__) break;                                        \
			(Arr_)->_data[i] = j;                                              \
		}                                                                      \
	} while (0)

#define ark_array_swap(Arr1_, Arr2_)                                           \
	do {                                                                       \
		auto ___tmp__ = (Arr1_);                                               \
		(Arr1_)		  = (Arr2_);                                               \
		(Arr2_)		  = ___tmp__;                                              \
	} while (0)

#define ark_array_sort(Arr_, Compar_)                                          \
	do {                                                                       \
		auto ___arr__ = (Arr_);                                                \
		qsort (                                                                \
			___arr__->_data,                                                   \
			___arr__->_size,                                                   \
			ark_array_elemszof (___arr__),                                     \
			(ark_compar_fn_t)Compar_                                           \
		);                                                                     \
	} while (0)

#define ark_array_memcpy(Arr_, Src_, Len_)                                     \
	do {                                                                       \
		$static_assert (                                                       \
			$typematch (ark_array_elemtypeof (Arr_), typeof (*(Src_)))         \
		);                                                                     \
		sz ___len__ = (Len_);                                                  \
		$assert_internal ((Arr_)->_size <= ___len__);                          \
		ARK_MEMCPY ((Arr_)->_data, (Src_), ___len__);                          \
	} while (0)

#define ark_array_memmove(Arr_, Src_, Len_)                                    \
	do {                                                                       \
		$static_assert (                                                       \
			$typematch (ark_array_elemtypeof (Arr_), typeof (*(Src_)))         \
		);                                                                     \
		sz ___len__ = (Len_);                                                  \
		$assert_internal ((Arr_)->_size <= ___len__);                          \
		ARK_MEMMOVE ((Arr_)->_data, (Src_), ___len__);                         \
	} while (0)


ARK_END

#endif /* ARKLIB_ARRAY_H_ */
