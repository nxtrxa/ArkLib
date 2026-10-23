/**
 * @file hashmap_oa.h
 * @brief Open‑addressing hashmap (Robin‑Hood linear probing).
 */

#ifndef HASHMAP_OA_H
#define HASHMAP_OA_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/** Opaque handle for an open‑addressing hashmap. */
typedef struct HashMap HashMap;

/**
 * @brief Create a new open‑addressing map.
 * @param initial_capacity Starting number of slots (0 = sensible default, 16).
 * @param hash_fn       64‑bit hash function for keys.
 * @param eq_fn         Key equality predicate.
 * @param key_dtor      Optional destructor for keys (may be NULL).
 * @param val_dtor      Optional destructor for values (may be NULL).
 * @return New map, or NULL on memory error.
 */
HashMap *hashmap_create (
	size_t initial_capacity,
	uint64_t (*hash_fn) (const void *),
	bool (*eq_fn) (const void *, const void *),
	void (*key_dtor) (void *),
	void (*val_dtor) (void *));

/**
 * @brief Destroy the map, calling destructors on all remaining entries.
 */
void hashmap_destroy (HashMap *map);

/**
 * @brief Insert or update a key.  The map takes ownership of key and value.
 * @return true on success, false on memory error (map unchanged, caller retains
 * pointers).
 */
bool hashmap_put (HashMap *map, void *key, void *value);

/**
 * @brief Check whether a key exists.
 */
bool hashmap_contains (const HashMap *map, const void *key);

/**
 * @brief Retrieve the value for a key, or NULL if not found.
 */
void *hashmap_get (const HashMap *map, const void *key);

/**
 * @brief Remove a key.  Destructors are called if present.
 * @return true if the key existed.
 */
bool hashmap_remove (HashMap *map, const void *key);

/**
 * @brief Number of stored entries.
 */
size_t hashmap_size (const HashMap *map);

/**
 * @brief Pre‑allocate space for at least @p capacity entries without immediate
 * rehashing.
 * @return false on memory error.
 */
bool hashmap_reserve (HashMap *map, size_t capacity);

#endif /* HASHMAP_OA_H */
