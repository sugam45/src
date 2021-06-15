/*	$NetBSD: msg_071.c,v 1.4 2021/04/05 01:35:34 rillig Exp $	*/
# 3 "msg_071.c"

// Test for message: too many characters in character constant [71]

/*
 * C11 6.4.4.4p7 says: Each hexadecimal escape sequence is the longest
 * sequence of characters that can constitute the escape sequence.
 */
char valid_multi_digit_hex = '\x0000000000000000000000a';
char invalid_multi_digit_hex = '\x000g000000000000000000a'; /* expect: 71 *//* expect: 178 */
