/*	$NetBSD: msg_126.c,v 1.5 2021/04/05 01:35:34 rillig Exp $	*/
# 3 "msg_126.c"

// Test for message: incompatible types '%s' and '%s' in conditional [126]

int
max(int cond, void *ptr, double dbl)	/* expect: 231 *//* expect: 231 *//* expect: 231 */
{
	return cond ? ptr : dbl;	/* expect: 126 *//* expect: 214 */
}
