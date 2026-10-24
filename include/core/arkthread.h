/*
 *
 *
 *
 *
 *
 *
 *
 */
#ifndef ARKLIB_THREAD_H_
#define ARKLIB_THREAD_H_

#import <pthread.h>

#import "core/arkexpect.h"
#import "internals/arkdefs.h"

ARK_BEGIN

#define ARK_EXPECT_CONTEXT ARK_THREAD

#ifdef ARK_NAMESPACE
	#define class_thread		  ark_class_thread
	#define class_thread_unsafe	  ark_class_thread_unsafe
	#define thread_worker		  ark_thread_function
	#define thread_main_loop	  ark_thread_main_loop
	#define thread				  ark_thread
	#define stop_token			  ark_stop_token
	#define thread_fn_t			  ark_thread_fn_t
	#define make_thread			  ark_make_thread
	#define destroy_thread		  ark_destroy_thread
	#define thread_request_stop	  ark_thread_request_stop
	#define thread_stop_requested ark_thread_stop_requested
	#define thread_join			  ark_thread_join
#endif

#define ark_class_thread(FaultHandler_, Thread_, ...)                                                                  \
	__attr_cleanup__ (ark_cleanup_thread)                                                                              \
		ark_thread* Thread_ = ark_catch (ark_make_thread (__VA_ARGS__), __ark_fault_, FaultHandler_ (__ark_fault_))

#define ark_class_thread_unsafe(Thread_, ...)                                                                          \
	__attr_cleanup__ (ark_cleanup_thread) ark_thread* Thread_ = ark_make_thread (__VA_ARGS__).unwrap

#define ark_destroy_thread(...) ark_pp_foreach (__ark_destroy_thread_, __VA_ARGS__)

#define ark_thread_function(FnName_, ...) u0* FnName_ (__decl_unused__ ark_stop_token tk_, u0* arg_)

#define ark_thread_main_loop(...) while (!ark_thread_stop_requested (tk_) __VA_ARGS__)

#if ARKLIB_IS_LINUX
	#define ARK_THREAD_ONCE_INIT PTHREAD_ONCE_INIT
#endif

ARK_CLASS (ark_thread);

typedef pthread_mutex_t ark_mutex_t __attr_capability__ ("mutex");

typedef pthread_once_t ark_thread_once_t;

typedef $fn (u0, ark_thread_once_fn_t) (u0);

/*
 * Shared state view.
 */
ARK_CLASS (ark_stop_token) {
	ark_atomic_t (bool) * _stop_req;
	u0* _stop_state;
};

ark_faultdefs (OUT_OF_MEMORY, PTHREAD_CREATE);

/*
 * Thread function signature.
 */
typedef $fn (u0*, ark_thread_fn_t) (ark_stop_token tk_, u0* arg_);

/*
 * Create thread object and executes `FN`
 *
 * @param fn_ <required>  : function to be executed by the new thread
 * @param arg_ <required> : opaque pointer passed to `FN`
 *
 * @return expected: opaque pointer to a ARK_THREAD object
 *         unexpected: integer error code
 */
ARKAPI ark_expect (ark_thread*) ark_make_thread (ark_thread_fn_t __nonnull__ fn_, u0* arg_) __attr_noexcept__;

/*
 * Request the thread to stop (non-blocking).
 */
ARKAPI u0 ark_thread_request_stop (ark_thread* thr_) __attr_noexcept__;

/*
 * Check is stop was requested (for use inside thread function).
 */
ARKAPI bool ark_thread_stop_requested (ark_stop_token tk_) __attr_noexcept__;

/*
 * Join thread (block untiol it finishes).
 */
ARKAPI u0 ark_thread_join (ark_thread* thr_);

/*
 * Check if thread is joinable.
 */
ARKAPI bool ark_thread_joinable (ark_thread* thr_) __attr_noexcept__;

/*
 *
 */
ARKAPI u0 ark_thread_call_once (ark_thread_once_fn_t fn_, ark_thread_once_t* flag_) __attr_noexcept__;

/*
 * Destroy thread (joins is still joinable).
 */
ARKAPI u0 ark_destroy_thread_fn (ark_thread* thr_) __attr_noexcept__;

__decl_static_inline__ u0 ark_cleanup_thread (ark_thread** thr_) __attr_noexcept__ {
	ark_destroy_thread_fn (*thr_);
}

#define __ark_destroy_thread_(Thread_) ark_destroy_thread_fn (Thread_);

#undef ARK_EXPECT_CONTEXT

ARK_END

#endif /* ARKLIB_THREAD_H_ */
