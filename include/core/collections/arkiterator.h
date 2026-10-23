#ifndef ARK_INTERFACES_ITERATOR_H_
#define ARK_INTERFACES_ITERATOR_H_

#import "internals/arkdefs.h"

ARK_BEGIN

#ifdef ARK_NAMESPACE
	#define iterator	   ark_iterator
	#define make_iterator  ark_make_iterator
	#define iterate		   ark_iterate
	#define iterator_next  ark_iterator_next
	#define iterator_all   ark_iterator_all
	#define iterator_any   ark_iterator_any
	#define iterator_chain ark_iterator_chain
	#define iterator_zip   ark_iterator_zip
	#define iterator_cycle ark_iterator_cycle
#endif

#define ark_iterator(Type_)                                                                                            \
	struct ark_pp_concat (ark_iterator_, Type_) {                                                                      \
		Type_* __nonnull__ begin;                                                                                      \
		Type_* __nonnull__ end;                                                                                        \
		Type_* __nonnull__ current;                                                                                    \
	}

typedef ark_iterator (u0) ark_iterator;

#define ark_make_iterator(...) ark_pp_concat (ark_make_iterator_, ark_pp_nargs (__VA_ARGS__)) (__VA_ARGS__)

#define ark_make_iterator_2(Type_, CArray_)                                                                            \
	({                                                                                                                 \
		$static_assert ($typematch (Type_, ((CArray_)[0])));                                                           \
		auto ___begin__					   = (CArray_);                                                                \
		auto ___end__					   = ((CArray_) + ARK_COUNTOF (CArray_));                                      \
		ark_iterator (Type_) ___iterator__ = {                                                                         \
			.begin	 = ___begin__,                                                                                     \
			.end	 = ___end__,                                                                                       \
			.current = ___begin__,                                                                                     \
		};                                                                                                             \
		___iterator__;                                                                                                 \
	})

#define ark_make_iterator_3(Type_, Begin_, End_)                                                                       \
	({                                                                                                                 \
		$static_assert ($typematch (Type_, (*(Begin_))));                                                              \
		(ark_iterator (Type_)) {                                                                                       \
			.begin	 = Begin_,                                                                                         \
			.end	 = End_,                                                                                           \
			.current = Begin_,                                                                                         \
		};                                                                                                             \
	})

#define ark_iterator_finished(It_) ((It_).current == (It_).end)

#define ark_iterate(X_, It_) for (auto(X_) = (It_).begin; X_ != (It_).end; ++(X_))

#define ark_iterator_next(It_)                                                                                         \
	({                                                                                                                 \
		((It_).current < (It_).end) ? ++(It_).current : ({                                                             \
			(It_).current = (It_).begin;                                                                               \
			(It_).begin;                                                                                               \
		});                                                                                                            \
	})


#define ark_iterator_all(It_, X_, Condition_)                                                                          \
	({                                                                                                                 \
		bool ___result__ = true;                                                                                       \
		ark_iterate ((X_), (It_)) {                                                                                    \
			if (!(Condition_)) ___result__ = false;                                                                    \
		}                                                                                                              \
		___result__;                                                                                                   \
	})

#define ark_iterator_any(It_, X_, Condition_)                                                                          \
	({                                                                                                                 \
		bool ___result__ = false;                                                                                      \
		ark_iterate ((X_), (It_)) {                                                                                    \
			if (Condition_) ___result__ = true;                                                                        \
		}                                                                                                              \
		___result__;                                                                                                   \
	})

#define ark_iterator_chain(...) ark_pp_concat (ark_iterator_chain_, ark_pp_nargs (__VA_ARGS__)) (__VA_ARGS__)

#define ark_iterator_chain_3(X_, It1_, It2_)                                                                           \
	$static_assert ($typematch ((It1_), (It2_)), "Iterators type do not match");                                       \
	for (int _chain_state_##X_ = 0; _chain_state_##X_ < 2; ++_chain_state_##X_)                                        \
		for (auto X_		= (_chain_state_##X_ == 0) ? (It1_).begin : (It2_).begin,                                  \
				  _end_##X_ = (_chain_state_##X_ == 0) ? (It1_).end : (It2_).end;                                      \
			 X_ != _end_##X_;                                                                                          \
			 ++X_)

#define ark_iterator_chain_4(X_, It1_, It2_, It3_)                                                                     \
	$static_assert ($typematch ((It1_), (It2_)) && $typematch ((It2_), (It3_)), "Iterators type do not match");        \
	for (int _chain_state_##X_ = 0; _chain_state_##X_ < 3; ++_chain_state_##X_)                                        \
		for (auto X_		= (_chain_state_##X_ == 0)	 ? (It1_).begin                                                \
							  : (_chain_state_##X_ == 1) ? (It2_).begin                                                \
														 : (It3_).begin,                                               \
				  _end_##X_ = (_chain_state_##X_ == 0)	 ? (It1_).end                                                  \
							  : (_chain_state_##X_ == 1) ? (It2_).end                                                  \
														 : (It3_).end;                                                 \
			 X_ != _end_##X_;                                                                                          \
			 ++X_)

#define ark_iterator_chain_5(X_, It1_, It2_, It3_, It4_)                                                               \
	$static_assert (                                                                                                   \
		$typematch ((It1_), (It2_)) && $typematch ((It2_), (It3_)) && $typematch ((It3_), (It4_)),                     \
		"Iterators type do not match"                                                                                  \
	);                                                                                                                 \
	for (int _chain_state_##X_ = 0; _chain_state_##X_ < 4; ++_chain_state_##X_)                                        \
		for (auto X_		= (_chain_state_##X_ == 0)	 ? (It1_).begin                                                \
							  : (_chain_state_##X_ == 1) ? (It2_).begin                                                \
							  : (_chain_state_##X_ == 2) ? (It3_).begin                                                \
														 : (It4_).begin,                                               \
				  _end_##X_ = (_chain_state_##X_ == 0)	 ? (It1_).end                                                  \
							  : (_chain_state_##X_ == 1) ? (It2_).end                                                  \
							  : (_chain_state_##X_ == 2) ? (It3_).end                                                  \
														 : (It4_).end;                                                 \
			 X_ != _end_##X_;                                                                                          \
			 ++X_)

#define ark_iterator_zip(...) ark_pp_concat (ark_iterator_zip_, ark_pp_nargs (__VA_ARGS__)) (__VA_ARGS__)

#define ark_iterator_zip_3(X_, It1_, It2_)                                                                             \
	for (                                                                                                              \
		struct {                                                                                                       \
			typeof ((It1_).begin) it1;                                                                                 \
			typeof ((It2_).begin) it2;                                                                                 \
		}(X_) = (typeof (X_)) {.it1 = (It1_).begin, .it2 = (It2_).begin};                                              \
		(X_).it1 != (It1_).end && (X_).it2 != (It2_).end;                                                              \
		++(X_).it1, ++(X_).it2)

#define ark_iterator_zip_4(X_, It1_, It2_, It3_)                                                                       \
	for (                                                                                                              \
		struct {                                                                                                       \
			typeof ((It1_).begin) it1;                                                                                 \
			typeof ((It2_).begin) it2;                                                                                 \
			typeof ((It3_).begin) it3;                                                                                 \
		}(X_) = (typeof (X_)) {.it1 = (It1_).begin, .it2 = (It2_).begin, .it3 = (It3_).begin};                         \
		(X_).it1 != (It1_).end && (X_).it2 != (It2_).end && (X_).it3 != (It3_).end;                                    \
		++(X_).it1, ++(X_).it2, ++(X_).it3)

#define ark_iterator_zip_5(X_, It1_, It2_, It3_, It4_)                                                                 \
	for (                                                                                                              \
		struct {                                                                                                       \
			typeof ((It1_).begin) it1;                                                                                 \
			typeof ((It2_).begin) it2;                                                                                 \
			typeof ((It3_).begin) it3;                                                                                 \
			typeof ((It4_).begin) it4;                                                                                 \
		}(X_) = (typeof (X_)) {.it1 = (It1_).begin, .it2 = (It2_).begin, .it3 = (It3_).begin, .it4 = (It4_).begin};    \
		(X_).it1 != (It1_).end && (X_).it2 != (It2_).end && (X_).it3 != (It3_).end && (X_).it4 != (It4_).end;          \
		++(X_).it1, ++(X_).it2, ++(X_).it3, ++(X_).it4)

/*
 * Cycle throuth elements for N iterations.
 */
#define ark_iterator_cycle(X_, It_, N_)                                                                                \
	typeof_unqual (N_) ark_iterator_counter_##X_##_##N_ = 0;                                                           \
	for (auto(X_) = (It_).begin; ark_iterator_counter_##X_##_##N_ < (N_);                                              \
		 (X_)	  = ark_iterator_next (It_), ++ark_iterator_counter_##X_##_##N_)

ARK_END

#endif /* ARK_INTERFACES_ITERATOR_H_ */
