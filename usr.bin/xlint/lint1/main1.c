/*	$NetBSD: main1.c,v 1.45 2021/04/18 22:51:24 rillig Exp $	*/

/*
 * Copyright (c) 1994, 1995 Jochen Pohl
 * All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *      This product includes software developed by Jochen Pohl for
 *	The NetBSD Project.
 * 4. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#if HAVE_NBTOOL_CONFIG_H
#include "nbtool_config.h"
#endif

#include <sys/cdefs.h>
#if defined(__RCSID) && !defined(lint)
__RCSID("$NetBSD: main1.c,v 1.45 2021/04/18 22:51:24 rillig Exp $");
#endif

#include <sys/types.h>
#include <errno.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "lint1.h"

/* set yydebug to 1*/
bool	yflag;

/*
 * Print warnings if an assignment of an integer type to another integer type
 * causes an implicit narrowing conversion. If aflag is 1, these warnings
 * are printed only if the source type is at least as wide as long. If aflag
 * is greater than 1, they are always printed.
 */
int	aflag;

/* Print a warning if a break statement cannot be reached. */
bool	bflag;

/* Print warnings for pointer casts. */
bool	cflag;

/* Allow features from C11, C99 and C90. */
bool	c11flag;

/* Print various debug information. */
bool	dflag;

/* Perform stricter checking of enum types and operations on enum types. */
bool	eflag;

/* Print complete pathnames, not only the basename. */
bool	Fflag;

/* Enable some extensions of gcc */
bool	gflag;

/* Treat warnings as errors */
bool	wflag;

/*
 * Apply a number of heuristic tests to attempt to intuit bugs, improve
 * style, and reduce waste.
 */
bool	hflag;

/* Attempt to check portability to other dialects of C. */
bool	pflag;

/*
 * In case of redeclarations/redefinitions print the location of the
 * previous declaration/definition.
 */
bool	rflag;

/* Strict ANSI C mode. */
bool	sflag;

bool	Tflag;

/* Traditional C mode. */
bool	tflag;

/* Enable C9X extensions */
bool	Sflag;

/* Picky flag */
bool	Pflag;

/*
 * Complain about functions and external variables used and not defined,
 * or defined and not used.
 */
bool	uflag = true;

/* Complain about unused function arguments. */
bool	vflag = true;

/* Complain about structures which are never defined. */
bool	zflag = true;

err_set	msgset;

sig_atomic_t fpe;

static	void	usage(void);

static const char builtins[] =
    "int __builtin_isinf(long double);\n"
    "int __builtin_isnan(long double);\n"
    "int __builtin_copysign(long double, long double);\n"
;
static const size_t builtinlen = sizeof(builtins) - 1;

static FILE *
gcc_builtins(void)
{
#if HAVE_NBTOOL_CONFIG_H
	char template[] = "/tmp/lint.XXXXXX";
	int fd;
	FILE *fp;
	if ((fd = mkstemp(template)) == -1)
		return NULL;
	(void)unlink(template);
	if ((fp = fdopen(fd, "r+")) == NULL) {
		close(fd);
		return NULL;
	}
	if (fwrite(builtins, 1, builtinlen, fp) != builtinlen) {
		fclose(fp);
		return NULL;
	}
	rewind(fp);
	return fp;
#else
	return fmemopen(__UNCONST(builtins), builtinlen, "r");
#endif
}

/*ARGSUSED*/
static void
sigfpe(int s)
{
	fpe = 1;
}

int
main(int argc, char *argv[])
{
	int	c;
	char	*ptr;

	setprogname(argv[0]);

	ERR_ZERO(&msgset);
	while ((c = getopt(argc, argv, "abcdeghmprstuvwyzA:FPR:STX:")) != -1) {
		switch (c) {
		case 'a':	aflag++;	break;
		case 'b':	bflag = true;	break;
		case 'c':	cflag = true;	break;
		case 'd':	dflag = true;	break;
		case 'e':	eflag = true;	break;
		case 'F':	Fflag = true;	break;
		case 'g':	gflag = true;	break;
		case 'h':	hflag = true;	break;
		case 'p':	pflag = true;	break;
		case 'P':	Pflag = true;	break;
		case 'r':	rflag = true;	break;
		case 's':	sflag = true;	break;
		case 'S':	Sflag = true;	break;
		case 'T':	Tflag = true;	break;
		case 't':	tflag = true;	break;
		case 'u':	uflag = false;	break;
		case 'w':	wflag = true;	break;
		case 'v':	vflag = false;	break;
		case 'y':	yflag = true;	break;
		case 'z':	zflag = false;	break;

		case 'A':
			if (strcmp(optarg, "c11") == 0) {
				c11flag = true;
				Sflag = true;
				sflag = true;
			} else
				usage();
			break;

		case 'm':
			msglist();
			return 0;

		case 'R':
			add_directory_replacement(optarg);
			break;

		case 'X':
			for (ptr = strtok(optarg, ","); ptr != NULL;
			    ptr = strtok(NULL, ",")) {
				char *eptr;
				long msg;

				errno = 0;
				msg = strtol(ptr, &eptr, 0);
				if ((msg == TARG_LONG_MIN || msg == TARG_LONG_MAX) &&
				    errno == ERANGE)
				    err(1, "invalid error message id '%s'",
					ptr);
				if (*eptr != '\0' || ptr == eptr || msg < 0 ||
				    msg >= ERR_SETSIZE)
					errx(1, "invalid error message id '%s'",
					    ptr);
				ERR_SET(msg, &msgset);
			}
			break;
		default:
			usage();
		}
	}
	argc -= optind;
	argv += optind;

	if (argc != 2)
		usage();


	/* initialize output */
	outopen(argv[1]);

#ifdef YYDEBUG
	if (yflag)
		yydebug = 1;
#endif

	(void)signal(SIGFPE, sigfpe);
	initmem();
	initdecl();
	initscan();

	if (gflag) {
		if ((yyin = gcc_builtins()) == NULL)
			err(1, "cannot open builtins");
		yyparse();
		fclose(yyin);
	}

	/* open the input file */
	if ((yyin = fopen(argv[0], "r")) == NULL)
		err(1, "cannot open '%s'", argv[0]);
	yyparse();
	fclose(yyin);

	/* Following warnings cannot be suppressed by LINTED */
	lwarn = LWARN_ALL;
#ifdef DEBUG
	printf("%s, %d: lwarn = %d\n", curr_pos.p_file, curr_pos.p_line, lwarn);
#endif

	check_global_symbols();

	outclose();

	return nerr != 0 ? 1 : 0;
}

static void __attribute__((noreturn))
usage(void)
{
	(void)fprintf(stderr,
	    "usage: %s [-abcdeghmprstuvwyzFST] [-Ac11] [-X <id>[,<id>]... "
	    "src dest\n",
	    getprogname());
	exit(1);
}

void __attribute__((noreturn))
norecover(void)
{
	/* cannot recover from previous errors */
	error(224);
	exit(1);
}
