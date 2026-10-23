/*-----------------------------------------------------------------------
 *  @Copyright (c) 2026 Murilo Gonzalez. All Rights Reserved.
 *
 *                          ARKLIB ALLOCATOR INTERFACE
 *
 *---------------------------------------------------------------------*/

#ifndef ARKLIB_INTERFACES_ALLOCATOR_H_
#define ARKLIB_INTERFACES_ALLOCATOR_H_

/*------------------------------------------------------------------------------------------------------
 * Name                                 Arena   Uses buffer   Region   OOM Fallback?   Mark?   Reset?
 * ARK_ARENA_ALLOCATOR                  Yes     Yes           Heap     Yes             No      Yes
 * ARK_LIBC_ALLOCATOR                   No      No            Heap     Yes             No      No
 * ARK_MONOTONIC_BUFFER_ALLOCATOR       Yes     Yes           Stack    No              Yes     Yes
 * ARK_POOL                             No      No            Stack    No              No      No
 -----------------------------------------------------------------------------------------------------*/

#import <stdckdint.h>
#import <string.h>

#import "internals/arkdefs.h"
#import "math/arkbits.h"

ARK_BEGIN

#ifdef ARK_NAMESPACE
	#define talloc			  ark_talloc
	#define static_alloc	  ark_static_alloc
	#define allocator		  ark_allocator
	#define allocator_type	  ark_allocator_type
	#define allocator_alloc	  ark_allocator_alloc
	#define allocator_realloc ark_allocator_realloc
	#define allocator_dealloc ark_allocator_dealloc
	#define alloc_fn_t		  ark_alloc_fn_t
	#define realloc_fn_t	  ark_realloc_fn_t
	#define dealloc_fn_t	  ark_dealloc_fn_t
#endif

#ifndef ARK_DEFAULT_ALLOCATOR
	#define ARK_DEFAULT_ALLOCATOR ARK_LIBC_ALLOCATOR
#endif

#define ark_talloc(...) ark_pp_concat (ark_talloc_, ark_pp_nargs (__VA_ARGS__)) (__VA_ARGS__)

#define ark_talloc_1(Ptr_) ARK_MALLOC (sizeof (*(Ptr_)))

#define ark_talloc_2(Ptr_, Count_)                                                                                     \
	({                                                                                                                 \
		sz	__bytes_;                                                                                                  \
		sz	__tsize_ = sizeof (*(Ptr_));                                                                               \
		u0* __ptr_	 = nullptr;                                                                                        \
		$unlikely (ckd_mul (&__bytes_, Count_, __tsize_)) {                                                            \
			ARK_WARN ("arithmetic operation (sizeof(" #Ptr_ ") * " #Count_ " overflows size_t");                       \
		}                                                                                                              \
		else {                                                                                                         \
			__ptr_ = ARK_MALLOC (__bytes_);                                                                            \
		}                                                                                                              \
		__ptr_;                                                                                                        \
	})

#define ark_talloc_3(Ptr_, Count_, Align_)                                                                             \
	({                                                                                                                 \
		$assert_internal (ark_math_is_pow2 (Align_) && "alignment must be a power of 2");                              \
		sz	__bytes_;                                                                                                  \
		sz	__tsize_ = sizeof (*(Ptr_));                                                                               \
		u0* __ptr_	 = nullptr;                                                                                        \
		$unlikely (ckd_mul (&__bytes_, Count_, __tsize_)) {                                                            \
			ARK_WARN ("arithmetic operation (sizeof(" #Ptr_ ") * " #Count_ " overflows size_t");                       \
		}                                                                                                              \
		else {                                                                                                         \
			__ptr_ = ARK_ALIGNED_ALLOC ((Align_), __bytes_);                                                           \
		}                                                                                                              \
		__ptr_;                                                                                                        \
	})

#ifndef ark_static_alloc
	#define ark_static_alloc alloca
#endif

#define __ark_dealloc_(Ptr_) ARK_FREE (Ptr_);
#define ark_dealloc(...)	 ark_pp_foreach (__ark_dealloc_, __VA_ARGS__)

#define ark_allocator_alloc(...) ark_pp_concat (ark_allocator_alloc_, ark_pp_nargs (__VA_ARGS__)) (__VA_ARGS__)

#define ark_allocator_alloc_2(Allocator_, Type_)                                                                       \
	({                                                                                                                 \
		auto __allocator_ = (Allocator_);                                                                              \
		auto __ptr_		  = __allocator_._vptr->_alloc (__allocator_._ctx, sizeof (Type_), alignof (Type_));           \
		ARK_DCHECK (__ptr_) {                                                                                          \
			ark_report_allocation (__allocator_._vptr->_type, __ptr_, sizeof (Type_), alignof (Type_));                \
		}                                                                                                              \
		__ptr_;                                                                                                        \
	})

#define ark_allocator_alloc_3(Allocator_, Alloc_Sz_, Align_)                                                           \
	({                                                                                                                 \
		auto __allocator_  = (Allocator_);                                                                             \
		auto ___alloc_sz__ = (Alloc_Sz_);                                                                              \
		auto __align_	   = (Align_);                                                                                 \
		auto __ptr_		   = __allocator_._vptr->_alloc (__allocator_._ctx, ___alloc_sz__, __align_);                  \
		ARK_DCHECK (__ptr_) {                                                                                          \
			ark_report_allocation (__allocator_._vptr->_type, __ptr_, ___alloc_sz__, __align_);                        \
		}                                                                                                              \
		__ptr_;                                                                                                        \
	})

#define ark_allocator_calloc(Allocator_, Alloc_Sz_, Align_)                                                            \
	({                                                                                                                 \
		auto __allocator_  = (Allocator_);                                                                             \
		auto ___alloc_sz__ = (Alloc_Sz_);                                                                              \
		auto __align_	   = (Align_);                                                                                 \
		auto __ptr_		   = __allocator_._vptr->_alloc (__allocator_._ctx, ___alloc_sz__, __align_);                  \
		if (__ptr_) {                                                                                                  \
			ARK_MEMSET (__ptr_, 0, ___alloc_sz__);                                                                     \
			ark_report_allocation (__allocator_._vptr->_type, __ptr_, ___alloc_sz__, __align_);                        \
		}                                                                                                              \
		__ptr_;                                                                                                        \
	})

#define ark_allocator_talloc(...) ark_pp_concat (ark_talloc_, ark_pp_nargs (__VA_ARGS__)) (__VA_ARGS__)

#define ark_allocator_talloc_2(Allocator_, Ptr_) ark_allocator_alloc_2 ((Allocator), typeof (*(Ptr_)))

#define ark_allocator_talloc_3(Allocator_, Ptr_, Count_)                                                               \
	({                                                                                                                 \
		sz	__bytes_;                                                                                                  \
		sz	__tsize_ = sizeof (*(Ptr_));                                                                               \
		u0* __ptr_	 = nullptr;                                                                                        \
		$unlikely (ckd_mul (&__bytes_, Count_, __tsize_)) {                                                            \
			ARK_WARN ("arithmetic operation (sizeof(" #Ptr_ ") * " #Count_ " overflows size_t");                       \
		}                                                                                                              \
		else {                                                                                                         \
			__ptr_ = ark_allocator_alloc_3 ((Allocator_), __bytes_, alignof (*(Ptr_)));                                \
		}                                                                                                              \
		__ptr_;                                                                                                        \
	})

#define ark_allocator_realloc(...) ark_pp_concat (ark_allocator_realloc_, ark_pp_nargs (__VA_ARGS__)) (__VA_ARGS__)

#define ark_allocator_realloc_4(Obj_, Old_Sz_, New_Sz_, Align_)                                                        \
	({                                                                                                                 \
		auto				 __old_sz_ = (Old_Sz_);                                                                    \
		auto				 __new_sz_ = (New_Sz_);                                                                    \
		auto				 __align_  = (Align_);                                                                     \
		__decl_unused__ auto __old_	   = (Obj_);                                                                       \
		(Obj_) = ark_allocator_realloc_fn ((Obj_)->_allocator, (Obj_), __old_sz_, __new_sz_, __align_);                \
		ARK_DCHECK (((Obj_)), {                                                                                        \
			ark_report_reallocation ((Obj_)->_allocator._vptr->_type, __old_, (Obj_), __old_sz_, __new_sz_, __align_); \
		})                                                                                                             \
		(Obj_);                                                                                                        \
	})

#define ark_allocator_realloc_5(Allocator_, Ptr_, Old_Sz_, New_Sz_, Align_)                                            \
	({                                                                                                                 \
		auto __allocator_ = (Allocator_);                                                                              \
		auto __ptr_		  = (Ptr_);                                                                                    \
		auto __old_sz_	  = (Old_Sz_);                                                                                 \
		auto __new_sz_	  = (New_Sz_);                                                                                 \
		auto __align_	  = (Align_);                                                                                  \
		auto __old_		  = __ptr_;                                                                                    \
		__ptr_			  = ark_allocator_realloc_fn (__allocator_, __ptr_, __old_sz_, __new_sz_, __align_);           \
		ARK_DCHECK ((__ptr_ && __ptr_ != __old_), {                                                                    \
			ark_report_reallocation (__allocator_._vptr->_type, __old_, __ptr_, __old_sz_, __new_sz_, __align_);       \
		})                                                                                                             \
		__ptr_;                                                                                                        \
	})

#define ark_allocator_dealloc(...) ark_pp_concat (ark_allocator_dealloc_, ark_pp_nargs (__VA_ARGS__)) (__VA_ARGS__)

#define ark_allocator_dealloc_1(Obj_)                                                                                  \
	do {                                                                                                               \
		auto __obj_		  = (Obj_);                                                                                    \
		auto __allocator_ = __obj_->_allocator;                                                                        \
		if (__allocator_._vptr->_dealloc) {                                                                            \
			ark_report_deallocation (__obj_);                                                                          \
			__allocator_._vptr->_dealloc (__allocator_._ctx, __obj_);                                                  \
		}                                                                                                              \
	} while (0)

#define ark_allocator_dealloc_2(Allocator_, Ptr_)                                                                      \
	do {                                                                                                               \
		auto __allocator_ = (Allocator_);                                                                              \
		auto __ptr_		  = (Ptr_);                                                                                    \
		if (__allocator_._vptr->_dealloc) {                                                                            \
			ark_report_deallocation (__ptr_);                                                                          \
			__allocator_._vptr->_dealloc (__allocator_._ctx, __ptr_);                                                  \
		}                                                                                                              \
	} while (0)

#define ark_allocator_vdealloc(Allocator_, ...)                                                                        \
	do {                                                                                                               \
		auto __allocator_ = (Allocator_);                                                                              \
		if (__allocator_._vptr->_dealloc) {                                                                            \
			u0* const __ptrs_[] = {__VA_ARGS__};                                                                       \
			$for (i, ark_pp_nargs (__VA_ARGS__)) {                                                                     \
				ark_report_deallocation (__ptrs_[i]);                                                                  \
				__allocator_._vptr->_dealloc (__allocator_._ctx, __ptrs_[i]);                                          \
			}                                                                                                          \
		}                                                                                                              \
	} while (0)

typedef $fn (u0*, ark_alloc_fn_t) (u0* ctx_, sz size_, sz alignment_);
typedef $fn (u0*, ark_realloc_fn_t) (u0* ctx_, u0* ptr_, sz old_sz_, sz new_sz_, sz alignment_);
typedef $fn (u0, ark_dealloc_fn_t) (u0* ctx_, u0* ptr_);
typedef $fn (u0, ark_reset_fn_t) (u0* ctx_);
typedef $fn (u0, ark_rewind_fn_t) (u0* ctx_, sz mark_);

ARK_ENUM (ark_allocator_type) {
	ARK_ALLOCATOR_TYPE_NOT_SET = 0,
	ARK_DYNAMIC_ALLOCATOR	   = ARK_HEAP_ALLOCATION,
	ARK_STATIC_ALLOCATOR	   = ARK_STACK_ALLOCATION,
};

ARK_INTERFACE (ark_allocator_vtable) {
	ark_alloc_fn_t	   _alloc;
	ark_realloc_fn_t   _realloc;
	ark_dealloc_fn_t   _dealloc;
	ark_reset_fn_t	   _reset;
	ark_rewind_fn_t	   _rewind;
	ark_allocator_type _type;
};

ARK_INTERFACE (ark_allocator) {
	u0* __restrict__ _ctx;
	const ark_allocator_vtable* __noalias__ _vptr;
};

__decl_nodiscard__ static inline u0* __nullable__ ark_allocator_realloc_fn (
	ark_allocator allocator_, u0* __nonnull__ ptr_, sz old_sz_, sz new_sz_, sz alignment__
) __attr_alloc_params__ (4, 5) __attr_noexcept__;

__decl_nodiscard__ __decl_static_inline__ bool
ark_calc_alloc_size (sz* alloc_sz_, sz hdr_sz_, sz count_, sz elem_sz_, sz alignment_) __attr_noexcept__;

u0* ark_allocator_realloc_fn (ark_allocator allocator_, u0* __nonnull__ ptr_, sz old_sz_, sz new_sz_, sz alignment_) {
	if (allocator_._vptr->_realloc) {
		return allocator_._vptr->_realloc (allocator_._ctx, ptr_, old_sz_, new_sz_, alignment_);
	} else {
		u0* ptr = allocator_._vptr->_alloc (allocator_._ctx, new_sz_, alignment_);
		if (ptr) {
			ARK_MEMMOVE (ptr, ptr_, ark_math_min (old_sz_, new_sz_));
			if (allocator_._vptr->_dealloc) {
				allocator_._vptr->_dealloc (allocator_._ctx, ptr_);
			}
			return ptr;
		} else {
			return nullptr;
		}
	}
}

bool ark_calc_alloc_size (sz* out_, sz hdr_sz_, sz count_, sz elem_sz_, sz alignment_) {
	$assert_internal (ark_math_is_pow2 (alignment_));
	sz bytes;
	if (ckd_mul (&bytes, count_, elem_sz_) || ckd_add (&bytes, bytes, hdr_sz_)) return false;
	if (alignment_ > 1) {
		if (ckd_add (&bytes, bytes, alignment_ - 1)) return false;
		bytes = ark_math_align_down (bytes, alignment_);
	}
	*out_ = bytes;
	return true;
}

ARK_END

#endif /* ARKLIB_INTERFACES_ALLOCATOR_H_ */
