/*	$NetBSD: msg_003.c,v 1.4 2021/02/28 12:40:00 rillig Exp $	*/
# 3 "msg_003.c"

// Test for message: '%s' declared in argument declaration list [3]

/*ARGSUSED*/
void
example(declare_struct, declare_union, declare_enum)
    struct struct_in_argument *declare_struct;	/* expect: 3 */
    union union_in_argument *declare_union;	/* expect: 3 */
    enum enum_in_argument *declare_enum;	/* expect: 3 */
{
}
