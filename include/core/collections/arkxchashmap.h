#ifndef ARK_XCHASHMAP_H_
#define ARK_XCHASHMAP_H_

#include "allocators/arklibc_allocator.h"

typedef struct xchashmap ark_xchashmap;

ark_xchashmap* ark_make_xchasmap ();

void ark_destroy_xchashmap (ark_xchashmap* map);

bool ark_xchashmap_insert (ark_xchashmap* map, U0* key, U0* value);

u0* ark_xchashmap_lookup (ark_xchashmap* map, U0* key);

u0 ark_xchashmap_delete (ark_xchashmap* map, U0* key);

u0* ark_xchashmap_at (ark_xchashmap* map, U0* key);

#endif /* ARK_XCHASHMAP_H_ */
