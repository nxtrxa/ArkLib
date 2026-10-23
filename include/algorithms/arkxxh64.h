/*
 * @Copyright (c) 2026 Murilo Gonzalez. All Rights Reserved.
 *
 *                      ARKLIB XXH64
 *
 * The core algorithm is XXH64 (xxHash 64‑bit), a battle‑tested
 * non‑cryptographic hash with excellent distribution, strong avalanche effect,
 * and high performance on both small keys and large buffers.
 *
 * Pure C (C2y compatible), no external dependencies.  Endian‑neutral reading
 * uses byte‑by‑byte LE assembly. The seeded variant allows different hash
 * values for the same key, mitigating collision‑based denial‑of‑service attacks
 * in hash maps – simply use a per‑table random seed.
 *
 * Reference: https://github.com/Cyan4973/xxHash
 */

#ifndef ARK_ALGORITHMS_XXH64_H_
#define ARK_ALGORITHMS_XXH64_H_

#include "internals/arkdefs.h"

ARK_BEGIN

/*
 * Seeded hash – for hash maps, prefer a random seed to avoid predictable
 * collisions.  Identical (key, length, seed) triples always produce the
 * same result.
 */
ARKAPI __decl_nodiscard__ __decl_overloadable__ u64 ark_xxh64_hash (U0* key, sz key_len, u64 seed);

/*
 * Unseeded hash – convenient when a fixed, reproducible mapping is desired.
 * Internally calls hash_with_seed() with seed = 0.
 */
ARKAPI __decl_nodiscard__ __decl_overloadable__ u64 ark_xxh64_hash (U0* key, sz key_len);

ARK_END

#endif /* ARK_ALGORITHMS_XXH64_H_ */
