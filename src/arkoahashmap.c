/**
 * @file hashmap_oa.c
 * @brief Open‑addressing hashmap (Robin‑Hood linear probing).
 *
 * Implementation uses:
 *  - Three parallel arrays: hashes (64‑bit), keys, values.
 *  - A uint8_t info array encoding displacement (0 = empty, 1‑255 = distance).
 *  - Capacity is always a power of two; indexing via bitmask.
 *  - Resize at 75 % load factor.
 *  - Backward‑shift deletion (no tombstones).
 */

#include <stdlib.h>
#include <string.h> // memset (used sparingly)

#include "collections/arkoahashmap.h"

/* ------------------------------------------------------------------ */
/*  Internal structure                                                 */
/* ------------------------------------------------------------------ */
struct HashMap {
	size_t	  size;		/* Number of stored entries            */
	size_t	  capacity; /* Number of slots (power of two)      */
	uint64_t* hashes;	/* Hash value for each slot            */
	void**	  keys;
	void**	  values;
	uint8_t*  info; /* 0 = empty, 1..255 = distance        */

	uint64_t (*hash_fn) (const void*);
	bool (*eq_fn) (const void*, const void*);
	void (*key_dtor) (void*);
	void (*val_dtor) (void*);
};

/* ------------------------------------------------------------------ */
/*  Helpers                                                            */
/* ------------------------------------------------------------------ */

/** Round up to the next power of two (0 → 1). */
static inline size_t next_pow2 (size_t n) {
	if (n == 0) return 1;
	n--;
	n |= n >> 1;
	n |= n >> 2;
	n |= n >> 4;
	n |= n >> 8;
	n |= n >> 16;
	n |= n >> 32;
	return n + 1;
}

/** Resize internal tables to @p new_cap (must be > current capacity). */
static bool resize (HashMap* map, size_t new_cap) {
	size_t mask = new_cap - 1;

	uint64_t* new_hashes = malloc (new_cap * sizeof (uint64_t));
	void**	  new_keys	 = malloc (new_cap * sizeof (void*));
	void**	  new_vals	 = malloc (new_cap * sizeof (void*));
	uint8_t*  new_info	 = calloc (new_cap, sizeof (uint8_t));

	if (!new_hashes || !new_keys || !new_vals || !new_info) {
		free (new_hashes);
		free (new_keys);
		free (new_vals);
		free (new_info);
		return false;
	}

	/* Reinsert all existing entries into the new table.               */
	for (size_t i = 0; i < map->capacity; i++) {
		if (map->info[i] == 0) continue;

		uint64_t hash	  = map->hashes[i];
		void*	 key	  = map->keys[i];
		void*	 value	  = map->values[i];
		size_t	 ideal	  = hash & mask;
		size_t	 idx	  = ideal;
		unsigned distance = 1;

		while (1) {
			if (new_info[idx] == 0) {
				new_hashes[idx] = hash;
				new_keys[idx]	= key;
				new_vals[idx]	= value;
				new_info[idx]	= (uint8_t)(distance < 255 ? distance : 255);
				break;
			}
			if (distance > (unsigned)new_info[idx]) {
				/* Robin‑Hood swap – the richer entry moves forward. */
				uint64_t tmp_hash = new_hashes[idx];
				void*	 tmp_key  = new_keys[idx];
				void*	 tmp_val  = new_vals[idx];
				unsigned tmp_dist = new_info[idx];

				new_hashes[idx] = hash;
				new_keys[idx]	= key;
				new_vals[idx]	= value;
				new_info[idx]	= (uint8_t)(distance < 255 ? distance : 255);

				hash	 = tmp_hash;
				key		 = tmp_key;
				value	 = tmp_val;
				distance = tmp_dist;
			}
			idx = (idx + 1) & mask;
			distance++;
		}
	}

	/* Replace old arrays. */
	free (map->hashes);
	free (map->keys);
	free (map->values);
	free (map->info);

	map->hashes	  = new_hashes;
	map->keys	  = new_keys;
	map->values	  = new_vals;
	map->info	  = new_info;
	map->capacity = new_cap;
	return true;
}

/* ------------------------------------------------------------------ */
/*  Public API                                                         */
/* ------------------------------------------------------------------ */

HashMap* hashmap_create (
	size_t initial_capacity,
	uint64_t (*hash_fn) (const void*),
	bool (*eq_fn) (const void*, const void*),
	void (*key_dtor) (void*),
	void (*val_dtor) (void*)) {
	if (initial_capacity == 0) initial_capacity = 16;
	size_t cap = next_pow2 (initial_capacity);

	HashMap* map = malloc (sizeof (HashMap));
	if (!map) return NULL;

	map->hashes = malloc (cap * sizeof (uint64_t));
	map->keys	= malloc (cap * sizeof (void*));
	map->values = malloc (cap * sizeof (void*));
	map->info	= calloc (cap, sizeof (uint8_t));

	if (!map->hashes || !map->keys || !map->values || !map->info) {
		free (map->hashes);
		free (map->keys);
		free (map->values);
		free (map->info);
		free (map);
		return NULL;
	}

	map->size	  = 0;
	map->capacity = cap;
	map->hash_fn  = hash_fn;
	map->eq_fn	  = eq_fn;
	map->key_dtor = key_dtor;
	map->val_dtor = val_dtor;
	return map;
}

void hashmap_destroy (HashMap* map) {
	if (!map) return;
	for (size_t i = 0; i < map->capacity; i++) {
		if (map->info[i] != 0) {
			if (map->key_dtor) map->key_dtor (map->keys[i]);
			if (map->val_dtor) map->val_dtor (map->values[i]);
		}
	}
	free (map->hashes);
	free (map->keys);
	free (map->values);
	free (map->info);
	free (map);
}

bool hashmap_put (HashMap* map, void* key, void* value) {
	/* Check load factor: 75 % threshold. */
	if ((map->size + 1) * 100 > map->capacity * 75) {
		size_t new_cap = map->capacity ? map->capacity * 2 : 16;
		if (!resize (map, new_cap)) return false;
	}

	size_t	 mask	  = map->capacity - 1;
	uint64_t hash	  = map->hash_fn (key);
	size_t	 ideal	  = hash & mask;
	size_t	 idx	  = ideal;
	unsigned distance = 1;

	void*	 cur_key  = key;
	void*	 cur_val  = value;
	uint64_t cur_hash = hash;

	while (1) {
		if (map->info[idx] == 0) {
			/* Empty slot – place entry. */
			map->hashes[idx] = cur_hash;
			map->keys[idx]	 = cur_key;
			map->values[idx] = cur_val;
			map->info[idx]	 = (uint8_t)(distance < 255 ? distance : 255);
			map->size++;
			return true;
		}

		/* Check for duplicate key. */
		if (map->hashes[idx] == cur_hash && map->eq_fn (map->keys[idx], cur_key)) {
			/* Replace value, destroy old key/value. */
			if (map->key_dtor) map->key_dtor (map->keys[idx]);
			if (map->val_dtor) map->val_dtor (map->values[idx]);
			map->keys[idx]	 = cur_key;
			map->values[idx] = cur_val;
			return true;
		}

		unsigned cur_dist = map->info[idx];
		if (cur_dist < distance) {
			/* Robin‑Hood: swap with richer entry. */
			uint64_t tmp_hash = map->hashes[idx];
			void*	 tmp_key  = map->keys[idx];
			void*	 tmp_val  = map->values[idx];

			map->hashes[idx] = cur_hash;
			map->keys[idx]	 = cur_key;
			map->values[idx] = cur_val;
			map->info[idx]	 = (uint8_t)(distance < 255 ? distance : 255);

			cur_hash = tmp_hash;
			cur_key	 = tmp_key;
			cur_val	 = tmp_val;
			distance = cur_dist;
		}

		idx = (idx + 1) & mask;
		distance++;
	}
}

bool hashmap_contains (const HashMap* map, const void* key) {
	if (map->size == 0) return false;
	size_t	 mask = map->capacity - 1;
	uint64_t hash = map->hash_fn (key);
	size_t	 idx  = hash & mask;

	while (map->info[idx] != 0) {
		if (map->hashes[idx] == hash && map->eq_fn (map->keys[idx], key)) return true;
		idx = (idx + 1) & mask;
	}
	return false;
}

void* hashmap_get (const HashMap* map, const void* key) {
	if (map->size == 0) return NULL;
	size_t	 mask = map->capacity - 1;
	uint64_t hash = map->hash_fn (key);
	size_t	 idx  = hash & mask;

	while (map->info[idx] != 0) {
		if (map->hashes[idx] == hash && map->eq_fn (map->keys[idx], key)) return map->values[idx];
		idx = (idx + 1) & mask;
	}
	return NULL;
}

bool hashmap_remove (HashMap* map, const void* key) {
	if (map->size == 0) return false;
	size_t	 cap  = map->capacity;
	size_t	 mask = cap - 1;
	uint64_t hash = map->hash_fn (key);
	size_t	 idx  = hash & mask;

	while (map->info[idx] != 0) {
		if (map->hashes[idx] == hash && map->eq_fn (map->keys[idx], key)) {
			/* Found – destroy and back‑shift. */
			if (map->key_dtor) map->key_dtor (map->keys[idx]);
			if (map->val_dtor) map->val_dtor (map->values[idx]);
			map->info[idx] = 0;
			map->size--;

			/* Backward‑shift deletion: fill the hole. */
			size_t hole = idx;
			size_t next = (hole + 1) & mask;
			while (map->info[next] != 0) {
				unsigned dist = map->info[next];
				if (dist == 1) break; /* at its ideal slot, cannot move */
				/* Move entry from `next` to `hole` and decrease its distance.
				 */
				map->hashes[hole] = map->hashes[next];
				map->keys[hole]	  = map->keys[next];
				map->values[hole] = map->values[next];
				map->info[hole]	  = (uint8_t)(dist - 1);
				map->info[next]	  = 0;

				hole = next;
				next = (hole + 1) & mask;
			}
			return true;
		}
		idx = (idx + 1) & mask;
	}
	return false;
}

size_t hashmap_size (const HashMap* map) {
	return map->size;
}

bool hashmap_reserve (HashMap* map, size_t capacity) {
	if (map->capacity >= capacity) return true;
	size_t new_cap = next_pow2 (capacity);
	if (new_cap < 16) new_cap = 16;
	return resize (map, new_cap);
}
