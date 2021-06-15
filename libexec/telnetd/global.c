/*	$NetBSD: global.c,v 1.9 2020/04/23 00:03:40 joerg Exp $	*/

/*
 * Copyright (c) 1989, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <sys/cdefs.h>
#ifndef lint
#if 0
static char sccsid[] = "@(#)global.c	8.1 (Berkeley) 6/4/93";
#else
__RCSID("$NetBSD: global.c,v 1.9 2020/04/23 00:03:40 joerg Exp $");
#endif
#endif /* not lint */

/*
 * Allocate global variables.  We do this
 * by including the header file that defines
 * them all as externs, but first we define
 * the keyword "extern" to be nothing, so that
 * we will actually allocate the space.
 */

#include <defs.h>
#include <ext.h>

char	options[256];
char	do_dont_resp[256];
char	will_wont_resp[256];
int	linemode;	/* linemode on/off */
#ifdef	LINEMODE
int	uselinemode;	/* what linemode to use (on/off) */
int	editmode;	/* edit modes in use */
int	useeditmode;	/* edit modes to use */
int	alwayslinemode;	/* command line option */
# ifdef	KLUDGELINEMODE
int	lmodetype;	/* Client support for linemode */
# endif	/* KLUDGELINEMODE */
#endif	/* LINEMODE */
int	flowmode;	/* current flow control state */
int	restartany;	/* restart output on any character state */
#ifdef DIAGNOSTICS
int	diagnostic;	/* telnet diagnostic capabilities */
#endif /* DIAGNOSTICS */
#ifdef SECURELOGIN
int	require_secure_login;
#endif
#ifdef AUTHENTICATION
int	auth_level;
#endif

slcfun	slctab[NSLC + 1];	/* slc mapping table */
char	terminaltype[41];

/*
 * I/O data buffers, pointers, and counters.
 */
char	ptyobuf[BUFSIZ+NETSLOP], *pfrontp, *pbackp;
char	netibuf[BUFSIZ], *netip;
char	netobuf[BUFSIZ+NETSLOP], *nfrontp, *nbackp;
char	*neturg;		/* one past last bye of urgent data */
int	pcc, ncc;
int	pty, net;
char	*line;
int	SYNCHing;		/* we are in TELNET SYNCH mode */

#ifdef	ENCRYPTION
char	*nclearto;
#endif	/* ENCRYPTION */

struct clockstate clocks;
