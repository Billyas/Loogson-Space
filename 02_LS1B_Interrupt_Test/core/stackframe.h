/*
 * Copyright (C) 2020-2021 Suzhou Tiancheng Software Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include "bsp.h"

#include "cpu.h"
#include "regdef.h"

#if !__mips_hard_float
#define CTX_SIZE        (33*4)
#else
#define CTX_SIZE        (66*4)
#endif

#define CTX_OFFSET(n)   ((n)*4)

//----------------------------------------------------------------------------

#if __mips_hard_float

//-----------------------------------------------------------------------------
// CP1 ¼Ä´æÆ÷ÈëÕ» __mips_fpr==32
//-----------------------------------------------------------------------------

.macro _save_fpu

    swc1    $f0,  CTX_OFFSET(33)(sp)
    swc1    $f1,  CTX_OFFSET(34)(sp)
    swc1    $f2,  CTX_OFFSET(35)(sp)
    swc1    $f3,  CTX_OFFSET(36)(sp)
    swc1    $f4,  CTX_OFFSET(37)(sp)
    swc1    $f5,  CTX_OFFSET(38)(sp)
    swc1    $f6,  CTX_OFFSET(39)(sp)
    swc1    $f7,  CTX_OFFSET(40)(sp)
    swc1    $f8,  CTX_OFFSET(41)(sp)
    swc1    $f9,  CTX_OFFSET(42)(sp)
    swc1    $f10, CTX_OFFSET(43)(sp)
    swc1    $f11, CTX_OFFSET(44)(sp)
    swc1    $f12, CTX_OFFSET(45)(sp)
    swc1    $f13, CTX_OFFSET(46)(sp)
    swc1    $f14, CTX_OFFSET(47)(sp)
    swc1    $f15, CTX_OFFSET(48)(sp)
    swc1    $f16, CTX_OFFSET(49)(sp)
    swc1    $f17, CTX_OFFSET(50)(sp)
    swc1    $f18, CTX_OFFSET(51)(sp)
    swc1    $f19, CTX_OFFSET(52)(sp)
    swc1    $f20, CTX_OFFSET(53)(sp)
    swc1    $f21, CTX_OFFSET(54)(sp)
    swc1    $f22, CTX_OFFSET(55)(sp)
    swc1    $f23, CTX_OFFSET(56)(sp)
    swc1    $f24, CTX_OFFSET(57)(sp)
    swc1    $f25, CTX_OFFSET(58)(sp)
    swc1    $f26, CTX_OFFSET(59)(sp)
    swc1    $f27, CTX_OFFSET(60)(sp)
    swc1    $f28, CTX_OFFSET(61)(sp)
    swc1    $f29, CTX_OFFSET(62)(sp)
    swc1    $f30, CTX_OFFSET(63)(sp)
    swc1    $f31, CTX_OFFSET(64)(sp)
    cfc1    k1, fcr31
    cfc1    k1, fcr31
    nop
    nop
    sw 		k1, CTX_OFFSET(65)(sp)
    nop

.endm

//-----------------------------------------------------------------------------
// CP1 ¼Ä´æÆ÷³öÕ» __mips_fpr==32
//-----------------------------------------------------------------------------

.macro _load_fpu

    lwc1    $f0,  CTX_OFFSET(33)(sp)
    lwc1    $f1,  CTX_OFFSET(34)(sp)
    lwc1    $f2,  CTX_OFFSET(35)(sp)
    lwc1    $f3,  CTX_OFFSET(36)(sp)
    lwc1    $f4,  CTX_OFFSET(37)(sp)
    lwc1    $f5,  CTX_OFFSET(38)(sp)
    lwc1    $f6,  CTX_OFFSET(39)(sp)
    lwc1    $f7,  CTX_OFFSET(40)(sp)
    lwc1    $f8,  CTX_OFFSET(41)(sp)
    lwc1    $f9,  CTX_OFFSET(42)(sp)
    lwc1    $f10, CTX_OFFSET(43)(sp)
    lwc1    $f11, CTX_OFFSET(44)(sp)
    lwc1    $f12, CTX_OFFSET(45)(sp)
    lwc1    $f13, CTX_OFFSET(46)(sp)
    lwc1    $f14, CTX_OFFSET(47)(sp)
    lwc1    $f15, CTX_OFFSET(48)(sp)
    lwc1    $f16, CTX_OFFSET(49)(sp)
    lwc1    $f17, CTX_OFFSET(50)(sp)
    lwc1    $f18, CTX_OFFSET(51)(sp)
    lwc1    $f19, CTX_OFFSET(52)(sp)
    lwc1    $f20, CTX_OFFSET(53)(sp)
    lwc1    $f21, CTX_OFFSET(54)(sp)
    lwc1    $f22, CTX_OFFSET(55)(sp)
    lwc1    $f23, CTX_OFFSET(56)(sp)
    lwc1    $f24, CTX_OFFSET(57)(sp)
    lwc1    $f25, CTX_OFFSET(58)(sp)
    lwc1    $f26, CTX_OFFSET(59)(sp)
    lwc1    $f27, CTX_OFFSET(60)(sp)
    lwc1    $f28, CTX_OFFSET(61)(sp)
    lwc1    $f29, CTX_OFFSET(62)(sp)
    lwc1    $f30, CTX_OFFSET(63)(sp)
    lwc1    $f31, CTX_OFFSET(64)(sp)
    cfc1    k1, fcr31
    cfc1    k1, fcr31
    nop
    nop
    lw      k1, CTX_OFFSET(65)(sp)
    nop
    ctc1    k1, fcr31
    nop

.endm

#endif  /* __mips_hard_float */

//-----------------------------------------------------------------------------
// CP0, CP1 ¼Ä´æÆ÷ÈëÕ»
//-----------------------------------------------------------------------------

//
// push context: at, v0-v1,a0-a3,t0-t9,s0-s7,gp,fp,ra, FR0~FR31, FCR1, & pc
//
.macro _save_all

    .set    noat
    .set    noreorder
    subu    sp, CTX_SIZE
    sw      ra, CTX_OFFSET(1)(sp)
    sw      fp, CTX_OFFSET(2)(sp)
    sw      gp, CTX_OFFSET(3)(sp)
    sw      t9, CTX_OFFSET(4)(sp)
    sw      t8, CTX_OFFSET(5)(sp)
    sw      s7, CTX_OFFSET(6)(sp)
    sw      s6, CTX_OFFSET(7)(sp)
    sw      s5, CTX_OFFSET(8)(sp)
    sw      s4, CTX_OFFSET(9)(sp)
    sw      s3, CTX_OFFSET(10)(sp)
    sw      s2, CTX_OFFSET(11)(sp)
    sw      s1, CTX_OFFSET(12)(sp)
    sw      s0, CTX_OFFSET(13)(sp)
    sw      t7, CTX_OFFSET(14)(sp)
    sw      t6, CTX_OFFSET(15)(sp)
    sw      t5, CTX_OFFSET(16)(sp)
    sw      t4, CTX_OFFSET(17)(sp)
    sw      t3, CTX_OFFSET(18)(sp)
    sw      t2, CTX_OFFSET(19)(sp)
    sw      t1, CTX_OFFSET(20)(sp)
    sw      t0, CTX_OFFSET(21)(sp)
    sw      a3, CTX_OFFSET(22)(sp)
    sw      a2, CTX_OFFSET(23)(sp)
    sw      a1, CTX_OFFSET(24)(sp)
    sw      a0, CTX_OFFSET(25)(sp)
    sw      v1, CTX_OFFSET(26)(sp)
    sw      v0, CTX_OFFSET(27)(sp)
    sw      AT, CTX_OFFSET(28)(sp)

	mflo	k0
	mfhi	k1
	sw	    k0, CTX_OFFSET(29)(sp)
	sw	    k1, CTX_OFFSET(30)(sp)

    mfc0    k1, C0_STATUS
    li      k0, SR_CU1
    sw      k1, CTX_OFFSET(31)(sp)

#if __mips_hard_float
    and     k0, k0, k1                  /* HAS FPU? */
    beqz    k0, 1f
    nop

    _save_fpu

#endif

1:  mfc0    k0, C0_EPC
    sw      k0, CTX_OFFSET(32)(sp)
    nop

.endm

//-----------------------------------------------------------------------------
// CP0, CP1 ¼Ä´æÆ÷³öÕ»
//-----------------------------------------------------------------------------

//
// pop context: at, v0-v1,a0-a3,t0-t9,s0-s7,gp,fp,ra, FR0~FR31, FCR1 & pc
//
.macro _load_all_eret

    .set    noat
    .set    noreorder
    lw      ra, CTX_OFFSET(1)(sp)
    lw      fp, CTX_OFFSET(2)(sp)
    lw      gp, CTX_OFFSET(3)(sp)
    lw      t9, CTX_OFFSET(4)(sp)
    lw      t8, CTX_OFFSET(5)(sp)
    lw      s7, CTX_OFFSET(6)(sp)
    lw      s6, CTX_OFFSET(7)(sp)
    lw      s5, CTX_OFFSET(8)(sp)
    lw      s4, CTX_OFFSET(9)(sp)
    lw      s3, CTX_OFFSET(10)(sp)
    lw      s2, CTX_OFFSET(11)(sp)
    lw      s1, CTX_OFFSET(12)(sp)
    lw      s0, CTX_OFFSET(13)(sp)
    lw      t7, CTX_OFFSET(14)(sp)
    lw      t6, CTX_OFFSET(15)(sp)
    lw      t5, CTX_OFFSET(16)(sp)
    lw      t4, CTX_OFFSET(17)(sp)
    lw      t3, CTX_OFFSET(18)(sp)
    lw      t2, CTX_OFFSET(19)(sp)
    lw      t1, CTX_OFFSET(20)(sp)
    lw      t0, CTX_OFFSET(21)(sp)
    lw      a3, CTX_OFFSET(22)(sp)
    lw      a2, CTX_OFFSET(23)(sp)
    lw      a1, CTX_OFFSET(24)(sp)
    lw      a0, CTX_OFFSET(25)(sp)
    lw      v1, CTX_OFFSET(26)(sp)
    lw      v0, CTX_OFFSET(27)(sp)
    lw      AT, CTX_OFFSET(28)(sp)

#if __mips_hard_float
    lw      k0, CTX_OFFSET(31)(sp)
    li      k1, SR_CU1
    and     k0, k0, k1
    beqz    k0, 2f
    nop

    _load_fpu

#endif

2:  lw   	k0, CTX_OFFSET(29)(sp)
	lw   	k1, CTX_OFFSET(30)(sp)
	mtlo	k0
	mthi	k1

	.set    mips32r2
	di
	.set    mips32

    lw      k0, CTX_OFFSET(31)(sp)
    lw      k1, CTX_OFFSET(32)(sp)
    addu    sp, CTX_SIZE
    mtc0    k0, C0_STATUS
    mtc0    k1, C0_EPC

	.set    mips32r2
    ehb
	.set    mips32

	eret
    nop

.endm

#endif /*__CONTEXT_H__*/


