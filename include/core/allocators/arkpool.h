#ifndef ARK_POOL_H_
#define ARK_POOL_H_

#include "internals/arkdefs.h"

ARK_BEGIN

#ifdef ARK_NAMESPACE
	#define MAKE_POOL(Type_, Cap_)                                                                                     \
		ARK_MAKE_POOL (Type_, Cap_)                                                                                    \
		ARK_POOL_IMPLEMETATION_NO_PREFIX (Type_, Cap_)
#endif

#define ARK_MAKE_POOL(Type_, Cap_)                                                                                     \
	$static_assert ((Cap_) > 0 && "Pool Capacity must be greater than zero.");                                         \
	ARK_POOL_INTERFACE (Type_, Cap_)                                                                                   \
	ARK_POOL_IMPLEMETATION (Type_, Cap_)

#define ARK_POOL_INTERFACE(Type_, Cap_) ARK_OBJECT_POOL (Type_, Cap_)

#define ARK_POOL_IMPLEMETATION(Type_, Cap_)                                                                            \
	ARK_INIT_POOL (Type_, Cap_)                                                                                        \
	ARK_POOL_GET (Type_, Cap_)                                                                                         \
	ARK_POOL_RETURN (Type_, Cap_)

#define ARK_POOL_IMPLEMETATION_NO_PREFIX(Type_, Cap_)                                                                  \
	ARK_POOL_GET_NO_PREFIX (Type_, Cap_)                                                                               \
	ARK_POOL_RETURN_NO_PREFIX (Type_, Cap_)

#define ARK_OBJECT_POOL(Type_, Cap_)                                                                                   \
	typedef struct ark_pp_concat (ark_, Type_, _pool_t) {                                                              \
		Type_ obj;                                                                                                     \
		struct ark_pp_concat (ark_, Type_, _pool_t) * next;                                                            \
	} ark_pp_concat (ark_, Type_, _pool);                                                                              \
	static ark_pp_concat (ark_, Type_, _pool) ark_pp_concat (Type_, _pool)[Cap_]	  = {{}};                          \
	static ark_pp_concat (ark_, Type_, _pool) * ark_pp_concat (Type_, _pool_freelist) = nullptr;

#define ARK_INIT_POOL(Type_, Cap_)                                                                                     \
	__decl_constructor__ () static u0 ark_init_pool_##Type_ (u0) {                                                     \
		$for (i, Cap_ - 1) {                                                                                           \
			Type_##_pool[i].next = &(Type_##_pool[i + 1]);                                                             \
		}                                                                                                              \
		Type_##_pool_freelist		= Type_##_pool;                                                                    \
		Type_##_pool[Cap_ - 1].next = nullptr;                                                                         \
	}

#define ARK_POOL_GET(Type_, Cap_)                                                                                      \
	Type_* ark_pool_get_##Type_ (u0) {                                                                                 \
		if (Type_##_pool_freelist) {                                                                                   \
			ark_##Type_##_pool* op = Type_##_pool_freelist;                                                            \
			Type_##_pool_freelist  = Type_##_pool_freelist->next;                                                      \
			return &(op->obj);                                                                                         \
		}                                                                                                              \
		return nullptr;                                                                                                \
	}

#define ARK_POOL_RETURN(Type_, Cap_)                                                                                   \
	u0 ark_pool_return_##Type_ (Type_* __noalias__ obj) {                                                              \
		ptrdiff_t pos = ((uintptr_t)obj - (uintptr_t)Type_##_pool) / sizeof (ark_##Type_##_pool);                      \
		$assert (&(Type_##_pool[pos].obj) == obj);                                                                     \
		ark_##Type_##_pool* op = &(Type_##_pool[pos]);                                                                 \
		op->next			   = Type_##_pool_freelist;                                                                \
		Type_##_pool_freelist  = op;                                                                                   \
	}

#define ARK_POOL_GET_NO_PREFIX(Type_, Cap_) __attr_alias__ ("ark_pool_get_" #Type_) Type_* pool_get_##Type_ (u0);

#define ARK_POOL_RETURN_NO_PREFIX(Type_, Cap_)                                                                         \
	__attr_alias__ ("ark_pool_return_" #Type_) u0 pool_return_##Type_ (Type_*);

ARK_END

#endif /* ARK_POOL_H_ */
