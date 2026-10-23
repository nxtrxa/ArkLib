/*
 * @Copyright (c) 2026 Murilo Gonzalez. All Rights Reserved.
 *
 *                      ARKLIB WIDE STRING STREAM
 */

#ifndef ARK_WSTRING_STREAM_H_
#define ARK_WSTRING_STREAM_H_

#import "internals/arkdefs.h"

ARK_BEGIN

#ifdef ARK_NAMESPACE
	#define wstring_stream		 ark_wstring_stream
	#define class_wstring_stream ark_class_wstring_stream
	#define make_wstring_stream	 ark_make_wstring_stream
#endif

ARK_INTERFACE (ark_allocator);

ARK_CLASS (ark_wstring_stream) {
	sz			   len;
	sz			   capacity;
	sz			   cursor;
	ark_allocator* allocator;
	wchar_t		   data[] __attr_counted_by__ (len);
};

#define ark_class_wstring_stream(Ss_, ...)                                                                             \
	__attr_cleanup__ (ark_cleanup_wstring_stream) ark_wstring_stream ss = ark_make_wstring_stream (__VA_ARGS__)

ARKAPI __decl_overloadable__ ark_wstring_stream
ark_make_wstring_stream (ark_allocator* allocator_, U0* buff_, sz buff_sz_, sz capacity);

ARKAPI __decl_overloadable__ ark_wstring_stream
ark_make_wstring_stream (ark_allocator* allocator_, U0* buff_, sz buff_sz_);

ARKAPI __decl_overloadable__ ark_wstring_stream ark_make_wstring_stream (U0* buff_, sz buff_sz_);

ARKAPI __decl_overloadable__ ark_wstring_stream ark_make_wstring_stream ();

ARKAPI u0 ark_reset_wstring_stream (ark_wstring_stream* wss_);

ARKAPI u0 ark_destroy_wstring_stream (ark_wstring_stream* wss_);

__decl_extern_inline__ u0 ark_cleanup_wstring_stream (ark_wstring_stream** __noalias__ wss_) __attr_diagnose_if__ (
	(wss_ == nullptr || *wss_ == nullptr), "wide string string is null", "error"
) {
	ark_destroy_wstring_stream (*wss_);
}

ARK_END

#endif /* ARK_WSTRING_STREAM_H_ */
