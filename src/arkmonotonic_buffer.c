#import "allocators/arkmonotonic_buffer.h"
#import "math/arkbits.h"

ARK_BEGIN

ARKEXPORT const ark_allocator_vtable ark_monotonic_buffer_allocator_vtable = {
	._alloc	 = (ark_alloc_fn_t)ark_monotonic_buffer_alloc,
	._reset	 = (ark_reset_fn_t)ark_monotonic_buffer_reset,
	._rewind = (ark_rewind_fn_t)ark_monotonic_buffer_rewind,
	._type	 = ARK_STATIC_ALLOCATOR,
};

ARKEXPORT u0* ark_monotonic_buffer_alloc (
	ark_internal_symbol (monotonic_buffer) * __noalias__ ctx_, sz alloc_sz_, sz alignment_
) __attr_alloc_params__ (2, 3) {
	if ((alignment_ == 0 || alloc_sz_ == 0) || (alignment_ % 2 != 0 && alignment_ != 1))
		ARK_PANIC ("invalid alignment");

	uintptr_t offset  = (uintptr_t)(ctx_->base + ctx_->used);
	sz		  aligned = ark_math_align_up (offset, alignment_);
	sz		  padd	  = aligned - offset;

	if (ctx_->used + alloc_sz_ + padd >= ctx_->capacity) return nullptr;

	u0* ptr		= (u0*)(offset + padd);
	ctx_->used += alloc_sz_;
	return ptr;
}

ARKEXPORT u0 ark_monotonic_buffer_reset (ark_internal_symbol (monotonic_buffer) * __noalias__ mbuff_) {
	mbuff_->used = 0;
}

ARKEXPORT u0 ark_monotonic_buffer_rewind (ark_internal_symbol (monotonic_buffer) * __noalias__ mbuff_, sz mark_) {
	if (mark_ >= mbuff_->used) mark_ = mbuff_->used;
	mbuff_->used = mark_;
}

ARK_END
