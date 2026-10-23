/*
 * Copyright (c) 2026 Murilo Gonzalez. All Rights Reserved.
 *
 *                     ARKLIB TYPE REFLECTION
 *
 *  Provides compile-time structure reflection metadata and C3-style
 *  enums featuring rich string and typed static associated data.
 *
 */
#ifndef ARK_TYPE_REFLECTION_H_
#define ARK_TYPE_REFLECTION_H_

#import "__arklib_pputils_.h"
#import "__arklib_types_.h"

/* ==========================================================================
   METADATA TYPES LAYOUT
   ========================================================================== */
typedef struct ArkFieldMetadata {
	CString type_name;
	CString field_name;
	sz		offset;
} ArkFieldMetadata;

typedef struct ArkTypeMetadata {
	CString					struct_name;
	const ArkFieldMetadata* fields;
	sz						field_count;
} ArkTypeMetadata;

typedef struct ArkEnumMetadata {
	CString		name;
	u64			value;
	const void* associated_data;
} ArkEnumMetadata;

typedef struct ArkEnumTypeReflection {
	CString				   enum_name;
	CString				   associated_type_name;
	const ArkEnumMetadata* members;
	sz					   member_count;
} ArkEnumTypeReflection;

#endif /* ARK_TYPE_REFLECTION_H_ */
