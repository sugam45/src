/*	$NetBSD: lib.h,v 1.6 2021/04/29 17:26:12 christos Exp $	*/

/*
 * Copyright (C) Internet Systems Consortium, Inc. ("ISC")
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, you can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * See the COPYRIGHT file distributed with this work for additional
 * information regarding copyright ownership.
 */

#pragma once

/*! \file isc/lib.h */

#include <isc/lang.h>
#include <isc/types.h>

ISC_LANG_BEGINDECLS

void
isc_lib_register(void);
/*!<
 * \brief Register the ISC library implementations for some base services
 * such as memory or event management and handling socket or timer events.
 * An external application that wants to use the ISC library must call this
 * function very early in main().
 */

extern void
isc_enable_constructors(void);
/*!<
 * \bried Enable constructor linkage in non-libtool static builds
 */

ISC_LANG_ENDDECLS
