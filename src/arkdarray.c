#import "collections/arkdarray.h"
#import "stdckdint.h"

ARK_BEGIN

static constexpr sz MIN_INIT_GROWTH_STEP = 4;
static constexpr sz GROWTH_DENOMINATOR	 = 2;

ARKINTERNAL __decl_nodiscard__ __decl_inline__ sz
next_capacity (sz current_, sz needed_, sz grow_factor_num_) __attr_noexcept__ {
	sz next_capacity;

	if (!current_) {
		next_capacity = MIN_INIT_GROWTH_STEP;
	} else if (ckd_mul (&next_capacity, current_, grow_factor_num_)) {
		next_capacity = needed_;
	} else {
		next_capacity /= GROWTH_DENOMINATOR;
	}
	return (next_capacity >= needed_) ? next_capacity : needed_;
}

ARKEXPORT __decl_overloadable__ ark_darray_expect_t
ark_make_darray_fn (ark_allocator allocator_, sz capacity_, sz hdr_sz_, sz elem_sz_, sz alignment_) {
	$assert_internal (ark_math_is_pow2 (alignment_) && "alignment must be a power of 2");
	$assert_internal (elem_sz_ > 0 && "invalid element size");

	sz alloc_sz;

	if (!ark_calc_alloc_size (&alloc_sz, hdr_sz_, capacity_, elem_sz_, alignment_)) {
		return ark_unexpected (ARK_DARRAY, SIZE_OVERFLOW, "allocation size overflows size_t");
	}

	ark_darray da = ark_allocator_alloc (allocator_, alloc_sz, alignment_);
	if (!da) {
		return ark_unexpected (ARK_DARRAY, OUT_OF_MEMORY, "allocation failed");
	}

	da->_allocator = allocator_;
	da->_capacity  = capacity_;
	da->_size	   = 0;
	return ark_expected (ARK_DARRAY, (u0*)da);
}

ARKEXPORT __decl_overloadable__ ark_darray_expect_t
ark_make_darray_fn (ark_allocator allocator_, sz hdr_sz_, sz elem_sz_, sz alignment_) {
	return ark_make_darray_fn (allocator_, ARK_DARRAY_INIT_CAPACITY, hdr_sz_, elem_sz_, alignment_);
}

ARKEXPORT __decl_overloadable__ ark_darray_expect_t
ark_make_darray_fn (sz capacity_, sz hdr_sz_, sz elem_sz_, sz alignment_) {
	return ark_make_darray_fn (ARK_DARRAY_DEFAULT_ALLOCATOR, capacity_, hdr_sz_, elem_sz_, alignment_);
}

ARKEXPORT ark_darray_fault
ark_darray_reserve_fn (ark_darray* darray_, sz capacity_, sz hdr_sz_, sz elem_sz_, sz alignment_) {
	ark_darray da = *darray_;

	if (capacity_ < da->_size) capacity_ = da->_size;

	sz old_bytes, new_bytes;

	if (!ark_calc_alloc_size (&old_bytes, hdr_sz_, da->_capacity, elem_sz_, alignment_)
		|| !ark_calc_alloc_size (&new_bytes, hdr_sz_, capacity_, elem_sz_, alignment_)) {
		return ark_exception_from (ARK_DARRAY_SIZE_OVERFLOW);
	}

	ark_allocator_realloc (da, old_bytes, new_bytes, alignment_);

	if (!da) {
		return ark_exception_from (ARK_DARRAY_OUT_OF_MEMORY);
	}

	da->_capacity = capacity_;
	*darray_	  = da;
	sz	offset	  = (da->_size) ? elem_sz_ : 0;
	u0* begin	  = ((u8*)da->_data + offset);
	return ark_exception_from (ARK_FAULT_SUCCESS);
}

ARKEXPORT ark_exception ark_darray_shrink_to_fit_fn (ark_darray* darray_, sz hdr_sz_, sz elem_sz_, sz alignment_) { }

ARKEXPORT ark_darray_fault
ark_darray_grow_fn (ark_darray* darray_, sz extra_, f64 grow_factor_num_, sz hdr_sz_, sz elem_sz_, sz alignment_) {
	sz needed;

	if (ckd_add (&needed, (*darray_)->_size, extra_)) {
		return ARK_DARRAY_SIZE_OVERFLOW;
	}

	return ark_darray_reserve_fn (
		darray_, next_capacity ((*darray_)->_capacity, needed, grow_factor_num_), hdr_sz_, elem_sz_, alignment_
	);
}

ARK_END
