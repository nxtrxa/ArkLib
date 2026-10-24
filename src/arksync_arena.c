#import <fcntl.h>
#import <pthread.h>
#import <stdint.h>
#import <string.h>
#import <sys/mman.h>
#import <sys/stat.h>
#import <sys/types.h>
#import <unistd.h>

#import "allocators/sync/arksync_arena.h"
#import "arkthread.h"
#import "collections/arkdarray.h"
#import "collections/arkiterator.h"
#import "math/arkbits.h"
#import "math/arkpair.h"

ARK_BEGIN

#define ARK_EXPECT_CONTEXT ARK_SYNC_ARENA

#define SCOPED_MUTEX_OP(Arena_)                                                                                        \
	pthread_mutex_lock (&(Arena_)->_lock);                                                                             \
	defer {                                                                                                            \
		pthread_mutex_unlock (&(Arena_)->_lock);                                                                       \
	}

ARKEXPORT const ark_allocator_vtable ark_sync_arena_allocator_vtable = {
	._alloc = (ark_alloc_fn_t)ark_sync_arena_alloc,
	._reset = (ark_reset_fn_t)ark_reset_sync_arena,
	._type	= ARK_DYNAMIC_ALLOCATOR,
};

static constexpr sz		 LC_ARENA_DEFAULT_REGION_SIZE = KiB (64);
static constexpr sz		 LC_ARENA_MAX_REGION_SIZE	  = GiB (1);
static sz				 l_cached_huge_page_sz		  = 0;
static ark_thread_once_t l_hp_flag					  = ARK_THREAD_ONCE_INIT;

struct class_ark_sync_arena {
	struct region* _head;					/* First region (never freed until destroy). */
	ark_atomic_t (struct region*) _current; /* Region we are currently bumping from. */
	ark_atomic_t (sz) _growth_prev_size;	/* Size of last region for geometric growth. */
	ark_mutex_t _lock;
	flag_t		_flags;
};

struct region {
	u0* _base;				 /* Start of usable memory (after possible guard). */
	u0* _mapping;			 /* Points to begin of mapped memory. Differs from BASE if
								ARK_SYNC_ARENA_GUARD_PAGES is set. */
	sz _capacity;			 /* Usable _size of this region. */
	ark_atomic_t (sz) _used; /* Current bump offset. */
	struct region* _next;	 /* Linked list. */
	sz			   _mapped;	 /* Size of mapped region */
};

static constexpr sz REGION_HEADER_SIZE = ark_math_align_up (sizeof (struct region), ARK_MATH_MAX_ALIGN);

/* Return the system page _size (cached). */
ARKINTERNAL __decl_inline__ sz page_size (u0) __attr_noexcept__ {
	static ark_atomic_t (sz) cached = 0;
	$unlikely (!atomic_load_explicit (&cached, memory_order_relaxed)) {
#if ARKLIB_HAS_INCLUDE(<unistd.h>)
		sz ps = sysconf (_SC_PAGESIZE);
		atomic_store_explicit (&cached, ps, memory_order_relaxed);
#elif ARKLIB_HAS_INCLUDE(<windows.h>)
		SYSTEM_INFO si;
		GetSystemInfo (&si);
		cached = si.dwPageSize;
#endif
	}
	return atomic_load_explicit (&cached, memory_order_relaxed);
}

ARKINTERNAL u0 init_huge_page_sz (u0) {
	$likely (!l_cached_huge_page_sz) {
		// because clang format is troll asf.
		// clang-format off
#if ARKLIB_HAS_INCLUDE(<sys/mman.h>) && defined(__linux__)
		/* 1. Fast path: read PMD huge page _size from sysfs */
		int fd = open ("/sys/kernel/mm/transparent_hugepage/hpage_pmd_size", O_RDONLY | O_CLOEXEC);
        if (fd >= 0) {
            ark_class_array(char, 32, buff);
            ssz n = read (fd, buff->_data, buff->_size - 1);
            close (fd);
            if (n > 0) {
                l_cached_huge_page_sz = strtoull (buff->_data, nullptr, 10);
            }
        }
        /* 2. Fallback: parse /proc/meminfo for default Hugepagesize */
        else {
            FILE* f = fopen ("/proc/meminfo", "re");
            defer {
                fclose (f);
            }
            if (f) {
                ark_class_array (char, 128, line);
                while (fgets (line->_data, line->_size, f)) {
                    sz kb = 0;
                    if (sscanf (line->_data, "Hugepagesize: %zu kB", (size_t*)&kb) == 1) {
                        l_cached_huge_page_sz = kb * 1024;
                    }
                }
            }
        }
    }
#elif ARKLIB_HAS_INCLUDE (<windows.h>)
    atomic_store_explicit(&cached, (sz)GetLargePageMinimum (), memory_order_relaxed);
#endif
    $likely (!l_cached_huge_page_sz) {
#if defined(__x86_64__) || defined(_M_X64)
        l_cached_huge_page_sz = MiB(2);
#elif defined(__aarch64__) || defined(_M_ARM64)
        l_cached_huge_page_sz = (page_size () == KiB (64)) ? MiB (512) : MiB (2)
#endif
    }
	// clang-format on
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
 * @param FLAGS         ARK_SYNC_ARENA flags.
 * @return              Pointer to a new struct region structure (aborts on
 * failure).
 */
ARKINTERNAL __decl_malloc__ __decl_returns_nonnull__ struct region* make_region (sz size_, flag_t flags_) {
	sz	page_sz		 = page_size ();
	sz	payload_sz	 = size_;
	int mmap_flags	 = MAP_PRIVATE | MAP_ANONYMOUS;
	sz	guard_offset = (flags_ & ARK_SYNC_ARENA_GUARD_PAGES) ? page_sz : 0;

	/* Huge page handling: round up _size and add MAP_HUGETLB. */
	if (flags_ & ARK_SYNC_ARENA_HUGE_PAGES) {
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
	if (mapping == MAP_FAILED) {
		ARK_PANIC ("arena: mmap failed");
	}

	/* Apply guard page protection. */
	if (flags_ & ARK_SYNC_ARENA_GUARD_PAGES) {
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
	r->_capacity	 = payload_sz; /* actual usab + guard_offset _size. */
	atomic_store_explicit (&r->_used, 0, memory_order_relaxed);
	r->_next   = nullptr;
	r->_mapped = total_sz;
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
ARKINTERNAL u0* arena_alloc_slow (ark_sync_arena* __noalias__ arena_, sz size_, sz alignment_) __attr_noexcept__ {
	struct region* curr = atomic_load_explicit (&arena_->_current, memory_order_relaxed);

	/*
	 * 1. Traverse pre-allocated regions (leftover from ARK_ARENA_RESET).
	 */
	while (curr->_next) {
		struct region* next		 = curr->_next;
		sz			   next_used = atomic_load_explicit (&next->_used, memory_order_relaxed);

		uintptr_t offset  = (uintptr_t)next->_base + next_used;
		uintptr_t aligned = ark_math_align_up (offset, alignment_);
		sz		  padd	  = (sz)(aligned - offset);

		/*
		 * Check if struct region has enough space for requested allocation
		 * _size. If not advance to next struct region.
		 */
		if (next->_capacity - next_used >= size_ + padd) {
			atomic_store_explicit (&arena_->_current, next, memory_order_relaxed);
			atomic_store_explicit (&next->_used, next_used + padd + size_, memory_order_relaxed);
			u0* ptr = (u0*)aligned;
			ark_asan_unpoison_mem (ptr, size_);
			return ptr;
		}
		curr = next;
	}

	/*
	 * 2. Allocate a brand new struct region.
	 *    Use geometric growth, but at least enough for this allocation.
	 */
	sz new_size = next_region_size (
		atomic_load_explicit (&arena_->_growth_prev_size, memory_order_relaxed),
		ark_math_align_up (size_ + REGION_HEADER_SIZE, page_size ())
	);
	struct region* new_reg = make_region (new_size, arena_->_flags);
	atomic_store_explicit (&arena_->_growth_prev_size, new_reg->_capacity, memory_order_relaxed);

	/* Link into the list. */
	curr->_next = new_reg;
	atomic_store_explicit (&arena_->_current, new_reg, memory_order_relaxed);

	/* Bump allocate from the fresh struct region. */
	uintptr_t offset  = (uintptr_t)new_reg->_base;
	uintptr_t aligned = ark_math_align_up (offset, alignment_);
	sz		  padd	  = (sz)(aligned - offset);

	atomic_store_explicit (&new_reg->_used, padd + size_, memory_order_relaxed);
	u0* ptr = (u0*)aligned;
	ark_asan_unpoison_mem (ptr, size_);
	return ptr;
}


/*
 * Public API implementation
 */


__decl_overloadable__ ark_expect (ark_sync_arena_p) ark_make_sync_arena (sz cap_, flag_t flags_) {
	ark_sync_arena* arena = ark_talloc (arena);
	if (!arena) return ark_unexpected (MALLOC, "failed to allocated arena object");

	/* Ensure minimum _size. */
	if (cap_ == 0) cap_ = LC_ARENA_DEFAULT_REGION_SIZE;

	struct region* first = make_region (cap_, flags_);
	arena->_head		 = first;
	atomic_store_explicit (&arena->_current, first, memory_order_relaxed);
	arena->_flags			 = flags_;
	arena->_growth_prev_size = first->_capacity;

	if (pthread_mutex_init (&arena->_lock, nullptr) != 0) {
		region_destroy (first);
		ark_dealloc (arena);
		return ark_unexpected (MUTEX_INIT, "failed to initialize mutex lock");
	}

	ARK_INFO ("ark_sync_arena created with capacity of: %zu", (size_t)cap_);

	return ark_expected (arena);
}

u0 ark_destroy_sync_arena (ark_sync_arena* __restrict__ arena_) {
	if (!arena_) return;

	pthread_mutex_lock (&arena_->_lock);

	struct region* r = arena_->_head;
#if ARKLIB_DEBUG_BUILD
	sz used		= 0;
	sz capacity = 0;
	sz rcount	= 0;
#endif
	while (r) {
		struct region* _next = r->_next;
#if ARKLIB_DEBUG_BUILD
		used	 += atomic_load_explicit (&r->_used, memory_order_relaxed);
		capacity += r->_capacity;
		rcount++;
#endif
		region_destroy (r);
		r = _next;
	}

	pthread_mutex_unlock (&arena_->_lock);
	pthread_mutex_destroy (&arena_->_lock);

	ARK_INFO (
		"ark_sync_arena destroyed.\n\tregions created: %zu\n\ttotal memory "
		"allocated: %zu\n\ttotal memory used: %zu",
		(size_t)rcount,
		(size_t)capacity,
		(size_t)used
	);
	ark_dealloc (arena_);
}

/* Fast path: bump pointer if enough space in _current struct region. */
u0* ark_sync_arena_alloc (ark_sync_arena* __noalias__ arena_, sz size_, sz alignment_) {
	$assert_internal (size_ > 0 && "trying to allocate zero bytes");
	$assert_internal (ark_math_is_pow2 (alignment_) && "passed a non power of two alignment");

	SCOPED_MUTEX_OP (arena_);

	struct region* curr		 = atomic_load_explicit (&arena_->_current, memory_order_relaxed);
	sz			   curr_used = atomic_load_explicit (&curr->_used, memory_order_relaxed);
	uintptr_t	   offset	 = (uintptr_t)curr->_base + curr_used;
	uintptr_t	   aligned	 = ark_math_align_up (offset, alignment_);
	uintptr_t	   padd		 = (sz)(aligned - offset);
	uintptr_t	   new_used	 = curr_used + padd + size_;

	$likely (new_used <= curr->_capacity) {
		u0* ptr = (u0*)aligned;
		atomic_store_explicit (&curr->_used, new_used, memory_order_relaxed);
		ark_asan_unpoison_mem (ptr, size_);
		return ptr;
	}
	/* Slow path: need to add a new struct region. */

	u0* ptr = arena_alloc_slow (arena_, size_, alignment_);

	return ptr;
}

u0 ark_reset_sync_arena (ark_sync_arena* __noalias__ arena_) {
	SCOPED_MUTEX_OP (arena_);

	sz page_sz = page_size ();
	/* Walk all regions, reset `used` to 0 */
	for (struct region* r = arena_->_head; r != nullptr; r = r->_next) {
		sz curr_used = atomic_load_explicit (&r->_used, memory_order_relaxed);
		if ((arena_->_flags & ARK_SYNC_ARENA_ZERO_ON_RESET) && curr_used > 0) {
			/* Release physical memory, but keep the mapping. */
			uintptr_t start = ark_math_align_up ((uintptr_t)r->_base, page_sz);
			uintptr_t end	= ark_math_align_down ((uintptr_t)(r->_base + curr_used), page_sz);
			if (end > start) {
				madvise ((u0*)start, end - start, MADV_DONTNEED);
			}
		}
		atomic_store_explicit (&r->_used, 0, memory_order_relaxed);
		ark_asan_poison_mem (r->_base, r->_capacity);
	}
	atomic_store_explicit (&arena_->_current, arena_->_head, memory_order_relaxed);
}

ARK_END
