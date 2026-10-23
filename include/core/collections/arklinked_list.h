#ifndef ARK_LLIST_H_
#define ARK_LLIST_H_

#include "interfaces/arkallocator_interface.h"

ARK_BEGIN

#define ark_llist_t(Type_)                                                                                             \
	struct ark_pp_concat (ark_llist_, Type_) {                                                                         \
		struct ark_pp_concat (ark_llist_node_, Type_) * head;                                                          \
		sz			  len;                                                                                             \
		ark_allocator allocator;                                                                                       \
	}

#define ark_llist_node_t(Type_)                                                                                        \
	struct ark_pp_concat (ark_llist_node_, Type_) {                                                                    \
		struct ark_pp_concat (ark_llist_node_, Type_) * next;                                                          \
		Type_ data;                                                                                                    \
	}

typedef ark_llist_node_t (u0_p) ark_llist_node;
typedef ark_llist_t (u0_p) ark_llist;

#define ark_llist(Type_)                                                                                               \
	ark_llist_node_t (Type_);                                                                                          \
	ark_llist_t (Type_)*

#define ark_llist_node(Type_) ark_llist_node_t (Type_)*

#define ark_llist_typeof(Llist_) typeof ((Llist_)->___node__)

#define ark_make_llist(...) ark_pp_concat (ark_make_llist_, ark_pp_nargs (__VA_ARGS__)) (__VA_ARGS__)

#define ark_make_llist_1(Type_)                                                                                        \
	({                                                                                                                 \
		ark_llist (Type_) ___llist__;                                                                                  \
		ark_allocator ___alloc__ = ARK_DEFAULT_ALLOCATOR;                                                              \
		___llist__ = ___alloc__.alloc (___alloc__.ctx, sizeof (*(___llist__)), __alignof__ (*(___llist__)));           \
		if (!___llist__) ARK_PANIC ("ark_make_llist: failed to allocate new lonked list");                             \
		___llist__->head	  = nullptr;                                                                               \
		___llist__->allocator = ___alloc__;                                                                            \
		___llist__->len		  = 0;                                                                                     \
		___llist__;                                                                                                    \
	})

#define ark_make_llist_2(Type_, Allocator_)                                                                            \
	({                                                                                                                 \
		ark_llist (Type_) ___llist__;                                                                                  \
		ark_allocator ___alloc__ = Allocator_;                                                                         \
		___llist__ = ___alloc__.alloc (___alloc__.ctx, sizeof (*(___llist__)), __alignof__ (*(___llist__)));           \
		if (!___llist__) ARK_PANIC ("ark_make_llist: failed to allocate new lonked list");                             \
		___llist__->head	  = nullptr;                                                                               \
		___llist__->allocator = ___alloc__;                                                                            \
		___llist__->len		  = 0;                                                                                     \
		___llist__;                                                                                                    \
	})

#define ark_destroy_llist(...) ark_pp_concat (ark_destroy_llist_, ark_pp_nargs (__VA_ARGS__)) (__VA_ARGS__)

#define ark_destroy_llist_1(Llist_)                                                                                    \
	do {                                                                                                               \
		if (Llist_ && (Llist_)->allocator.dealloc) {                                                                   \
			auto ___curr__ = (Llist_)->head;                                                                           \
			while ((___curr__ = (Llist_)->head)) {                                                                     \
				(Llist_)->head = (Llist_)->head->next;                                                                 \
				(Llist_)->allocator.dealloc (___curr__);                                                               \
			}                                                                                                          \
			(Llist_)->allocator.dealloc (Llist_);                                                                      \
		}                                                                                                              \
	} while (0)

#define ark_destroy_llist_2(Llist_, Node_Data_Destructor_)                                                             \
	do {                                                                                                               \
		if (Llist_ && (Llist_)->allocator.dealloc) {                                                                   \
			auto ___curr__ = (Llist_)->head;                                                                           \
			while ((___curr__ = (Llist_)->head)) {                                                                     \
				(Llist_)->head = (Llist_)->head->next;                                                                 \
				Node_Data_Destructor_ (&(___curr__->data));                                                            \
				(Llist_)->allocator.dealloc (___curr__);                                                               \
			}                                                                                                          \
			(Llist_)->allocator.dealloc (Llist_);                                                                      \
		}                                                                                                              \
	} while (0)

#define ark_destroy_llists(...)                                                                                        \
	do {                                                                                                               \
		void* const llists[] = {__VA_ARGS__};                                                                          \
		$ark_for (i, ark_pp_nargs (__VA_ARGS__)) ark_destroy_llist (llists[i])                                         \
	} while (0)

#define ark_llist_push_back(Llist_, Type_, Item_)                                                                      \
	do {                                                                                                               \
		auto ___node__ = ark_llist_make_node (Type_, (Llist_)->allocator, nullptr, Item_);                             \
		if ((Llist_)->len) {                                                                                           \
			auto last  = ark_llist_get_last (Llist_);                                                                  \
			last->next = ___node__;                                                                                    \
		} else {                                                                                                       \
			(Llist_)->head = ___node__;                                                                                \
		}                                                                                                              \
		(Llist_)->len++;                                                                                               \
	} while (0)

#define ark_llist_push_front(Llist_, Type_, Item_)                                                                     \
	do {                                                                                                               \
		auto ___node__ = ark_llist_make_node (Type_, (Llist_)->allocator, (Llist_)->head, Item_);                      \
		(Llist_)->head = ___node__;                                                                                    \
		(Llist_)->len++;                                                                                               \
	} while (0)

#define ark_llist_insert(Llist_, Type_, Idx_, Item_)                                                                   \
	do {                                                                                                               \
		if (Idx_ == 0) {                                                                                               \
			ark_llist_push_front (Llist_, Type_, Item_);                                                               \
			break;                                                                                                     \
		} else if (Idx_ == (Llist_)->len - 1) {                                                                        \
			ark_llist_push_back (Llist_, Type_, Item_);                                                                \
			break;                                                                                                     \
		}                                                                                                              \
		if (ARK_UNLIKELY (Idx_ < 0 || Idx_ >= (Llist_)->len)) {                                                        \
			ARK_WARN (                                                                                                 \
				"ark_llist_get_node: index out of bounds\n\tlinked list "                                              \
				"len: %zu\n\tindex: %zu",                                                                              \
				(size_t)(Llist_)->len,                                                                                 \
				(size_t)Idx_                                                                                           \
			);                                                                                                         \
			break;                                                                                                     \
		}                                                                                                              \
		auto ___node__ = ark_llist_make_node (Type_, (Llist_)->allocator, ark_llist_get_node (Llist_, Idx_), Item_);   \
		auto prev	   = ark_llist_get_node (Llist_, Idx_ - 1);                                                        \
		prev->next	   = ___node__;                                                                                    \
		(Llist_)->len++;                                                                                               \
	} while (0)

#define ark_llist_pop_back(Llist_)                                                                                     \
	do {                                                                                                               \
		if ((Llist_) && (Llist_)->len) {                                                                               \
			auto sc = ark_llist_get_node (Llist_, (Llist_)->len - 2);                                                  \
			if ((Llist_)->allocator.dealloc) {                                                                         \
				(Llist_)->allocator.dealloc (sc->next);                                                                \
			}                                                                                                          \
			sc->next = nullptr;                                                                                        \
			(Llist_)->len--;                                                                                           \
		}                                                                                                              \
	} while (0)

#define ark_llist_pop_front(Llist_)                                                                                    \
	do {                                                                                                               \
		if ((Llist_) && (Llist_)->len) {                                                                               \
			auto first	   = (Llist_)->head;                                                                           \
			(Llist_)->head = (Llist_)->head->next;                                                                     \
			if ((Llist_)->allocator.dealloc) {                                                                         \
				(Llist_)->allocator.dealloc (first);                                                                   \
			}                                                                                                          \
			(Llist_)->len--;                                                                                           \
		}                                                                                                              \
	} while (0)

#define ark_llist_delete(...) ark_pp_concat (ark_llist_delete_, ark_pp_nargs (__VA_ARGS__)) (__VA_ARGS__)

#define ark_llist_delete_1(Llist_)                                                                                     \
	do {                                                                                                               \
		if (Llist_ && (Llist_)->allocator.dealloc) {                                                                   \
			auto ___curr__ = (Llist_)->head;                                                                           \
			while ((___curr__ = (Llist_)->head)) {                                                                     \
				(Llist_)->head = (Llist_)->head->next;                                                                 \
				(Llist_)->allocator.dealloc (___curr__);                                                               \
			}                                                                                                          \
			(Llist_)->head = nullptr;                                                                                  \
			(Llist_)->len  = 0;                                                                                        \
		}                                                                                                              \
	} while (0)

#define ark_llist_delete_2(Llist_, Idx_)                                                                               \
	do {                                                                                                               \
		if ((Llist_) && (Llist_)->len) {                                                                               \
			if (Idx_ == 0) {                                                                                           \
				ark_llist_pop_front (Llist_);                                                                          \
				break;                                                                                                 \
			} else if (Idx_ == (Llist_)->len - 1) {                                                                    \
				ark_llist_pop_back (Llist_);                                                                           \
				break;                                                                                                 \
			}                                                                                                          \
			auto prev	 = ark_llist_get_node (Llist_, Idx_ - 1);                                                      \
			auto deleted = prev->next;                                                                                 \
			prev->next	 = deleted->next;                                                                              \
			if ((Llist_)->allocator.dealloc) {                                                                         \
				(Llist_)->allocator.dealloc (deleted);                                                                 \
			}                                                                                                          \
			(Llist_)->len--;                                                                                           \
		}                                                                                                              \
	} while (0)

#define ark_llist_get_last(Llist_) ark_llist_get_node (Llist_, (Llist_)->len - 1)

#define ark_llist_get(Llist_, Idx_)                                                                                    \
	({                                                                                                                 \
		auto ___node__ = ark_llist_get_node (Llist_, Idx_);                                                            \
		___node__->data;                                                                                               \
	})

#define ark_llist_get_node(Llist_, Idx_)                                                                               \
	({                                                                                                                 \
		sz ___index__ = Idx_;                                                                                          \
		if (___index__ < 0 || ___index__ >= (Llist_)->len) {                                                           \
			ARK_WARN (                                                                                                 \
				"ark_llist_get_node: index out of bounds\n\tlinked list "                                              \
				"len: %zu\n\tindex: %zu",                                                                              \
				(size_t)(Llist_)->len,                                                                                 \
				(size_t)___index__                                                                                     \
			);                                                                                                         \
			___index__ = (Llist_)->len - 1;                                                                            \
		}                                                                                                              \
		auto ___curr__ = (Llist_)->head;                                                                               \
		for (sz i = 1; i <= ___index__ && ___curr__; ++i) {                                                            \
			___curr__ = ___curr__->next;                                                                               \
		}                                                                                                              \
		___curr__;                                                                                                     \
	})

#define ark_llist_make_node(Type_, Allocator_, Next_, ...)                                                             \
	({                                                                                                                 \
		ark_llist_node (Type_)                                                                                         \
			___node__ = Allocator_.alloc (Allocator_.ctx, sizeof (*(___node__)), __alignof__ (*(___node__)));          \
		if (!___node__) ARK_PANIC ("ark_list_make_node: failed to allocate new ___node__");                            \
		___node__->data = __VA_ARGS__;                                                                                 \
		___node__->next = Next_;                                                                                       \
		___node__;                                                                                                     \
	})

#endif /* ARK_LLIST_H_ */
