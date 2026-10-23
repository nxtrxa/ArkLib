/*
 * @Copyright (c) 2026 Murilo Gonzalez. All Rights Reserved.
 *
 *                      ARKLIB STRING STREAM
 */

#ifndef ARK_STRING_STREAM_H_
#define ARK_STRING_STREAM_H_

#import "internals/arkdefs.h"

ARK_BEGIN

#ifdef ARK_NAMESPACE
	#define string_stream		ark_string_stream
	#define class_string_stream ark_class_string_stream
	#define make_string_stream	ark_make_string_stream
#endif

ARK_INTERFACE (ark_allocator);

ARK_CLASS (ark_string_stream) {
	sz			   len;
	sz			   capacity;
	sz			   cursor;
	ark_allocator* allocator;
	char		   data[] __attr_counted_by__ (len);
};

#define ark_class_string_stream(Ss_, ...)                                                                              \
	__attr_cleanup__ (ark_cleanup_string_stream) ark_string_stream ss = ark_make_string_stream (__VA_ARGS__)

ARKAPI __decl_overloadable__ ark_string_stream
ark_make_string_stream (ark_allocator* allocator_, U0* buff_, sz buff_sz_, sz capacity);

ARKAPI __decl_overloadable__ ark_string_stream
ark_make_string_stream (ark_allocator* allocator_, U0* buff_, sz buff_sz_);

ARKAPI __decl_overloadable__ ark_string_stream ark_make_string_stream (U0* buff_, sz buff_sz_);

ARKAPI __decl_overloadable__ ark_string_stream ark_make_string_stream ();

ARKAPI u0 ark_reset_string_stream (ark_string_stream* ss_);

ARKAPI u0 ark_destroy_string_stream (ark_string_stream* ss_);

__decl_extern_inline__ u0 ark_cleanup_string_stream (ark_string_stream** __noalias__ ss_) __attr_diagnose_if__ (
	(ss_ == nullptr || *ss_ == nullptr), "wide string string is null", "error"
) {
	ark_destroy_string_stream (*ss_);
}

ARK_END

#endif /* ARK_STRING_STREAM_H_ */
