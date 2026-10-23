/*
 * Copyright (c) 2026 Murilo Gonzalez. All Rights Reserved.
 *
 *              ARKLIB FAULT BUILDER
 *
 *  Builds a per-context exception enum from a flat list of exception
 *  names:
 *
 *      ARK_DECL_FAULTS (MyContext, Foo, Bar, Baz);
 *
 *  expands to:
 *
 *      typedef enum {
 *          MyContext_Foo = __COUNTER__ + 1,
 *          MyContext_Bar = __COUNTER__ + 1,
 *          MyContext_Baz = __COUNTER__ + 1,
 *      } MyContext_FAULTS;
 *
 *  Up to 64 exception names per context.
 *
 */
#ifndef ARKLIB_FAULT_BUILDER_H_
	#define ARKLIB_FAULT_BUILDER_H_

	#define ARK_FAULT_ID(Ctx_, Except_) ark_pp_concat (Ctx_, _, Except_)

	#define ARK_DECL_FAULT_SEQ(Ctx_, Except_) ARK_FAULT_ID (Ctx_, Except_) = __COUNTER__ + 1,

	#define ARK_DECL_FAULT_TABLE(Ctx_, Except_)                                                                        \
		[ARK_FAULT_ID (Ctx_, Except_)] = ark_pp_stringfy (ARK_FAULT_ID (Ctx_, Except_)),

/*
 * Core Dispatcher
 */
