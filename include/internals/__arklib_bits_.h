#ifndef ARK_INTERNALS_BITS_H_
#define ARK_INTERNALS_BITS_H_

#import "__arklib_features_.h"

ARK_BEGIN

#ifdef ARK_NAMESPACE
	#define MIN				ARK_MIN
	#define MAX				ARK_MAX
	#define COUNTOF			ARK_COUNTOF
	#define IS_CARRAY		ARK_IS_C_ARRAY
	#define IS_C_ARRAY		ARK_IS_C_ARRAY
	#define IS_POW2			ARK_IS_POW2
	#define ALIGN_UP_POW2	ARK_ALIGN_UP_POW2
	#define ALIGN_DOWN_POW2 ARK_ALIGN_DOWN_POW2
	#define ALIGN_UP		ARK_ALIGN_UP
	#define MAX_ALIGN		ARK_MAX_ALIGN
#endif

/* Size Units */
#if !defined(KiB) && !defined(MiB) && !defined(GiB)
	#define KiB(N_) ((u64)(N_) << 10)
	#define MiB(N_) ((u64)(N_) << 20)
	#define GiB(N_) ((u64)(N_) << 30)
#endif

/* Array Capacity and Bounds Inspection */
#ifdef countof
	#define ARK_COUNTOF(CArray_) countof (CArray_)
#else
	#define ARK_COUNTOF(CArray_)                                                                                       \
		_Generic (                                                                                                     \
			&(CArray_), typeof (*CArray_) (*)[]: (size_t)(sizeof (CArray_) / sizeof (*(CArray_))), default: nullptr    \
		)
#endif

#define ARK_IS_C_ARRAY(Ptr_) _Generic (&(Ptr_), typeof (*(Ptr_)) (*)[]: true, default: false)

#define ARK_CARRAY_SIZEOF(CArray_) (sz) (sizeof (CArray_) / sizeof (*(CArray_)))

ARK_END

#endif /* ARK_INTERNALS_BITS_H_ */
