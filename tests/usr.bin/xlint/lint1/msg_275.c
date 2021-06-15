/*	$NetBSD: msg_275.c,v 1.4 2021/02/28 12:40:00 rillig Exp $	*/
# 3 "msg_275.c"

// Test for message: cast discards 'const' from type '%s' [275]

/* lint1-extra-flags: -h */

char *
unconst_string(const char *s)
{
	return (char *)s;	/* expect: 275 */
}

const char *
const_string(char *s)
{
	return (const char *)s;
}
