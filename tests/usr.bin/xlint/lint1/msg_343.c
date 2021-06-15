/*	$NetBSD: msg_343.c,v 1.4 2021/04/14 22:06:38 rillig Exp $	*/
# 3 "msg_343.c"

/* Test for message: static array size is a C11 extension [343] */

/* lint1-flags: -Sw */

void takes_int_pointer(int []);
void takes_int_pointer_with_ignored_size(int [3]);
void takes_int_array(int[static 3]);	/* expect: 343 */
/* expect+1: syntax error '3' */
void takes_volatile_int_array(int[volatile 3]);

int
returns_int_pointer(int a[])
{
	return a[0];
}

int
returns_int_pointer_with_ignored_size(int a[3])
{
	return a[0];
}

int
returns_int_array(int a[static 3])	/* expect: 343 */
{
	return a[0];
}

int
/* expect+1: syntax error '3' */
returns_volatile_int_array(int a[volatile 3])
{
	/* expect+2: cannot dereference non-pointer type */
	/* expect+1: expects to return value */
	return a[0];
}
