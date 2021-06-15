/*	$NetBSD: feat_stacktrace.c,v 1.1 2021/04/08 22:18:27 rillig Exp $	*/
# 3 "feat_stacktrace.c"

/*
 * In macros or nested includes, lint prints a stack trace to show exactly
 * where the code comes from.
 */

# 1 "/usr/include/stdlib.h" 1 3 4
# 38 "/usr/include/stdlib.h" 3 4
# 39 "/usr/include/stdlib.h" 3 4
# 1 "/usr/include/sys/types.h" 1 3 4
# 43 "/usr/include/sys/types.h" 3 4
# 1 "/usr/include/amd64/types.h" 1 3 4
# 40 "/usr/include/amd64/types.h" 3 4
# 1 "/usr/include/sys/featuretest.h" 1 3 4
# 41 "/usr/include/amd64/types.h" 2 3 4
# 1 "/usr/include/amd64/int_types.h" 1 3 4

/*
 * The next filename is a relative filename since the tests are run without
 * the lint option -F, which would generate the fully qualified filename for
 * the main file as well.
 */
# 1 "common_int_types.h" 1 3 4
typedef int;		/* expect: typedef declares no type name */
# 39 "common_int_types.h" 3 4
# 39 "/usr/include/amd64/int_types.h" 2 3 4
# 42 "/usr/include/amd64/types.h" 2 3 4
# 68 "/usr/include/amd64/types.h" 3 4
# 46 "/usr/include/sys/types.h" 2 3 4
