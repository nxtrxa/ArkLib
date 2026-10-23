#import <fcntl.h>
#import <pthread.h>
#import <stdint.h>
#import <string.h>
#import <sys/mman.h>
#import <sys/stat.h>
#import <sys/types.h>
#import <unistd.h>

#import "allocators/async/arkasync_arena.h"
#import "collections/arkdarray.h"
#import "collections/arkiterator.h"
#import "core/arkthread.h"
#import "math/arkbits.h"
#import "math/arkpair.h"

ARK_BEGIN

#define SCOPED_MUTEX_OP(Arena_)                                                                                        \
	pthread_mutex_lock (&(Arena_)->_lock);                                                                             \
	defer {                                                                                                            \
		pthread_mutex_unlock (&(Arena_)->_lock);                                                                       \
	}

ARKEXPORT const ark_allocator_vtable ark_async_arena_allocator_vtable = {
	._alloc = (ark_alloc_fn_t)ark_async_arena_alloc,
	._reset = (ark_reset_fn_t)ark_reset_async_arena,
	._type	= ARK_DYNAMIC_ALLOCATOR,
};

static constexpr sz		 LC_ARENA_DEFAULT_REGION_SIZE = KiB (64);
static constexpr sz		 LC_ARENA_MAX_REGION_SIZE	  = GiB (1);
static sz				 l_cached_huge_page_sz		  = 0;
static ark_thread_once_t l_hp_flag					  = ARK_THREAD_ONCE_INIT;

/* -------------------------------------------------------------------------
 * Packed (generation, used) state.
 *
 *   bits [63:40]  = generation (24 bits)
 *   bits [39: 0]  = bump offset (40 bits) — cap is 1 TiB, well above the
 *                   1 GiB max region, so the top bits are never lost.
 *
 * A single atomic word holds both. Any rollback (reset, temp_end) bumps the
 * generation, which invalidates any in-flight allocator CAS that captured
 * the previous generation. This is what lets the fast path go lock-free and
 * still be safe against concurrent rollback.
 * ------------------------------------------------------------------------- */

typedef u64		 state_t;
typedef unsigned bitint (40) u40;
typedef unsigned bitint (24) u24;

#define GEN_BITS  24
#define USED_BITS (64 - GEN_BITS)
#define USED_MASK ((1ULL << USED_BITS) - 1)
#define GEN_SHIFT USED_BITS

struct class_ark_async_arena {
	struct region* _head;					/* First region (never freed until destroy). */
	ark_atomic_t (struct region*) _current; /* Region we are currently bumping from. */
	ark_atomic_t (sz) _growth_prev_size;	/* Size of last region for geometric growth. */
	flag_t		_flags;
	ark_mutex_t _lock; /* Used in slow path and RESET/TEMP_END. */
};

struct region {
	u0* _base;					   /* Start of usable memory (after possible guard). */
	u0* _mapping;				   /* Points to begin of mapped memory. Differs from BASE if
									  ARK_ASYNC_ARENA_GUARD_PAGES is set. */
	sz _capacity;				   /* Usable _size of this region. */
	ark_atomic_t (state_t) _state; /* (gen, used) packed. */
	struct region* _next;		   /* Linked list. */
	sz			   _mapped;		   /* Size of mapped region */
};

static constexpr sz REGION_HEADER_SIZE = ark_math_align_up (sizeof (struct region), ARK_MATH_MAX_ALIGN);

ARKINTERNAL __decl_inline__ state_t pack (sz used_, u24 gen_) __attr_noexcept__ {
	return ((state_t)(used_ & USED_MASK)) | ((state_t)gen_ << GEN_SHIFT);
}

ARKINTERNAL __decl_inline__ u40 unpack_used (state_t s_) __attr_noexcept__ {
	return s_ & USED_MASK;
}

ARKINTERNAL __decl_inline__ u24 unpack_gen (state_t s_) __attr_noexcept__ {
	return (u24)(s_ >> GEN_SHIFT);
}

/* Return the system page _size (cached). */
ARKINTERNAL __decl_inline__ sz page_size (u0) __attr_noexcept__ {
	static ark_atomic_t (sz) cached = 0;
	$unlikely (!atomic_load_explicit (&cached, memory_order_relaxed)) {
		sz ps = sysconf (_SC_PAGESIZE);
		atomic_store_explicit (&cached, ps, memory_order_relaxed);
	}
	return atomic_load_explicit (&cached, memory_order_relaxed);
}

ARKINTERNAL u0 init_huge_page_sz (u0) {
	$likely (!l_cached_huge_page_sz) {
		/* 1. Fast path: read PMD huge page _size from sysfs */
		int fd = open ("/sys/kernel/mm/transparent_hugepage/hpage_pmd_size", O_RDONLY | O_CLOEXEC);
		if (fd >= 0) {
			ark_class_array (char, 32, buff);
			ssz n = read (fd, buff->_data, buff->_size - 1);
			close (fd);
			if (n > 0) {
				buff->_data[n] = '\0';
				sz v		   = strtoull (buff->_data, nullptr, 10);
				if (v > 0) {
					l_cached_huge_page_sz = v;
					return;
				}
			}
		}
	}

	/* Fallback: parse /proc/meminfo regardless of why the sysfs path failed. */
	FILE* f = fopen ("/proc/meminfo", "re");
	if (f) {
		defer {
			fclose (f);
		}
		ark_class_array (char, 128, line);
		while (fgets (line->_data, line->_size, f)) {
			sz kb = 0;
			if (sscanf (line->_data, "Hugepagesize: %zu kB", (size_t*)&kb) == 1 && kb > 0) {
				l_cached_huge_page_sz = kb * 1024;
				return;
			}
		}
	}

/* Arch default. */
#if defined(__x86_64__) || defined(_M_X64)
	l_cached_huge_page_sz = MiB (2);
#elif defined(__aarch64__) || defined(_M_ARM64)
	l_cached_huge_page_sz = (page_size () == KiB (64)) ? MiB (512) : MiB (2);
#else
	l_cached_huge_page_sz = page_size () * 512;
#endif
}

ARKINTERNAL sz huge_page_size () {
	ark_thread_call_once (init_huge_page_sz, &l_hp_flag);
	return l_cached_huge_page_sz;
}

/* Determine the _size for a new struct region using a geometric growth policy.
 */
ARKINTERNAL __decl_inline__ sz next_region_size (sz prev_size_, sz min_size_) __attr_noexcept__ {
	sz _next = prev_size_ * 2;
	if (_next < min_size_) _next = min_size_;
	if (_next > LC_ARENA_MAX_REGION_SIZE && min_size_ <= LC_ARENA_MAX_REGION_SIZE) _next = LC_ARENA_MAX_REGION_SIZE;
	return _next;
}
/*
 * Create a new memory struct region, possibly with guard pages and/or huge
 * pages.
 *
 * - If ARENA_GUARD_PAGES is set, the mapping includes a PROT_NONE page before
 *   and after the usable area. The returned `region->_base` points after the
 *   leading guard.
 * - If ARENA_HUGE_PAGES is set, the _size is rounded up to a huge page boundary
 *   and MAP_HUGETLB is passed to mmap(2).
 *
 * @param SIZE          Usable _size (will be adjusted for alignment/guards).
 * @param FLAGS         ARK_ASYNC_ARENA flags.
 * @return              Pointer to a new struct region structure (aborts on
 * failure).
 */
ARKINTERNAL __decl_malloc__ __decl_returns_nonnull__ struct region* make_region (sz size_, flag_t flags_) {
	sz	page_sz		 = page_size ();
	sz	payload_sz	 = size_;
	int mmap_flags	 = MAP_PRIVATE | MAP_ANONYMOUS;
	sz	guard_offset = (flags_ & ARK_ASYNC_ARENA_GUARD_PAGES) ? page_sz : 0;

	/* Huge page handling: round up _size and add MAP_HUGETLB. */
	if (flags_ & ARK_ASYNC_ARENA_HUGE_PAGES) {
		payload_sz	= ark_math_align_up (payload_sz, huge_page_size ());
		mmap_flags |= MAP_HUGETLB;
	} else {
		payload_sz = ark_math_align_up (payload_sz, page_sz);
	}

	sz total_sz = REGION_HEADER_SIZE + (2 * guard_offset) + payload_sz;
	total_sz	= ark_math_align_up (total_sz, page_sz);

	u8* mapping = mmap (nullptr, total_sz, PROT_READ | PROT_WRITE, mmap_flags, -1, 0);
	/* If huge pages fail, retry without MAP_HUGETLB. */
	if (mapping == MAP_FAILED && mmap_flags & MAP_HUGETLB) {
		mmap_flags &= ~MAP_HUGETLB;
		mapping		= mmap (nullptr, total_sz, PROT_READ | PROT_WRITE, mmap_flags, -1, 0);
	}
	if (mapping == MAP_FAILED) ARK_PANIC ("arena: mmap failed");


	/* Apply guard page protection. */
	if (flags_ & ARK_ASYNC_ARENA_GUARD_PAGES) {
		/* Leading guard page. */
		if (mprotect (mapping, page_sz, PROT_NONE) != 0) {
			ARK_PANIC ("arena: mprotect(guard) failed");
		}
		/* Trailing guard page. */
		u0* trailing = mapping + total_sz - page_sz;
		if (mprotect (trailing, page_sz, PROT_NONE) != 0) {
			ARK_PANIC ("arena: mprotect(guard) failed");
		}
	}

	struct region* r = (struct region*)(mapping + guard_offset);
	r->_base		 = mapping + guard_offset + REGION_HEADER_SIZE;
	r->_mapping		 = mapping;
	r->_capacity	 = payload_sz; /* actual usable + guard_offset _size. */
	r->_next		 = nullptr;
	r->_mapped		 = total_sz;
	atomic_store_explicit (&r->_state, pack (0, 0), memory_order_relaxed);
	ark_asan_poison_mem (r->_base, r->_capacity);
	return r;
}

ARKINTERNAL u0 region_destroy (struct region* r_) __attr_noexcept__ {
	if (!r_) return;
	munmap (r_->_mapping, r_->_mapped);
}

/*
 * Slow path of arena_alloc – separated to keep the fast path lean.
 *
 * Trying to grow the _current struct region contiguously using mremap is likely
 * to always fail if MREMAP_MAYMOVE is not set. Using MREMAP_MAYMOVE would
 * invalidate all pointers in the _current struct region, so it pointer would
 * have to be remapped as well, making the operation complex and likely to fail.
 *
 */
ARKINTERNAL u0* arena_alloc_slow (ark_async_arena* __noalias__ arena_, sz size_, sz alignment_) __attr_noexcept__ {
	struct region* r	= atomic_load_explicit (&arena_->_current, memory_order_acquire);
	auto		   tail = r;

	while (r) {
		tail			 = r;
		state_t old		 = atomic_load_explicit (&r->_state, memory_order_acquire);
		u24		old_gen	 = unpack_gen (old);
		u40		old_used = unpack_used (old);

		uintptr_t offset   = (uintptr_t)r->_base + old_used;
		uintptr_t aligned  = ark_math_align_up (offset, alignment_);
		sz		  padd	   = (sz)(aligned - offset);
		sz		  new_used = old_used + size_ + padd;

		if (new_used > r->_capacity) {
			r = r->_next;
			continue;
		} // region full - try next

		state_t new_state = pack (new_used, old_gen);

		if (atomic_compare_exchange_weak_explicit (
				&r->_state, &old, new_state, memory_order_acq_rel, memory_order_acquire
			)) {
			atomic_store_explicit (&arena_->_current, r, memory_order_release);
			u0* ptr = (u0*)aligned;
			ark_asan_unpoison_mem (ptr, size_);
			return ptr;
		}

		r = r->_next;
	}

	/* Allocate new region and link it */
	sz new_size = next_region_size (
		atomic_load_explicit (&arena_->_growth_prev_size, memory_order_relaxed),
		ark_math_align_up (size_ + REGION_HEADER_SIZE, page_size ())
	);

	struct region* new_reg = make_region (new_size, arena_->_flags);
	atomic_store_explicit (&arena_->_growth_prev_size, new_reg->_capacity, memory_order_relaxed);

	tail->_next = new_reg;

	uintptr_t offset  = (uintptr_t)new_reg->_base;
	uintptr_t aligned = ark_math_align_up (offset, alignment_);
	sz		  padd	  = (sz)(aligned - offset);

	atomic_store_explicit (&new_reg->_state, pack (padd + size_, 0), memory_order_release);

	atomic_store_explicit (&arena_->_current, new_reg, memory_order_release);

	u0* ptr = (u0*)aligned;
	ark_asan_unpoison_mem (ptr, size_);
	return ptr;
}


/*
 * Public API implementation
 */


__decl_overloadable__ ark_expect (ARK_ASYNC_ARENA, ark_async_arena_p) ark_make_async_arena (sz cap_, flag_t flags_) {
	ark_async_arena* arena = ark_talloc (arena);
	if (!arena) return ark_unexpected (ARK_ASYNC_ARENA, MALLOC, "failed to allocate arena object");

	if (cap_ == 0) cap_ = LC_ARENA_DEFAULT_REGION_SIZE;

	struct region* first = make_region (cap_, flags_);
	arena->_head		 = first;
	arena->_flags		 = flags_;
	atomic_store_explicit (&arena->_current, first, memory_order_relaxed);
	atomic_store_explicit (&arena->_growth_prev_size, first->_capacity, memory_order_relaxed);

	if (pthread_mutex_init (&arena->_lock, nullptr)) {
		region_destroy (first);
		ark_dealloc (arena);
		return ark_unexpected (ARK_ASYNC_ARENA, MUTEX_INIT, "failed to initialize mutex");
	}

	ARK_INFO ("ark_async_arena created with capacity of: %zu", (size_t)cap_);

	return ark_expected (ARK_ASYNC_ARENA, arena);
}

u0 ark_destroy_async_arena (ark_async_arena* __restrict__ arena_) {
	if (!arena_) return;

	pthread_mutex_lock (&arena_->_lock);

	struct region* r = arena_->_head;
#if ARKLIB_DEBUG_BUILD
	sz used = 0, capacity = 0, rcount = 0;
#endif
	while (r) {
		struct region* _next = r->_next;
#if ARKLIB_DEBUG_BUILD
		used	 += unpack_used (atomic_load_explicit (&r->_state, memory_order_relaxed));
		capacity += r->_capacity;
		rcount++;
#endif
		region_destroy (r);
		r = _next;
	}

	pthread_mutex_unlock (&arena_->_lock);
	pthread_mutex_destroy (&arena_->_lock);

	ARK_INFO (
		"ark_async_arena destroyed.\n\tregions created: %zu\n\ttotal memory "
		"allocated: %zu\n\ttotal memory used: %zu",
		(size_t)rcount,
		(size_t)capacity,
		(size_t)used
	);
	ark_dealloc (arena_);
}

/*
 * Lock-free fast path.
 *
 * Loads the packed (GEN, USED) from CURRENT, computes the new offset, and
 * CASes the packed word back. On failure:
 *   - If only `USED` changed, another allocator won the race — retry the
 *     same region with the fresh value.
 *   - If `GEN` changed, a RESET or TEMP_END ran concurrently — reload
 *     CURRENT (it may have moved) and retry from scratch.
 */
u0* ark_async_arena_alloc (ark_async_arena* __noalias__ arena_, sz size_, sz alignment_) {
	$assert_internal (size_ > 0 && "trying to allocate zero bytes");
	$assert_internal (ark_math_is_pow2 (alignment_) && "passed a non power of two alignment");

	struct region* curr = atomic_load_explicit (&arena_->_current, memory_order_acquire);

	for (;;) {
		state_t old		 = atomic_load_explicit (&curr->_state, memory_order_acquire);
		u24		old_gen	 = unpack_gen (old);
		sz		old_used = unpack_used (old);

		uintptr_t offset   = (uintptr_t)curr->_base + old_used;
		uintptr_t aligned  = ark_math_align_up (offset, alignment_);
		sz		  padd	   = (sz)(aligned - offset);
		sz		  new_used = old_used + padd + size_;

		$unlikely (new_used > curr->_capacity) {
			pthread_mutex_lock (&arena_->_lock);
			u0* p = arena_alloc_slow (arena_, size_, alignment_);
			pthread_mutex_unlock (&arena_->_lock);
			return p;
		}

		state_t new_state = pack (new_used, old_gen);
		if (atomic_compare_exchange_weak_explicit (
				&curr->_state, &old, new_state, memory_order_acq_rel, memory_order_acquire
			)) {
			u0* ptr = (u0*)aligned;
			ark_asan_unpoison_mem (ptr, size_);
			return ptr;
		}
		/* CAS failed. `OLD` now holds the fresh state. */
		$unlikely (unpack_gen (old) != old_gen) {
			curr = atomic_load_explicit (&arena_->_current, memory_order_acquire);
		}
	}
}

/*
 * Reset: bump generation on every region, zero the bump offset. No
 * quiescence needed — in-flight allocator CASes fail and retry.
 */
u0 ark_reset_async_arena (ark_async_arena* __noalias__ arena_) {
	SCOPED_MUTEX_OP (arena_);

	sz page_sz = page_size ();

	$llforeach (r, arena_->_head, _next) {
		state_t old	 = atomic_load_explicit (&r->_state, memory_order_relaxed);
		u24		gen	 = unpack_gen (old);
		u40		used = unpack_used (old);

		if ((arena_->_flags & ARK_ASYNC_ARENA_ZERO_ON_RESET) && used > 0) {
			uintptr_t start = ark_math_align_up ((uintptr_t)r->_base, page_sz);
			uintptr_t end	= ark_math_align_down ((uintptr_t)(r->_base + used), page_sz);
			if (end > start) madvise ((u0*)start, end - start, MADV_DONTNEED);
		}
		atomic_store_explicit (&r->_state, pack (0, gen + 1), memory_order_release);
		ark_asan_poison_mem (r->_base, r->_capacity);
	}
	atomic_store_explicit (&arena_->_current, arena_->_head, memory_order_release);
}

/*
 * Temp arena: take a snapshot, roll everything back on end.
 *
 * Only regions touched during the scope are affected:
 *   - the checkpoint region is rolled back to its snapshot offset (gen bumped)
 *   - every subsequent region is zeroed (gen bumped)
 *
 * Semantically the checkpoint only makes sense if no reset ran in between.
 * Do not interleave reset with an active temp scope on the same arena.
 */
ARKEXPORT ark_temp_async_arena ark_async_arena_temp_begin (ark_async_arena* __noalias__ arena_) {
	$assert_internal (arena_ && "passed nullptr");

	struct region* reg	= atomic_load_explicit (&arena_->_current, memory_order_relaxed);
	sz			   used = unpack_used (atomic_load_explicit (&reg->_state, memory_order_relaxed));

	return (ark_temp_async_arena) {
		._arena	 = arena_,
		._region = reg,
		._used	 = used,
	};
}

ARKEXPORT u0 ark_async_arena_temp_end (ark_temp_async_arena* __noalias__ temp_) {
	$unlikely (!temp_->_arena || !temp_->_region) return;

	ark_async_arena* arena	 = temp_->_arena;
	struct region*	 region	 = temp_->_region;
	sz				 page_sz = page_size ();

	SCOPED_MUTEX_OP (arena);

	{
		state_t old	 = atomic_load_explicit (&region->_state, memory_order_relaxed);
		u24		gen	 = unpack_gen (old);
		u40		used = unpack_used (old);

		if (used > temp_->_used) {
			if (arena->_flags & ARK_ASYNC_ARENA_ZERO_ON_RESET) {
				uintptr_t start = ark_math_align_up ((uintptr_t)region->_base + temp_->_used, page_sz);
				uintptr_t end	= ark_math_align_down ((uintptr_t)region->_base + used, page_sz);
				if (end > start) madvise ((u0*)start, end - start, MADV_DONTNEED);
			}
			ark_asan_poison_mem (region->_base + temp_->_used, used - temp_->_used);
			atomic_store_explicit (&region->_state, pack (temp_->_used, gen + 1), memory_order_release);
		} else {
			/* Nothing to roll back, but still bump gen to invalidate in-flight
			  CASes. */
			atomic_store_explicit (&region->_state, pack (used, gen + 1), memory_order_release);
		}
	}

	/* Subsequent regions: zero everything, bump gen. */
	$llforeach (r, region->_next, _next) {
		state_t old	 = atomic_load_explicit (&r->_state, memory_order_relaxed);
		u24		gen	 = unpack_gen (old);
		u40		used = unpack_used (old);

		if ((arena->_flags & ARK_ASYNC_ARENA_ZERO_ON_RESET) && used > 0) {
			uintptr_t start = ark_math_align_up ((uintptr_t)r->_base, page_sz);
			uintptr_t end	= ark_math_align_down ((uintptr_t)(r->_base + used), page_sz);
			if (end > start) madvise ((u0*)start, end - start, MADV_DONTNEED);
		}
		atomic_store_explicit (&r->_state, pack (0, gen + 1), memory_order_release);
		ark_asan_poison_mem (r->_base, r->_capacity);
	}
	atomic_store_explicit (&arena->_current, region, memory_order_release);
}

ARK_END
