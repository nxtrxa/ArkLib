#include "allocators/arklibc_allocator.h"

ARK_BEGIN

ARKEXPORT const ark_allocator_vtable ark_libc_allocator_vtable = {
	._alloc	  = ARK_LIBC_ALLOCATOR_ALLOC,
	._realloc = ARK_LIBC_ALLOCATOR_REALLOC,
	._dealloc = ARK_LIBC_ALLOCATOR_DEALLOC,
	._type	  = ARK_DYNAMIC_ALLOCATOR,
};

ARKEXPORT u0* ark_libc_alloc (__decl_unused__ u0* __restrict__ ctx_, sz size_, sz alignment_) {
	$assert_internal (ark_math_is_pow2 (alignment_) && "alignment must be a power of 2");
	if (!size_) return nullptr;
	if (alignment_ <= ARK_MATH_MAX_ALIGN) {
		return ARK_MALLOC (size_);
	} else {
		return ARK_ALIGNED_ALLOC (alignment_, size_);
	}
}

ARKEXPORT u0*
ark_libc_realloc (__decl_unused__ u0* __restrict__ ctx_, u0* ptr_, sz old_sz_, sz new_sz_, sz alignment_) {
	if (!ptr_) {
		return ARK_MALLOC (new_sz_);
	}
	if (!new_sz_) {
		ark_dealloc (ptr_);
		return nullptr;
	}

	$assert_internal (ark_math_is_pow2 (alignment_) && "alignment must be a power of 2");

	u0* nptr = nullptr;
	if (alignment_ <= ARK_MATH_MAX_ALIGN) {
		nptr = ARK_REALLOC (ptr_, new_sz_);
		ARK_DCHECK (!nptr, { ARK_WARN ("realloc failed"); })
		return nptr;
	} else {
		nptr = ARK_ALIGNED_ALLOC (alignment_, new_sz_);
		ARK_DCHECK (!nptr, { ARK_WARN ("realloc failed"); })
		ARK_MEMCPY (nptr, ptr_, ark_math_min (old_sz_, new_sz_));
		ark_dealloc (ptr_);
		return nptr;
	}
}

ARKEXPORT u0 ark_libc_dealloc (__decl_unused__ u0* __restrict__ ctx_, u0* __restrict__ ptr_) {
	if (!ptr_) return;
	ark_dealloc (ptr_);
}

ARK_END
