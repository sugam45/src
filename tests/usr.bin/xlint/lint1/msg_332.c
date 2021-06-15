/*	$NetBSD: msg_332.c,v 1.3 2021/04/05 01:35:34 rillig Exp $	*/
# 3 "msg_332.c"

// Test for message: right operand of '%s' must be bool, not '%s' [332]
//
// See d_c99_bool_strict.c for many more examples.

/* lint1-extra-flags: -T */

typedef _Bool bool;

void
test(bool);

void
example(bool b, char c, int i)
{
	test(b && b);
	test(b && c);		/* expect: 332 *//* expect: 334 */
	test(b && i);		/* expect: 332 *//* expect: 334 */

	test(c != '\0');
	test(i != 0);
}
