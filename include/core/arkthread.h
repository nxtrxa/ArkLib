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

#ifdef ARK_NAMESPACE
	#define class_thread		  ark_class_thread
	#define class_thread_unsafe	  ark_class_thread_unsafe
	#define thread_worker		  ark_thread_worker
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

typedef pthread_mutex_t ark_mutex_t __attr_capability__ ("mutex");

#define ark_class_thread(Exception_Handler_, Thread_, ...)                                                             \
	__attr_cleanup__ (ark_cleanup_thread)                                                                              \
		ark_thread* Thread_ = ark_catch (ark_make_thread (__VA_ARGS__), ___except__, Exception_Handler_ (___except__))

#define ark_class_thread_unsafe(Thread_, ...)                                                                          \
	__attr_cleanup__ (ark_cleanup_thread) ark_thread* Thread_ = ark_make_thread (__VA_ARGS__).unwrap

#define ark_destroy_thread(...) ark_pp_foreach (___ARK__DESTROY_THREAD__, __VA_ARGS__)

#define ark_thread_worker(FnName_, ...) u0* FnName_ (__decl_unused__ ark_stop_token tk_, u0* arg_)
#define ark_thread_main_loop(...)		while (!ark_thread_stop_requested (tk_) __VA_ARGS__)

#define ARK_THREAD_ONCE_INIT PTHREAD_ONCE_INIT

ARK_CLASS (ark_thread);

ARK_DECL_FAULTS (ARK_THREAD, MALLOC, PTHREAD_CREATE);

typedef pthread_once_t ark_thread_once_t;

typedef $fn (u0, ark_thread_once_fn_t) (u0);


/*
 * Shared state view.
 */
ARK_CLASS (ark_stop_token) {
	ark_atomic_t (bool) * _stop_req;
	u0* _stop_state;
};

/*
 * Thread function signature.
 */
typedef $fn (u0*, ark_thread_fn_t) (ark_stop_token tk_, u0* arg_);

/*
 *
 */
ARKAPI ark_expect (ARK_THREAD, ark_thread_p) ark_make_thread (ark_thread_fn_t fn_, u0* arg_) __attr_noexcept__;

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

#define ___ARK__DESTROY_THREAD__(Thread_) ark_destroy_thread_fn (Thread_);

ARK_END

#endif /* ARKLIB_THREAD_H_ */
