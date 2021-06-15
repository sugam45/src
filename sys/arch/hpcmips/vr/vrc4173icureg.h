/*	$NetBSD: vrc4173icureg.h,v 1.1 2001/06/13 07:32:48 enami Exp $	*/

/*-
 * Copyright (c) 2001 Enami Tsugutomo.
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

#define	VRC4173ICU_IOBASE	0x060
#define	VRC4173ICU_IOSIZE	0x018

#define	VRC4173ICU_SYSINT1	0x00
#define	VRC4173ICU_DOZEPIUINTR	13
#define	VRC4173ICU_AC97INTR1	10
#define	VRC4173ICU_AC97INTR	9
#define	VRC4173ICU_GIUINTR	8
#define	VRC4173ICU_KIUINTR	7
#define	VRC4173ICU_AIUINTR	6
#define	VRC4173ICU_PIUINTR	5
#define	VRC4173ICU_PS2CH1INTR	4
#define	VRC4173ICU_PS2CH2INTR	3
#define	VRC4173ICU_PCMCIA1INTR	2
#define	VRC4173ICU_PCMCIA2INTR	1
#define	VRC4173ICU_USBINTR	0

#define	VRC4173ICU_PIUINT	0x02
#define	VRC4173ICU_AIUINT	0x04

#define	VRC4173ICU_KIUINT	0x06
#define	VRC4173ICU_KDATLOST	(1<<2)
#define	VRC4173ICU_KDATRDY	(1<<1)
#define	VRC4173ICU_SCANINT	(1<<0)

#define	VRC4173ICU_GIULINT	0x08
#define	VRC4173ICU_GIUHINT	0x0a

#define	VRC4173ICU_MSYSINT1	0x0c
#define	VRC4173ICU_MPIUINT	0x0e
#define	VRC4173ICU_MAIUINT	0x10
#define	VRC4173ICU_MKIUINT	0x12
#define	VRC4173ICU_MGIULINT	0x14
#define	VRC4173ICU_MGIUHINT	0x16