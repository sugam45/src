/*	$NetBSD: msg_186.c,v 1.4 2021/02/22 15:09:50 rillig Exp $	*/
# 3 "msg_186.c"

/* Test for message: bit-field initialization is illegal in traditional C [186] */

/* lint1-flags: -tw */

struct bit_field {
	unsigned one: 1;
	unsigned three: 3;
	unsigned two: 2;
};

struct bit_field bit_field = {
	1,
	3.0,			/* expect: 186 */
	2
};
/* XXX: The message is misleading.  Initialization using integers is ok. */
