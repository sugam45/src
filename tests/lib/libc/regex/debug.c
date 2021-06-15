/*	$NetBSD: debug.c,v 1.7 2021/02/25 22:37:36 christos Exp $	*/

/*-
 * Copyright (c) 1993 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE NETBSD FOUNDATION, INC. AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <sys/types.h>
#include <ctype.h>
#include <limits.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef __linux__
/* Don't sort these! */
#include "utils.h"
#include "regex2.h"
#else
#define REGEX_NODEBUG
#endif

#ifdef REGEX_TRE
#define REGEX_NODEBUG
#endif

#include "test_regex.h"

#ifndef REGEX_NODEBUG
static void s_print(struct re_guts *, FILE *);
static char *regchar(int);
#endif

/*
 * regprint - print a regexp for debugging
 */
void
regprint(regex_t *r, FILE *d)
{
#ifndef REGEX_NODEBUG
	struct re_guts *g = r->re_g;

	fprintf(d, ", first %u last %u", g->firststate, g->laststate);
	if (g->iflags&USEBOL)
		fprintf(d, ", USEBOL");
	if (g->iflags&USEEOL)
		fprintf(d, ", USEEOL");
	if (g->iflags&BAD)
		fprintf(d, ", BAD");
	if (g->nsub > 0)
		fprintf(d, ", nsub=%zu", g->nsub);
	if (g->must != NULL)
		fprintf(d, ", must(%zu) `%*s'", g->mlen, (int)g->mlen, g->must);
	if (g->backrefs)
		fprintf(d, ", backrefs");
	if (g->nplus > 0)
		fprintf(d, ", nplus %u", g->nplus);
	fprintf(d, "\n");
	s_print(g, d);
	fprintf(d, "\n");
#endif
}

#ifndef REGEX_NODEBUG
/*
 * s_print - print the strip for debugging
 */
static void
s_print(struct re_guts *g, FILE *d)
{
	sop *s;
	int done = 0;
	sop opnd;
	int col = 0;
	sopno offset = 2;
#	define	GAP()	{	if (offset % 5 == 0) { \
					if (col > 40) { \
						fprintf(d, "\n\t"); \
						col = 0; \
					} else { \
						fprintf(d, " "); \
						col++; \
					} \
				} else \
					col++; \
				offset++; \
			}

	if (OP(g->strip[0]) != OEND)
		fprintf(d, "missing initial OEND!\n");
	for (s = &g->strip[1]; !done; s++) {
		opnd = OPND(*s);
		switch (OP(*s)) {
		case OEND:
			fprintf(d, "\n");
			done = 1;
			break;
		case OCHAR:
			if (strchr("\\|()^$.[+*?{}!<> ", (char)opnd) != NULL)
				fprintf(d, "\\%c", (char)opnd);
			else
				fprintf(d, "%s", regchar((char)opnd));
			break;
		case OBOL:
			fprintf(d, "^");
			break;
		case OEOL:
			fprintf(d, "$");
			break;
		case OBOW:
			fprintf(d, "\\{");
			break;
		case OEOW:
			fprintf(d, "\\}");
			break;
		case OANY:
			fprintf(d, ".");
			break;
		case OANYOF:
			fprintf(d, "[(%u)", opnd);
			fprintf(d, "]");
			break;
		case OBACK_:
			fprintf(d, "(\\<%u>", opnd);
			break;
		case O_BACK:
			fprintf(d, "<%u>\\)", opnd);
			break;
		case OPLUS_:
			fprintf(d, "(+");
			if (OP(*(s+opnd)) != O_PLUS)
				fprintf(d, "<%u>", opnd);
			break;
		case O_PLUS:
			if (OP(*(s-opnd)) != OPLUS_)
				fprintf(d, "<%u>", opnd);
			fprintf(d, "+)");
			break;
		case OQUEST_:
			fprintf(d, "(?");
			if (OP(*(s+opnd)) != O_QUEST)
				fprintf(d, "<%u>", opnd);
			break;
		case O_QUEST:
			if (OP(*(s-opnd)) != OQUEST_)
				fprintf(d, "<%u>", opnd);
			fprintf(d, "?)");
			break;
		case OLPAREN:
			fprintf(d, "((<%u>", opnd);
			break;
		case ORPAREN:
			fprintf(d, "<%u>))", opnd);
			break;
		case OCH_:
			fprintf(d, "<");
			if (OP(*(s+opnd)) != OOR2)
				fprintf(d, "<%u>", opnd);
			break;
		case OOR1:
			if (OP(*(s-opnd)) != OOR1 && OP(*(s-opnd)) != OCH_)
				fprintf(d, "<%u>", opnd);
			fprintf(d, "|");
			break;
		case OOR2:
			fprintf(d, "|");
			if (OP(*(s+opnd)) != OOR2 && OP(*(s+opnd)) != O_CH)
				fprintf(d, "<%u>", opnd);
			break;
		case O_CH:
			if (OP(*(s-opnd)) != OOR1)
				fprintf(d, "<%u>", opnd);
			fprintf(d, ">");
			break;
		default:
			fprintf(d, "!%u(%u)!", OP(*s), opnd);
			break;
		}
		if (!done)
			GAP();
	}
}

/*
 * regchar - make a character printable
 */
static char *			/* -> representation */
regchar(int ch)
{
	static char buf[10];

	if (isprint(ch) || ch == ' ')
		sprintf(buf, "%c", ch);
	else
		sprintf(buf, "\\%o", ch);
	return(buf);
}
#endif
