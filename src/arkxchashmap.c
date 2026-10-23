/*
 *
 * Copyright (c) 2026 Notrya A. ad' avlis. All Rights Reserved.
 *
 * External‑chaining hashmap
 *
 * Entries are obtained from a per‑map slab _allocator (256B slabs).
 * A free‑list recycles nodes removed by deletions.  The _allocator is
 * **not** thread‑safe – single‑threaded usage is assumed.
 * Load factor >= 0.6 triggers a resize (doubles _capacity).
 */

#include "algorithms/arkxxh64.h"
#include "collections/arkxchashmap.h"
#include "core/arkstring_view.h"

ARK_BEGIN

typedef $fn (u64, ark_hash_fn) (U0*, sz);
typedef $fn (bool, ark_compar_fn) (U0*, U0*);

static constexpr f64 XCHM_INIT_CAP			= 16;
static constexpr f64 XCHM_GROW_FACTOR		= 2;
static constexpr f64 XCHM_SHRINK_FACTOR		= 0.5;
static constexpr f64 XCHM_SHRINK_THRESHOULD = 0.25;
static constexpr f64 XCHM_LOAD_FACTOR		= 0.6;

ARK_CLASS (xcentry) {
	U0*		 _key;
	U0*		 _value;
	xcentry* _next;
};

struct xchashmap {
	sz				 _size;
	sz				 _capacity;
	ark_allocator	 _allocator;
	ark_hash_fn		 _hash;
	ark_compar_fn	 _compar;
	struct xcentry*	 _free_list;
	struct xcentry** _entries;
};

static uint64_t prng_state = 123456789ULL;

static inline uint64_t prng_u64 (void) {
	uint64_t x	= prng_state;
	x		   ^= x >> 12;
	x		   ^= x << 25;
	x		   ^= x >> 27;
	prng_state	= x;
	return x * 0x2545F4914F6CDD1DULL;
}

ARKINTERNAL __decl_inline__ bool _compar (U0* p1, U0* p2) {
	ark_string_view sv1 = ark_make_string_view (p1);
	ark_string_view sv2 = ark_make_string_view (p2);
	return ark_strcomp (sv1, sv2);
}

struct xchashmap* ark_make_xchasmap () {
	struct xchashmap* map = ark_allocator_alloc (
		ARK_LIBC_ALLOCATOR,
		sizeof (struct xchashmap),
		__alignof__ (struct xchashmap)
	);

	*map = (struct xchashmap) {
		._capacity	= XCHM_INIT_CAP,
		._allocator = ARK_LIBC_ALLOCATOR,
		._hash		= ark_xxh64_hash,
		._compar	= _compar,
	};
	return map;
}

void ark_destroy_xchashmap (ark_xchashmap* map) {
	ark_allocator_dealloc (map);
}

bool ark_xchashmap_insert (ark_xchashmap* map, U0* _key, U0* _value);

u0* ark_xchashmap_lookup (ark_xchashmap* map, U0* _key);

u0 ark_xchashmap_delete (ark_xchashmap* map, U0* _key);

u0* ark_xchashmap_at (ark_xchashmap* map, U0* _key);

ARK_END
