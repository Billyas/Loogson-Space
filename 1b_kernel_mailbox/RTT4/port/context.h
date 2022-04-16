/*
 * Copyright (C) 2020-2021 Suzhou Tiancheng Software Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * context.h
 *
 * created: 2020/4/21
 *  author: Bian
 */

#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include "regdef.h"
#include "cpu.h"

//-----------------------------------------------------------------------------

#define CTX_OFFSET(n)   ((n)*4)       

#if __mips_hard_float
#define CTX_SIZE        (72*4)
#else
#define CTX_SIZE        (38*4)
#endif

//-----------------------------------------------------------------------------

#if defined(__ASSEMBLER__)

#if __mips_hard_float

//-----------------------------------------------------------------------------

.macro SAVE_FPU
    .set    noreorder
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
    cfc1    k0, fcr31                       /* Read FP status/conrol reg */
    cfc1    k0, fcr31                       /* Two reads clear pipeline */
    nop 
    nop 
    sw 		k0, CTX_OFFSET(R_FCSR)(sp);     /* Store value to FPCS location */
    nop
    .set    reorder
.endm

//-----------------------------------------------------------------------------

.macro RESTORE_FPU
    .set    noreorder
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
    cfc1    k0, fcr31                      /* Read from FP status/control reg */
    cfc1    k0, fcr31                      /* Two reads clear pipeline */
    nop  
    nop                                    /* nops ensure execution */
    lw      k0, CTX_OFFSET(R_FCSR)(sp)     /* Load saved FPCS value */
    nop
    ctc1    k0, fcr31                      /* Restore FPCS register */
    nop 
    .set    reorder       
.endm

#endif

//-----------------------------------------------------------------------------

.macro SAVE_CONTEXT
    .set    noreorder
    .set    noat
    subu    sp, sp, CTX_SIZE
	/* Save general registers.  */
	sw      AT, CTX_OFFSET(R_AT)(sp)
	sw      v0, CTX_OFFSET(R_V0)(sp)
	sw      v1, CTX_OFFSET(R_V1)(sp)
	sw      a0, CTX_OFFSET(R_A0)(sp)
	sw      a1, CTX_OFFSET(R_A1)(sp)
	sw      a2, CTX_OFFSET(R_A2)(sp)
	sw      a3, CTX_OFFSET(R_A3)(sp)
	sw      t0, CTX_OFFSET(R_T0)(sp)
	sw      t1, CTX_OFFSET(R_T1)(sp)
	sw      t2, CTX_OFFSET(R_T2)(sp)
	sw      t3, CTX_OFFSET(R_T3)(sp)
	sw      t4, CTX_OFFSET(R_T4)(sp)
	sw      t5, CTX_OFFSET(R_T5)(sp)
	sw      t6, CTX_OFFSET(R_T6)(sp)
	sw      t7, CTX_OFFSET(R_T7)(sp)
	sw      s0, CTX_OFFSET(R_S0)(sp)
	sw      s1, CTX_OFFSET(R_S1)(sp)
	sw      s2, CTX_OFFSET(R_S2)(sp)
	sw      s3, CTX_OFFSET(R_S3)(sp)
	sw      s4, CTX_OFFSET(R_S4)(sp)
	sw      s5, CTX_OFFSET(R_S5)(sp)
	sw      s6, CTX_OFFSET(R_S6)(sp)
	sw      s7, CTX_OFFSET(R_S7)(sp)
	sw      t8, CTX_OFFSET(R_T8)(sp)
	sw      t9, CTX_OFFSET(R_T9)(sp)
    sw      k0, CTX_OFFSET(R_K0)(sp)
    sw      k1, CTX_OFFSET(R_K1)(sp)
	sw      gp, CTX_OFFSET(R_GP)(sp)
    addiu   k0, sp, CTX_SIZE    
	sw      k0, CTX_OFFSET(R_SP)(sp)    /* k0 = SP */
	sw      fp, CTX_OFFSET(R_FP)(sp)
	sw      ra, CTX_OFFSET(R_RA)(sp)

#if __mips_hard_float
    mfc0	k1, C0_SR
    li      k0, SR_CU1
    and     k0, k0, k1
    beqz    k0, 1f
    nop
    SAVE_FPU                            /* save the FPU registers */
1:
#endif

    /* Save CP0 registers. */
	mflo	k0
	mfhi	k1
	sw	    k0, CTX_OFFSET(R_MDLO)(sp)
	sw	    k1, CTX_OFFSET(R_MDHI)(sp)

    mfc0	k0, C0_BADVADDR
    mfc0	k1, C0_CAUSE
    sw	    k0, CTX_OFFSET(R_BADVADDR)(sp)
    sw	    k1, CTX_OFFSET(R_CAUSE)(sp)

	mfc0	k0, C0_SR
    mfc0    k1, C0_EPC
	sw	    k0, CTX_OFFSET(R_SR)(sp)
	sw   	k1, CTX_OFFSET(R_EPC)(sp)
    nop
    .set    reorder
.endm

//-----------------------------------------------------------------------------

/* Restores a gp_ctx pointed to by a0.  Leaves interrupts disabled and
   C0_EPC ready to eret.  */

.macro RESTORE_CONTEXT
    .set    noreorder
    .set    noat
	/* Restore the general registers.  */
	lw   	AT, CTX_OFFSET(R_AT)(sp)
	lw   	v0, CTX_OFFSET(R_V0)(sp)
	lw   	v1, CTX_OFFSET(R_V1)(sp)
	lw   	a0, CTX_OFFSET(R_A0)(sp)
	lw   	a1, CTX_OFFSET(R_A1)(sp)
	lw   	a2, CTX_OFFSET(R_A2)(sp)
	lw   	a3, CTX_OFFSET(R_A3)(sp)
	lw   	t0, CTX_OFFSET(R_T0)(sp)
	lw   	t1, CTX_OFFSET(R_T1)(sp)
	lw   	t2, CTX_OFFSET(R_T2)(sp)
	lw   	t3, CTX_OFFSET(R_T3)(sp)
	lw   	t4, CTX_OFFSET(R_T4)(sp)
	lw   	t5, CTX_OFFSET(R_T5)(sp)
	lw   	t6, CTX_OFFSET(R_T6)(sp)
	lw   	t7, CTX_OFFSET(R_T7)(sp)
	lw   	s0, CTX_OFFSET(R_S0)(sp)
	lw   	s1, CTX_OFFSET(R_S1)(sp)
	lw   	s2, CTX_OFFSET(R_S2)(sp)
	lw   	s3, CTX_OFFSET(R_S3)(sp)
	lw   	s4, CTX_OFFSET(R_S4)(sp)
	lw   	s5, CTX_OFFSET(R_S5)(sp)
	lw   	s6, CTX_OFFSET(R_S6)(sp)
	lw   	s7, CTX_OFFSET(R_S7)(sp)
	lw   	t8, CTX_OFFSET(R_T8)(sp)
	lw   	t9, CTX_OFFSET(R_T9)(sp)
	lw   	gp, CTX_OFFSET(R_GP)(sp)
	lw   	fp, CTX_OFFSET(R_FP)(sp)
	lw   	ra, CTX_OFFSET(R_RA)(sp)

#if __mips_hard_float
	lw	    k1, CTX_OFFSET(R_SR)(sp)    /* Check the saved SR */
    li      k0, SR_CU1
    and     k0, k0, k1
    beqz    k0, 1f
    nop
    RESTORE_FPU                         /* load the FPU registers */
1:
#endif

	/* Restore CP0 registers, kernel registers and stack with
	   interrupts disabled.  */

	lw	    k0, CTX_OFFSET(R_MDLO)(sp)
	lw	    k1, CTX_OFFSET(R_MDHI)(sp)
	mtlo	k0
	mthi	k1

    lw	    k0, CTX_OFFSET(R_BADVADDR)(sp)
    lw	    k1, CTX_OFFSET(R_CAUSE)(sp)
    mtc0    k0, C0_BADVADDR
    mtc0    k1, C0_CAUSE

	.set mips32r2
	di
	.set    mips32

	lw   	k0, CTX_OFFSET(R_EPC)(sp)
    lw	    k1, CTX_OFFSET(R_SR)(sp)
	lw   	sp, CTX_OFFSET(R_SP)(sp)    /* Finally restore $29/sp. */

    mtc0    k0, C0_EPC
	mtc0	k1, C0_SR

	.set mips32r2
	ehb
	.set    mips32

//	lw   	k1, CTX_OFFSET(R_K1)(sp)
//	lw   	k0, CTX_OFFSET(R_K0)(sp)
    nop
    .set    reorder
.endm

#endif /* defined(__ASSEMBLER__) */

#endif /*__CONTEXT_H__*/


