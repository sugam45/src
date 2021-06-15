/*	$NetBSD: msg_132.c,v 1.5 2021/04/06 21:17:28 rillig Exp $	*/
# 3 "msg_132.c"

// Test for message: conversion from '%s' to '%s' may lose accuracy [132]

/*
 * NetBSD's default lint flags only include a single -a, which only flags
 * narrowing conversions from long.  To get warnings for all narrowing
 * conversions, -aa needs to be given more than once.
 *
 * https://gnats.netbsd.org/14531
 */

/* lint1-extra-flags: -aa */

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

typedef signed char i8;
typedef signed short i16;
typedef signed int i32;
typedef signed long long i64;

void
convert_unsigned(u8 v8, u16 v16, u32 v32, u64 v64)
{
	v8 = v16;		/* expect: 132 */
	v8 = v32;		/* expect: 132 */
	v8 = v64;		/* expect: 132 */

	v16 = v8;
	v16 = v32;		/* expect: 132 */
	v16 = v64;		/* expect: 132 */

	v32 = v8;
	v32 = v16;
	v32 = v64;		/* expect: 132 */

	v64 = v8;
	v64 = v16;
	v64 = v32;
}

void
convert_signed(i8 v8, i16 v16, i32 v32, i64 v64)
{
	v8 = v16;		/* expect: 132 */
	v8 = v32;		/* expect: 132 */
	v8 = v64;		/* expect: 132 */

	v16 = v8;
	v16 = v32;		/* expect: 132 */
	v16 = v64;		/* expect: 132 */

	v32 = v8;
	v32 = v16;
	v32 = v64;		/* expect: 132 */

	v64 = v8;
	v64 = v16;
	v64 = v32;
}

/*
 * Before tree.c 1.268 from 2021-04-06, lint wrongly warned that conversion to
 * _Bool might lose accuracy.  C99 6.3.1.2 defines a special conversion rule
 * from scalar to _Bool though.
 */
_Bool
to_bool(long a, long b)
{
	/* seen in fp_lib.h, function wideRightShiftWithSticky */
	return a | b;
}
