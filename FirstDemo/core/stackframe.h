/*
 * Copyright (C) 2020-2021 Suzhou Tiancheng Software Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include "cpu.h"
#include "regdef.h"

//----------------------------------------------------------------------------

#define CTX_OFFSET(n)   ((n)*4)

#if __mips_hard_float
#define CTX_SIZE        (72*4)
#else
#define CTX_SIZE        (38*4)
#endif

//----------------------------------------------------------------------------

#if __mips_hard_float

//-----------------------------------------------------------------------------
// CP1 ¼Ä´æÆ÷ÈëÕ» __mips_fpr==32
//-----------------------------------------------------------------------------

.macro _save_fpu

    swc1    $f0,  CTX_OFFSET(R_F0)(sp)
    swc1    $f1,  CTX_OFFSET(R_F1)(sp)
    swc1    $f2,  CTX_OFFSET(R_F2)(sp)
    swc1    $f3,  CTX_OFFSET(R_F3)(sp)
    swc1    $f4,  CTX_OFFSET(R_F4)(sp)
    swc1    $f5,  CTX_OFFSET(R_F5)(sp)
    swc1    $f6,  CTX_OFFSET(R_F6)(sp)
    swc1    $f7,  CTX_OFFSET(R_F7)(sp)
    swc1    $f8,  CTX_OFFSET(R_F8)(sp)
    swc1    $f9,  CTX_OFFSET(R_F9)(sp)
    swc1    $f10, CTX_OFFSET(R_F10)(sp)
    swc1    $f11, CTX_OFFSET(R_F11)(sp)
    swc1    $f12, CTX_OFFSET(R_F12)(sp)
    swc1    $f13, CTX_OFFSET(R_F13)(sp)
    swc1    $f14, CTX_OFFSET(R_F14)(sp)
    swc1    $f15, CTX_OFFSET(R_F15)(sp)
    swc1    $f16, CTX_OFFSET(R_F16)(sp)
    swc1    $f17, CTX_OFFSET(R_F17)(sp)
    swc1    $f18, CTX_OFFSET(R_F18)(sp)
    swc1    $f19, CTX_OFFSET(R_F19)(sp)
    swc1    $f20, CTX_OFFSET(R_F20)(sp)
    swc1    $f21, CTX_OFFSET(R_F21)(sp)
    swc1    $f22, CTX_OFFSET(R_F22)(sp)
    swc1    $f23, CTX_OFFSET(R_F23)(sp)
    swc1    $f24, CTX_OFFSET(R_F24)(sp)
    swc1    $f25, CTX_OFFSET(R_F25)(sp)
    swc1    $f26, CTX_OFFSET(R_F26)(sp)
    swc1    $f27, CTX_OFFSET(R_F27)(sp)
    swc1    $f28, CTX_OFFSET(R_F28)(sp)
    swc1    $f29, CTX_OFFSET(R_F29)(sp)
    swc1    $f30, CTX_OFFSET(R_F30)(sp)
    swc1    $f31, CTX_OFFSET(R_F31)(sp)
    cfc1    k1, fcr31
    cfc1    k1, fcr31
    nop
    nop
    sw      k1, CTX_OFFSET(R_FCSR)(sp)
    nop

.endm

//-----------------------------------------------------------------------------
// CP1 ¼Ä´æÆ÷³öÕ» __mips_fpr==32
//-----------------------------------------------------------------------------

.macro _load_fpu

    lwc1    $f0,  CTX_OFFSET(R_F0)(sp)
    lwc1    $f1,  CTX_OFFSET(R_F1)(sp)
    lwc1    $f2,  CTX_OFFSET(R_F2)(sp)
    lwc1    $f3,  CTX_OFFSET(R_F3)(sp)
    lwc1    $f4,  CTX_OFFSET(R_F4)(sp)
    lwc1    $f5,  CTX_OFFSET(R_F5)(sp)
    lwc1    $f6,  CTX_OFFSET(R_F6)(sp)
    lwc1    $f7,  CTX_OFFSET(R_F7)(sp)
    lwc1    $f8,  CTX_OFFSET(R_F8)(sp)
    lwc1    $f9,  CTX_OFFSET(R_F9)(sp)
    lwc1    $f10, CTX_OFFSET(R_F10)(sp)
    lwc1    $f11, CTX_OFFSET(R_F11)(sp)
    lwc1    $f12, CTX_OFFSET(R_F12)(sp)
    lwc1    $f13, CTX_OFFSET(R_F13)(sp)
    lwc1    $f14, CTX_OFFSET(R_F14)(sp)
    lwc1    $f15, CTX_OFFSET(R_F15)(sp)
    lwc1    $f16, CTX_OFFSET(R_F16)(sp)
    lwc1    $f17, CTX_OFFSET(R_F17)(sp)
    lwc1    $f18, CTX_OFFSET(R_F18)(sp)
    lwc1    $f19, CTX_OFFSET(R_F19)(sp)
    lwc1    $f20, CTX_OFFSET(R_F20)(sp)
    lwc1    $f21, CTX_OFFSET(R_F21)(sp)
    lwc1    $f22, CTX_OFFSET(R_F22)(sp)
    lwc1    $f23, CTX_OFFSET(R_F23)(sp)
    lwc1    $f24, CTX_OFFSET(R_F24)(sp)
    lwc1    $f25, CTX_OFFSET(R_F25)(sp)
    lwc1    $f26, CTX_OFFSET(R_F26)(sp)
    lwc1    $f27, CTX_OFFSET(R_F27)(sp)
    lwc1    $f28, CTX_OFFSET(R_F28)(sp)
    lwc1    $f29, CTX_OFFSET(R_F29)(sp)
    lwc1    $f30, CTX_OFFSET(R_F30)(sp)
    lwc1    $f31, CTX_OFFSET(R_F31)(sp)
    cfc1    k1, fcr31
    cfc1    k1, fcr31
    nop
    nop
    lw      k1, CTX_OFFSET(R_FCSR)(sp)
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
    sw      sp, CTX_OFFSET(R_SP)(sp)
    sw      k0, CTX_OFFSET(R_K0)(sp)
    sw      k1, CTX_OFFSET(R_K1)(sp)
    sw      ra, CTX_OFFSET(R_RA)(sp)
    sw      fp, CTX_OFFSET(R_FP)(sp)
    sw      gp, CTX_OFFSET(R_GP)(sp)
    sw      t9, CTX_OFFSET(R_T9)(sp)
    sw      t8, CTX_OFFSET(R_T8)(sp)
    sw      s7, CTX_OFFSET(R_S7)(sp)
    sw      s6, CTX_OFFSET(R_S6)(sp)
    sw      s5, CTX_OFFSET(R_S5)(sp)
    sw      s4, CTX_OFFSET(R_S4)(sp)
    sw      s3, CTX_OFFSET(R_S3)(sp)
    sw      s2, CTX_OFFSET(R_S2)(sp)
    sw      s1, CTX_OFFSET(R_S1)(sp)
    sw      s0, CTX_OFFSET(R_S0)(sp)
    sw      t7, CTX_OFFSET(R_T7)(sp)
    sw      t6, CTX_OFFSET(R_T6)(sp)
    sw      t5, CTX_OFFSET(R_T5)(sp)
    sw      t4, CTX_OFFSET(R_T4)(sp)
    sw      t3, CTX_OFFSET(R_T3)(sp)
    sw      t2, CTX_OFFSET(R_T2)(sp)
    sw      t1, CTX_OFFSET(R_T1)(sp)
    sw      t0, CTX_OFFSET(R_T0)(sp)
    sw      a3, CTX_OFFSET(R_A3)(sp)
    sw      a2, CTX_OFFSET(R_A2)(sp)
    sw      a1, CTX_OFFSET(R_A1)(sp)
    sw      a0, CTX_OFFSET(R_A0)(sp)
    sw      v1, CTX_OFFSET(R_V1)(sp)
    sw      v0, CTX_OFFSET(R_V0)(sp)
    sw      AT, CTX_OFFSET(R_AT)(sp)

    mflo    k0
    mfhi    k1
    sw      k0, CTX_OFFSET(R_MDLO)(sp)
    sw      k1, CTX_OFFSET(R_MDHI)(sp)

    mfc0    k1, C0_STATUS
    li      k0, SR_CU1
    sw      k1, CTX_OFFSET(R_SR)(sp)

#if __mips_hard_float
    and     k0, k0, k1                  /* HAS FPU? */
    beqz    k0, 1f
    nop

    _save_fpu

#endif

1:  mfc0    k0, C0_EPC
    sw      k0, CTX_OFFSET(R_EPC)(sp)
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
    lw      ra, CTX_OFFSET(R_RA)(sp)
    lw      fp, CTX_OFFSET(R_FP)(sp)
    lw      gp, CTX_OFFSET(R_GP)(sp)
    lw      t9, CTX_OFFSET(R_T9)(sp)
    lw      t8, CTX_OFFSET(R_T8)(sp)
    lw      s7, CTX_OFFSET(R_S7)(sp)
    lw      s6, CTX_OFFSET(R_S6)(sp)
    lw      s5, CTX_OFFSET(R_S5)(sp)
    lw      s4, CTX_OFFSET(R_S4)(sp)
    lw      s3, CTX_OFFSET(R_S3)(sp)
    lw      s2, CTX_OFFSET(R_S2)(sp)
    lw      s1, CTX_OFFSET(R_S1)(sp)
    lw      s0, CTX_OFFSET(R_S0)(sp)
    lw      t7, CTX_OFFSET(R_T7)(sp)
    lw      t6, CTX_OFFSET(R_T6)(sp)
    lw      t5, CTX_OFFSET(R_T5)(sp)
    lw      t4, CTX_OFFSET(R_T4)(sp)
    lw      t3, CTX_OFFSET(R_T3)(sp)
    lw      t2, CTX_OFFSET(R_T2)(sp)
    lw      t1, CTX_OFFSET(R_T1)(sp)
    lw      t0, CTX_OFFSET(R_T0)(sp)
    lw      a3, CTX_OFFSET(R_A3)(sp)
    lw      a2, CTX_OFFSET(R_A2)(sp)
    lw      a1, CTX_OFFSET(R_A1)(sp)
    lw      a0, CTX_OFFSET(R_A0)(sp)
    lw      v1, CTX_OFFSET(R_V1)(sp)
    lw      v0, CTX_OFFSET(R_V0)(sp)
    lw      AT, CTX_OFFSET(R_AT)(sp)

#if __mips_hard_float
    lw      k0, CTX_OFFSET(R_SR)(sp)
    li      k1, SR_CU1
    and     k0, k0, k1
    beqz    k0, 2f
    nop

    _load_fpu

#endif

2:  lw      k0, CTX_OFFSET(R_MDLO)(sp)
    lw      k1, CTX_OFFSET(R_MDHI)(sp)
    mtlo    k0
    mthi    k1

    .set    mips32r2
    di
    .set    mips32

    lw      k0, CTX_OFFSET(R_SR)(sp)
    lw      k1, CTX_OFFSET(R_EPC)(sp)

    addu    sp, CTX_SIZE
    
    mtc0    k0, C0_STATUS
    mtc0    k1, C0_EPC

    .set    mips32r2
    ehb
    .set    mips32

    lw      k0, (CTX_OFFSET(R_K0)-CTX_SIZE)(sp)
    lw      k1, (CTX_OFFSET(R_K1)-CTX_SIZE)(sp)

    eret
    nop

.endm

#endif /*__CONTEXT_H__*/


