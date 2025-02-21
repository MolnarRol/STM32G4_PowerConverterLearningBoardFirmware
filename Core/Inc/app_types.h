/*
 * @file app_types.h
 * Application used types
 * @author Roland Molnar
 */
#include <stdint.h>
#include <limits.h>
#include <float.h>
#include <compiler_defines.h>

#ifndef INC_APP_TYPES_H_
#define INC_APP_TYPES_H_

/* Unsigned integers. */
typedef uint8_t 	U8;
typedef uint8_t     u8;
#define U8_MAX		( UINT8_MAX )

typedef uint16_t	U16;
typedef uint16_t    u16;
#define U16_MAX		( UINT16_MAX )

typedef uint32_t	U32;
typedef uint32_t    u32;
#define U32_MAX		( UINT32_MAX )

typedef uint64_t	U64;
typedef uint64_t    u64;
#define U64_MAX		( UINT64_MAX )

/* Signed integers. */
typedef int8_t		S8;
typedef int8_t      s8;
#define S8_MIN		( INT8_MIN )
#define S8_MAX		( INT8_MAX )

typedef int16_t		S16;
typedef int16_t     s16;
#define S16_MIN		( INT16_MIN )
#define S16_MAX		( INT16_MAX )

typedef int32_t		S32;
typedef int32_t     s32;
#define S32_MIN		( INT32_MIN )
#define S32_MAX		( INT32_MAX )

typedef int64_t		S64;
typedef int64_t     s64;
#define S64_MIN		( INT64_MIN )
#define S64_MAX		( INT64_MAX )

/* Floating point */
typedef float		F32;
typedef float        f32;
typedef double		F64;
typedef double      f64;

typedef uint8_t		boolean;

#define True_b		( (boolean) 1 )
#define False_b		( (boolean) 0 )

#endif /* INC_APP_TYPES_H_ */
