#ifndef ARKLIB_COLLECTIONS_DARRAY_H_
#define ARKLIB_COLLECTIONS_DARRAY_H_

#import "allocators/arklibc_allocator.h"
#import "core/arkexpect.h"
#import "internals/arkdefs.h"

ARK_BEGIN

#ifdef ARK_NAMESPACE
	#define darray				  ark_darray
	#define darray_t			  ark_darray_t
	#define class_darray		  ark_class_darray
	#define make_darray			  ark_make_darray
	#define destroy_darray		  ark_destroy_darray
	#define darray_elemtypeof	  ark_darray_typeof
	#define darray_elemszof		  ark_darray_elemszof
	#define darray_alignof		  ark_darray_alignof
	#define is_darray_null		  ark_is_darray_null
	#define is_darray_empty		  ark_is_darray_empty
	#define is_darray_initialized ark_is_darray_initialized
	#define darray_data			  ark_darray_data
	#define darray_size			  ark_darray_size
	#define darray_capacity		  ark_darray_capacity
	#define darray_resize		  ark_darray_resize
	#define darray_allocator	  ark_darray_allocator
	#define darray_begin		  ark_darray_begin
	#define darray_end			  ark_darray_end
	#define darray_reserve		  ark_darray_reserve
	#define darray_shrink_to_fit  ark_darray_shrink_to_fit
	#define darray_push_back	  ark_darray_push_back
	#define darray_vpush_back	  ark_darray_vpush_back
	#define darray_push_front	  ark_darray_push_front
	#define darray_vpush_front	  ark_darray_vpush_front
	#define darray_emplace_back	  ark_darray_emplace_back
	#define darray_emplace_front  ark_darray_emplace_front
	#define darray_insert		  ark_darray_insert
	#define darray_pop_back		  ark_darray_pop_back
	#define darray_pop_back_or	  ark_darray_pop_back_or
	#define darray_pop_front	  ark_darray_pop_front
	#define darray_erase		  ark_darray_erase
	#define darray_swap_remove	  ark_darray_swap_remove
	#define darray_ptr			  ark_darray_ptr
	#define darray_get			  ark_darray_get
	#define darray_at			  ark_darray_at
	#define darray_set			  ark_darray_set
	#define darray_sort			  ark_darray_sort
	#define darray_contains		  ark_darray_contains
	#define darray_front		  ark_darray_front
	#define darray_back			  ark_darray_back
#endif

#ifndef ARK_DARRAY_DEFAULT_ALLOCATOR
	#define ARK_DARRAY_DEFAULT_ALLOCATOR ARK_DEFAULT_ALLOCATOR
#endif

#ifndef ARK_G_DARRAY_INIT_CAPACITY
	#define ARK_G_DARRAY_INIT_CAPACITY 16
#endif

/*
 * At the calculation fase this number will be divided by 2.
 */
#ifndef ARK_G_DARRAY_GROWTH_NUM
	#define ARK_G_DARRAY_GROWTH_NUM 4
#endif

enum {
	ARK_DARRAY_INIT_CAPACITY = ARK_G_DARRAY_INIT_CAPACITY,
	ARK_DARRAY_GROWTH_NUM	 = ARK_G_DARRAY_GROWTH_NUM,
};

static constexpr sz ARK_LC_DARRAY_MIN_ALIGNMENT = alignof (u0*) > alignof (sz) ? alignof (u0*) : alignof (sz);

#define ark_darray_alignment(Type_)                                                                                    \
	(alignof (Type_) > ARK_LC_DARRAY_MIN_ALIGNMENT) ? alignof (Type_) : ARK_LC_DARRAY_MIN_ALIGNMENT

#define ark_darray_typename(Type_) struct ark_pp_concat (ark_darray_, Type_)

#define ark_darray_def(Type_)                                                                                          \
	__decl_aligned__ (ark_darray_alignment (Type_)) struct ark_pp_concat (ark_darray_, Type_) {                        \
		sz			  _size;                                                                                           \
		sz			  _capacity;                                                                                       \
		ark_allocator _allocator;                                                                                      \
		Type_		  _data[] __attr_counted_by__ (_size);                                                             \
	}

#define ark_darray(Type_) ark_darray_def (Type_)*

#define ark_class_darray(Except_Handler_, Type_, Da_, ...)                                                             \
	__attr_cleanup__ (ark_darray_cleanup) ark_darray (Type_)                                                           \
		Da_ = ark_catch (ark_make_darray (Type_ __VA_OPT__ (, ) __VA_ARGS__), __except_, Except_Handler_ (__except_))

#define ark_class_darray_unsafe(Type_, Da_, ...)                                                                       \
	__attr_cleanup__ (ark_darray_cleanup) ark_darray (Type_)                                                           \
		Da_ = ark_make_darray (Type_ __VA_OPT__ (, ) __VA_ARGS__).unwrap

/*
 * Introspection
 */
#define ark_darray_hdrsz(Da_)	 sizeof (*(Da_))
#define ark_darray_typeof(Da_)	 typeof ((Da_)->_data[0])
#define ark_darray_elemszof(Da_) sizeof ((Da_)->_data[0])
#define ark_darray_alignof(Da_)	 alignof (*(Da_))
#define ark_darray_meta(Da_)	 ark_darray_hdrsz (Da_), ark_darray_elemszof (Da_), ark_darray_alignof (Da_)

#define ark_is_darray_null(Da_)		   (!(Da_))
#define ark_is_darray_empty(Da_)	   (!(Da_) || (Da_)->_size == 0)
#define ark_is_darray_initialized(Da_) (!ark_is_darray_empty (Da_))

#define ark_darray_data(Da_)	  ((Da_) ? (Da_)->_data : nullptr)
#define ark_darray_size(Da_)	  ((Da_) ? (Da_)->_size : 0)
#define ark_darray_capacity(Da_)  ((Da_) ? (Da_)->_capacity : 0)
#define ark_darray_allocator(Da_) ((Da_) ? (Da_)->_allocator : (ark_allocator) {})

/*
 * Iteratarion
 */
#define ark_darray_begin(Da_) ((Da_) ? (Da_)->_data : nullptr)
#define ark_darray_end(Da_)	  ((Da_) ? (Da_)->_data + (Da_)->_size : nullptr)

#define $in_darray(Da_)	  ARK_DECL_IN_MACRO ((Da_), ark_darray)
#define $from_darray(Da_) ARK_DECL_FROM_MACRO ((Da_), ark_darray)

/*
 * Constructors / Destructors
 */
#define ark_make_darray(...) ark_pp_concat (ark_make_darray_, ark_pp_nargs (__VA_ARGS__)) (__VA_ARGS__)

#define ark_destroy_darray(...) ark_pp_concat (ark_destroy_darray_, ark_pp_nargs (__VA_ARGS__)) (__VA_ARGS__)

#define ark_darray_reserve(Da_, Capacity_)                                                                             \
	__ark_darray_reserve_ (ark_pp_unique_id (__result_), ark_pp_unique_id (__cap_), (Da_), (Capacity_))

#define ark_darray_push_back(Da_, ...)                                                                                 \
	__ark_darray_push_back_ (ark_pp_unique_id (__result_), ark_pp_unique_id (__elem_), (Da_), __VA_ARGS__)

#define ark_darray_vpush_back(Da_, ...)                                                                                \
	__ark_darray_vpush_back_ (                                                                                         \
		ark_pp_unique_id (__result_), ark_pp_unique_id (__elems_), ark_pp_unique_id (__count_), (Da_), __VA_ARGS__     \
	)

#define ark_darray_push_front(Da_, ...)                                                                                \
	__ark_darray_push_front_ (ark_pp_unique_id (__result_), ark_pp_unique_id (__elem_), (Da_), __VA_ARGS__)

#define ark_darray_append(Da_, Src_, Count_)                                                                           \
	__ark_darray_append_ (                                                                                             \
		ark_pp_unique_id (__result_), ark_pp_unique_id (__count_), (Da_), (Src_), (Count_), ark_darray_elemszof (Da_)  \
	)

#define ark_darray_insert(Da_, At_, ...) __ark_darray_insert_ (ark_pp_unique_id (__at_), (Da_), (At_), __VA_ARGS__)

#define ark_darray_pop_back(Da_, ...) __ark_darray_pop_back_ (Da_, ark_pp_nargs (__VA_ARGS__), __VA_ARGS__)

#define ark_darray_pop_front(Da_, ...) __ark_darray_pop_front_ (Da_, ark_pp_nargs (__VA_ARGS__), __VA_ARGS__)

#define ark_darray_remove(...) ark_pp_concat (ark_darray_remove_, ark_pp_nargs (__VA_ARGS__)) (__VA_ARGS__)

/*
 * `ark_make_darray` macros overloads
 */

#define ark_make_darray_1(Type_)                                                                                       \
	ark_make_darray_fn (                                                                                               \
		ARK_DARRAY_DEFAULT_ALLOCATOR,                                                                                  \
		ARK_DARRAY_INIT_CAPACITY,                                                                                      \
		sizeof (ark_darray_typename (Type_)),                                                                          \
		sizeof (Type_),                                                                                                \
		ark_darray_alignment (Type_)                                                                                   \
	)

#define ark_make_darray_2(Type_, Alloc_Or_Cap_)                                                                        \
	ark_make_darray_fn (                                                                                               \
		(Alloc_Or_Cap_), sizeof (ark_darray_typename (Type_)), sizeof (Type_), ark_darray_alignment (Type_)            \
	)

#define ark_make_darray_3(Type_, Allocator_, Capacity_)                                                                \
	ark_make_darray_fn (                                                                                               \
		(Allocator_), (Capacity_), sizeof (ark_darray_typename (Type_)), sizeof (Type_), ark_darray_alignment (Type_)  \
	)

/*
 * `ark_destroy_darray` macros overloads
 */

#define ark_destroy_darray_1(Da_) ark_allocator_dealloc (Da_)

#define ark_destroy_darray_2(Da_, Inner_Type_Destructor_)                                                              \
	do {                                                                                                               \
		$foreach (e $in_darray (Da_)) {                                                                                \
			Inner_Type_Destructor_ (e);                                                                                \
		}                                                                                                              \
		ark_allocator_dealloc (Da_);                                                                                   \
	} while (0)

#define ark_destroy_darray_3(Da_, Inner_Type_Destructor_, ITD_Args_)                                                   \
	do {                                                                                                               \
		$foreach (e $in_darray (Da_)) {                                                                                \
			Inner_Type_Destructor_ (e, ark_pp_unwrap ITD_Args_);                                                       \
		}                                                                                                              \
		ark_allocator_dealloc (Da_);                                                                                   \
	} while (0)

#define __ark_darray_cast_(Da_) ark_reinterpret_cast (ark_darray*, &(Da_))
#define __ark_darray_end_(Da_)	((u8*)(Da_)->_data + ((Da_)->_size * sizeof ((Da_)->_data[0])))

#define __ark_darray_auth_type_(Da_, Type_)                                                                            \
	$static_assert ($typematch ((Da_)->_data[0], Type_) || ($is_real ((Da_)->_data[0]) && $is_real (Type_)))

#define __ark_darray_reserve_(R_, C_, Da_, Capacity_)                                                                  \
	({                                                                                                                 \
		ark_darray_expect_t R_;                                                                                        \
		sz					C_ = Capacity_;                                                                            \
		if (C_ > ark_darray_capacity (Da_)) {                                                                          \
			R_ = ark_darray_reserve_fn (__ark_darray_cast_ (Da_), Capacity_, ark_darray_meta (Da_));                   \
		} else if (C_ < ark_darray_capacity (Da_)) {                                                                   \
			R_ = ark_darray_shrink_to_fit_fn (__ark_darray_cast_ (Da_), ark_darray_meta (Da_));                        \
		}                                                                                                              \
		R_;                                                                                                            \
	})

#define __ark_darray_push_back_(R_, E_, Da_, ...)                                                                      \
	({                                                                                                                 \
		__ark_darray_auth_type_ (Da_, (__VA_ARGS__));                                                                  \
		ark_darray_typeof (Da_) E_ = __VA_ARGS__;                                                                      \
		int R_;                                                                                                        \
		$unlikely (Da_->_size >= Da_->_capacity) {                                                                     \
			R_ = ark_darray_grow_fn (__ark_darray_cast_ (Da_), 1, ARK_DARRAY_GROWTH_NUM, ark_darray_meta (Da_));       \
			if (ark_expect_ok (R_)) Da_->_data[Da_->_size++] = E_;                                                     \
		}                                                                                                              \
		else {                                                                                                         \
			sz	__offset_			 = (Da_->_size) ? sizeof (Da_->_data[0]) : 0;                                      \
			u0* __begin_			 = ((u8*)Da_->_data + __offset_);                                                  \
			R_						 = ark_expected (ARK_DARRAY, __begin_);                                            \
			Da_->_data[Da_->_size++] = ark_reinterpret_cast (ark_darray_typeof (Da_), E_);                             \
		};                                                                                                             \
		R_;                                                                                                            \
	})

#define __ark_darray_vpush_back_(R_, Es_, C_, Da_, ...)                                                                \
	({                                                                                                                 \
		ark_darray_typeof (Da_) Es_[] = {__VA_ARGS__};                                                                 \
		__ark_darray_append_ (R_, C_, Da_, Es_, ARK_COUNTOF (Es_), ark_darray_elemszof (Da_));                         \
	})

#define __ark_darray_append_(R_, C_, Da_, Src_, Count_, Elem_Sz_)                                                      \
	({                                                                                                                 \
		__ark_darray_auth_type_ (Da_, Src_[0]);                                                                        \
		ark_darray_expect_t R_;                                                                                        \
		sz					C_ = Count_;                                                                               \
		$unlikely (C_ > Da_->_capacity - Da_->_size) {                                                                 \
			R_ = ark_darray_grow_fn (__ark_darray_cast_ (Da_), C_, ARK_DARRAY_GROWTH_NUM, ark_darray_meta (Da_));      \
			if (ark_expect_ok (R_)) {                                                                                  \
				ARK_MEMCPY (__ark_darray_end_ (Da_), Src_, C_ * Elem_Sz_);                                             \
				Da_->_size += C_;                                                                                      \
			}                                                                                                          \
		}                                                                                                              \
		else {                                                                                                         \
			sz	__offset_ = (Da_->_size) ? sizeof (Da_->_data[0]) : 0;                                                 \
			u0* __begin_  = ((u8*)Da_->_data + __offset_);                                                             \
			R_			  = ark_expected (ARK_DARRAY, __begin_);                                                       \
			ARK_MEMCPY (__ark_darray_end_ (Da_), Src_, C_ * Elem_Sz_);                                                 \
			Da_->_size += C_;                                                                                          \
		}                                                                                                              \
		R_;                                                                                                            \
	})

#define ARK_EXPECT_CONTEXT ARK_DARRAY

ark_faultdefs (OUT_OF_MEMORY, SIZE_OVERFLOW, OUT_OF_RANGE);

typedef ark_darray_def (u0_p) ark_darray_t;

typedef ark_darray (u0_p) ark_darray;

typedef ark_expect (u0_p) ark_darray_expect_t;

ARKAPI __decl_overloadable__ __decl_nodiscard__ ark_darray_expect_t
ark_make_darray_fn (ark_allocator allocator_, sz capacity_, sz hdr_sz_, sz elem_sz_, sz alignment_) __attr_noexcept__;

ARKAPI __decl_overloadable__ __decl_nodiscard__ ark_darray_expect_t
ark_make_darray_fn (ark_allocator allocator_, sz hdr_sz_, sz elem_sz_, sz alignment_) __attr_noexcept__;

ARKAPI __decl_overloadable__ __decl_nodiscard__ ark_darray_expect_t
ark_make_darray_fn (sz capacity_, sz hdr_sz_, sz elem_sz_, sz alignment_) __attr_noexcept__;

ARKAPI __decl_nodiscard__ ark_fault_t
ark_darray_reserve_fn (ark_darray* darray_, sz capacity_, sz hdr_sz_, sz elem_sz_, sz alignment_) __attr_noexcept__;

ARKAPI __decl_nodiscard__ ark_fault_t
ark_darray_shrink_to_fit_fn (ark_darray* darray_, sz hdr_sz_, sz elem_sz_, sz alignment_) __attr_noexcept__;

ARKAPI __decl_nodiscard__ ark_fault_t ark_darray_grow_fn (
	ark_darray* darray_, sz extra_, f64 grow_factor_num_, sz hdr_sz_, sz elem_sz_, sz alignment_
) __attr_noexcept__;

__decl_static_inline__ u0 ark_darray_cleanup (u0* darray_) {
	ark_darray da = *ark_reinterpret_cast (ark_darray*, darray_);
	ark_allocator_dealloc (da);
}

#undef ARK_EXPECT_CONTEXT

ARK_END

#endif /* ARKLIB_COLLECTIONS_DARRAY_H_ */
