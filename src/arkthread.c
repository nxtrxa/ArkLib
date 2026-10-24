#ifdef __linux__
	#import <unistd.h>
#elifdef __WIN32__
	#import <windows.h>
#endif

#import "core/arkthread.h"
#import "interfaces/arkallocator_interface.h"

ARK_BEGIN

#define ARK_EXPECT_CONTEXT ARK_THREAD

struct class_ark_thread {
	pthread_t _handler;
	ark_atomic_t (bool) _stop_req;
	bool _joined;
};

/*
 * Adapter to our own signature
 */
struct trampoline_arg {
	ark_thread_fn_t _fn;
	ark_thread*		_thr;
	u0*				_arg;
};

ARKINTERNAL u0* ark_thread_trampoline (u0* raw_) {
	struct trampoline_arg* ta = raw_;
	ark_stop_token		   tk = {
		._stop_req	 = &ta->_thr->_stop_req,
		._stop_state = nullptr,
	};
	ta->_fn (tk, ta->_arg);
	free (ta);
	return nullptr;
}

ARKEXPORT ark_expect (ark_thread*) ark_make_thread (ark_thread_fn_t __nonnull__ fn_, u0* arg_) {
	ark_thread* thr = ark_talloc (thr);
	if (!thr) return ark_unexpected (OUT_OF_MEMORY);

	atomic_init (&thr->_stop_req, false);
	thr->_joined = false;

	struct trampoline_arg* ta = ark_talloc (ta);
	if (!ta) {
		ark_dealloc (thr);
		return ark_unexpected (OUT_OF_MEMORY);
	}

	ta->_fn	 = fn_;
	ta->_arg = arg_;
	ta->_thr = thr;

	if (pthread_create (&thr->_handler, nullptr, ark_thread_trampoline, ta) != 0) {
		ark_dealloc (thr, ta);
		return ark_unexpected (PTHREAD_CREATE);
	}

	return ark_expected (thr);
}

ARKEXPORT u0 ark_destroy_thread_fn (ark_thread* thr_) {
	if (!thr_) return;
	ark_thread_request_stop (thr_);
	ark_thread_join (thr_);
	ark_dealloc (thr_);
}

ARKEXPORT u0 ark_thread_request_stop (ark_thread* thr_) {
	if (!thr_) return;
	atomic_store (&thr_->_stop_req, true);
}

ARKEXPORT bool ark_thread_stop_requested (ark_stop_token tk_) {
	return atomic_load (tk_._stop_req);
}

ARKEXPORT u0 ark_thread_join (ark_thread* thr_) {
	if (!thr_ || thr_->_joined) return;
	pthread_join (thr_->_handler, nullptr);
	thr_->_joined = true;
}

ARKEXPORT bool ark_thread_joinable (ark_thread* thr_) {
	return thr_->_joined;
}

ARKEXPORT u0 ark_thread_call_once (ark_thread_once_fn_t fn_, ark_thread_once_t* flag_) {
	if (!flag_ || !fn_) return;
	pthread_once (flag_, fn_);
}

ARKEXPORT u64 ark_thread_hardware_nothreads (u0) {
#if ARKLIB_HAS_INCLUDE(<unistd.h>)
	u64 cores = 0;
	cores	  = (u64)sysconf (_SC_NPROCESSORS_ONLN);
	return cores;
#elif ARKLIB_HAS_INCLUDE(<windows.h>)
	SYSTEM_INFO sysinfo;
	GetSystemInfo (&sysinfo);
	DWORD cores = sysinfo.dwNumberOfProcessors;
	return cores;
#else
	#error "Only linux is supported and windows compatibilty is on progress"
#endif
}

ARK_END
