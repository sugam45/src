/*	$NetBSD: result.c,v 1.3 2019/02/25 02:31:57 christos Exp $	*/

/* result.c
 */

/* 
 * Copyright (c) 2004-2017 by Internet Systems Consortium, Inc. ("ISC")
 * Copyright (c) 1999-2003 by Internet Software Consortium
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND ISC DISCLAIMS ALL WARRANTIES WITH
 * REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS.  IN NO EVENT SHALL ISC BE LIABLE FOR ANY SPECIAL, DIRECT,
 * INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
 * LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE
 * OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 *
 *   Internet Systems Consortium, Inc.
 *   950 Charter Street
 *   Redwood City, CA 94063
 *   <info@isc.org>
 *   https://www.isc.org/
 *
 */

#include <sys/cdefs.h>
__RCSID("$NetBSD: result.c,v 1.3 2019/02/25 02:31:57 christos Exp $");

#include "dhcpd.h"

/*
 * In the previous code the results started at 36
 * rather than ISC_RESULTCLASS_DHCP + 0
 * ISC_R_NOTCONNECTED was + 4 (40), it has been superseeded by the isc version
 */

static const char *text[DHCP_R_NRESULTS] = {
	"host unknown",				/* 0 */
	"protocol version mismatch",		/* 1 */
	"protocol error",			/* 2 */
	"invalid argument",			/* 3 */
	"data not yet available",		/* 4 */
	"object unchanged",			/* 5 */
	"more than one object matches key",	/* 6 */
	"key conflict",				/* 7 */
	"parse error(s) occurred",		/* 8 */
	"no key specified",			/* 9 */
	"zone TSIG key not known",		/* 10 */
	"invalid TSIG key",			/* 11 */
	"operation in progress",		/* 12 */
	"DNS format error",			/* 13 */
	"DNS server failed",			/* 14 */
	"no such domain",			/* 15 */
	"not implemented",			/* 16 */
	"refused",				/* 17 */
	"domain already exists",		/* 18 */
	"RRset already exists",			/* 19 */
	"no such RRset",			/* 20 */
	"not authorized",			/* 21 */
	"not a zone",				/* 22 */
	"bad DNS signature",			/* 23 */
	"bad DNS key",				/* 24 */
	"clock skew too great",			/* 25 */
	"no root zone",				/* 26 */
	"destination address required",		/* 27 */
	"cross-zone update",			/* 28 */
	"no TSIG signature",			/* 29 */
	"not equal",				/* 30 */
	"connection reset by peer",		/* 31 */
	"unknown attribute"			/* 32 */
};

#define DHCP_RESULT_RESULTSET		2
#define DHCP_RESULT_UNAVAILABLESET	3

isc_result_t
dhcp_result_register(void) {
	isc_result_t result;

	result = isc_result_register(ISC_RESULTCLASS_DHCP, DHCP_R_NRESULTS,
				     text, DHCP_RESULT_RESULTSET);

	return(result);
}