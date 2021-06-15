/*	$NetBSD: d_c99_init.c,v 1.33 2021/04/09 23:03:26 rillig Exp $	*/
# 3 "d_c99_init.c"

/*
 * Test C99 initializers.
 *
 * See C99 6.7.8 "Initialization".
*/


void use(const void *);

typedef struct any {
	const void *value;
} any;


// C99 6.7.8p11 says "optionally enclosed in braces".  There is no limitation
// on the number of brace pairs.
int scalar_without_braces = 3;
int scalar_with_optional_braces = { 3 };
int scalar_with_too_many_braces = {{ 3 }};
int scalar_with_too_many_initializers = { 3, 5 };	/* expect: 174 */


// See init_expr, 'handing over to ASSIGN'.
void
struct_initialization_via_assignment(any arg)
{
	any local = arg;
	use(&local);
}


// See init_expr, initialization_init_array_using_string.
char static_duration[] = "static duration";
signed char static_duration_signed[] = "static duration";
unsigned char static_duration_unsigned[] = "static duration";
int static_duration_wchar[] = L"static duration";

// See init_expr.
void
initialization_by_braced_string(void)
{
	any local = { "hello" };
	use(&local);
}

void
initialization_by_redundantly_braced_string(void)
{
	any local = {{{{ "hello" }}}};
	use(&local);
}

/*
 * Only scalar expressions and string literals may be enclosed by additional
 * braces.  Since 'arg' is a struct, this is a compile-time error.
 */
void
initialization_with_too_many_braces(any arg)
{
	any local = { arg };	/* expect: 185 */
	use(&arg);
}

// Some of the following examples are mentioned in the introduction comment
// in init.c.

int number = 12345;

int number_with_braces_and_comma = {
	12345,
};

int array_with_fixed_size[3] = {
	111,
	222,
	333,
	444,			/* expect: too many array initializers */
};

// See initialization_set_set_of_unknown_array.
int array_of_unknown_size[] = {
	111,
	222,
	333,
};

int array_flat[2][2] = {
	11,
	12,
	21,
	22
};

int array_nested[2][2] = {
	{
		11,
		12
	},
	{
		21,
		22
	}
};

int array_with_designators[] = {
	['1'] = 111,
	['5'] = 555,
	['9'] = 999
};

int array_with_some_designators[] = {
	['1'] = 111,
	222,
	['9'] = 999
};

struct point {
	int x;
	int y;
};

struct point point = {
	3,
	4
};

struct point point_with_designators = {
	.y = 4,
	.x = 3,
};

struct point point_with_mixed_designators = {
	.x = 3,
	4,
	5,			/* expect: too many struct/union initializers */
	.x = 3,
};

int array_with_designator[] = {
	111,
	.member = 222,		/* expect: 249 */
	333,
};

/*
 * C99 6.7.8p11 says that the initializer of a scalar can be "optionally
 * enclosed in braces".  It does not explicitly set an upper limit on the
 * number of braces.  It also doesn't restrict the term "initializer" to only
 * mean the "outermost initializer".  Both GCC 10 and Clang 8 already warn
 * about this, so there is no extra work for lint to do.
 */
struct point scalar_with_several_braces = {
	{{{3}}},
	{{{{4}}}},
};

struct rectangle {
	struct point top_left;
	struct point bottom_right;
};

/* C99 6.7.8p18 */
struct rectangle screen = {
	.bottom_right = {
		1920,
		1080,
	}
};

/*
 * C99 6.7.8p22 says: At the _end_ of its initializer list, the array no
 * longer has incomplete type.
 */
struct point points[] = {
	{
		/*
		 * At this point, the size of the object 'points' is not known
		 * yet since its type is still incomplete.  Lint could warn
		 * about this, but GCC and Clang already do.
		 *
		 * This test case demonstrates that in
		 * extend_if_array_of_unknown_size, setcomplete is called too
		 * early.
		 */
		sizeof(points),
		4
	}
};


struct triangle {
	struct point points[3];
};

struct pentagon {
	struct point points[5];
};

struct geometry {
	struct pentagon pentagons[6];
	struct triangle triangles[10];
	struct point points[3][5][2];
};

/*
 * Initialization of a complex struct containing nested arrays and nested
 * structs.
 */
struct geometry geometry = {
	.pentagons[0].points[4].x = 1,
	.points[0][0][0] = { 0, 0 },
	.points[2][4][1] = {301, 302 },
	/* expect+1: array subscript cannot be > 2: 3 */
	.points[3][0][0] = {3001, 3002 },
	/* expect+1: array subscript cannot be > 4: 5 */
	.points[0][5][0] = {501, 502 },
	/* expect+1: array subscript cannot be > 1: 2 */
	.points[0][0][2] = {21, 22 },
};

struct ends_with_unnamed_bit_field {
	int member;
	int : 0;
} ends_with_unnamed_bit_field = {
	12345,
	/* expect+1: too many struct/union initializers */
	23456,
};

char prefixed_message[] = {
	'E', ':', ' ',
	/* expect+1: illegal combination of integer (char) and pointer */
	"message\n",
};

char message_with_suffix[] = {
	"message",
	/* The excess character is not detected by lint but by compilers. */
	'\n',
};

struct ten {
	int i0;
	int i1;
	int i2;
	int i3;
	int i4;
	int i5;
	int i6;
	int i7;
	int i8;
	int i9;
};

struct ten ten = {
	.i3 = 3,
	4,
	5,
	6,
};

int c99_6_7_8_p26_example3[4][3] = {
	{ 1, 3, 5 },
	{ 2, 4, 6 },
	{ 3, 5, 7 },
};

int c99_6_7_8_p27_example4[4][3] = {
	{ 1 }, { 2 }, { 3 }, { 4 }
};

struct {
	int a[3], b;
} c99_6_7_8_p28_example5[] = {
	{ 1 },		/* just parsed, not checked in detail */
	2,		/* just parsed, not checked in detail */
};

short c99_6_7_8_p29_example6a[4][3][2] = {
	{ 1 },
	{ 2, 3 },
	{ 4, 5, 6 },
};

short c99_6_7_8_p29_example6b[4][3][2] = {
	1, 0, 0, 0, 0, 0,
	2, 3, 0, 0, 0, 0,
	4, 5, 6, 0, 0, 0,
};

short c99_6_7_8_p29_example6c[4][3][2] = {
	{
		{ 1 },
	},
	{
		{ 2, 3 },
	},
	{
		{ 4, 5 },
		{ 6 },
	}
};

/*
 * During initialization of an object of type array of unknown size, the type
 * information on the symbol is updated in-place.  Ensure that this happens on
 * a copy of the type.
 */
void
ensure_array_type_is_not_modified_during_initialization(void)
{
	typedef int array_of_unknown_size[];

	array_of_unknown_size a1 = { 1, 2, 3};

	switch (4) {
	case sizeof(array_of_unknown_size):
	case 0:			/* expect: duplicate case in switch: 0 */
	case 3:
	case 4:
	case 12:
		break;
	}
}

struct point unknown_member_name_beginning = {
	.r = 5,			/* expect: does not have member 'r' */
	.x = 4,
	.y = 3,
};

struct point unknown_member_name_middle = {
	.x = 4,
	.r = 5,			/* expect: does not have member 'r' */
	.y = 3,
};

struct point unknown_member_name_end = {
	.x = 4,
	.y = 3,
	.r = 5,			/* expect: does not have member 'r' */
};

union value {
	int int_value;
	void *pointer_value;
};

union value unknown_union_member_name_first = {
	.unknown_value = 4,	/* expect: does not have member */
	.int_value = 3,
};

union value unknown_union_member_name_second = {
	.int_value = 3,
	.unknown_value = 4,	/* expect: does not have member */
};

struct point designators_with_subscript = {
	[0] = 3,		/* expect: only for arrays */
	.member[0][0].member = 4, /* expect: does not have member 'member' */
	.x.y.z = 5,	/* intentionally not caught, see designator_look_up */
};

struct {
	int : 16;
} struct_with_only_unnamed_members = {	/* expect: has no named members */
	123,		/* expect: too many struct/union initializers */
};

union {
	int : 16;
} union_with_only_unnamed_members = {	/* expect: has no named members */
	123,		/* expect: too many struct/union initializers */
};

int designator_for_scalar = {
	.value = 3,		/* expect: scalar type cannot use designator */
};

struct point designator_for_scalar_in_struct = {
	{ .x = 3 },		/* expect: scalar type cannot use designator */
	{ [1] = 4 },		/* expect: scalar type cannot use designator */
};


/* Seen in pcidevs_data.h, variable 'pci_words'. */
const char string_initialized_with_braced_literal[] = {
	"initializer",
};
