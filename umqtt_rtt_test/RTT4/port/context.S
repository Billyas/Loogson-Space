/*
 * Copyright (C) 2020-2021 Suzhou Tiancheng Software Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * context.S
 *
 * created: 2020/4/20
 *  author: Bian
 */

#include "regdef.h"
#include "cpu.h"
#include "asm.h"

#include "context.h"
    
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

    .text
    .set noreorder

//-----------------------------------------------------------------------------
// function: void rt_hw_context_switch(rt_uint32 from, rt_uint32 to)
// param:    a0 --> from
//           a1 --> to
//-----------------------------------------------------------------------------

    .globl rt_hw_context_switch
rt_hw_context_switch:
    mtc0    ra, C0_EPC
    SAVE_CONTEXT
    
    sw      sp, 0(a0)       /* store sp in preempted tasks TCB */
    lw      sp, 0(a1)       /* get new task stack pointer */

    RESTORE_CONTEXT
    eret
    nop

//-----------------------------------------------------------------------------
// function: void rt_hw_context_switch_to(rt_uint32 to)
// params:   a0 --> to
//-----------------------------------------------------------------------------
    .globl rt_hw_context_switch_to
rt_hw_context_switch_to:
    lw      sp, 0(a0)       /* get new task stack pointer */
    
    RESTORE_CONTEXT
    eret
    nop

//-----------------------------------------------------------------------------

    .data

rt_thread_switch_interrupt_flag:
    .word   0
rt_interrupt_from_thread:
    .word   0
rt_interrupt_to_thread:
    .word   0

//-----------------------------------------------------------------------------

    .text

//-----------------------------------------------------------------------------
// function: void rt_hw_context_switch_interrupt(rt_uint32 from, rt_uint32 to) 
// params: 
//-----------------------------------------------------------------------------

    .globl rt_hw_context_switch_interrupt
rt_hw_context_switch_interrupt:
    .set noreorder

    la      t0, rt_thread_switch_interrupt_flag
    lw      t1, 0(t0)
    nop
    bnez    t1, _reswitch
    nop
    li      t1, 0x01                       /* set rt_thread_switch_interrupt_flag to 1 */
    sw      t1, 0(t0)
    la      t0, rt_interrupt_from_thread   /* set rt_interrupt_from_thread */
    sw      a0, 0(t0)
_reswitch:
    la      t0, rt_interrupt_to_thread     /* set rt_interrupt_to_thread */
    sw      a1, 0(t0)
    jr      ra
    nop

//-----------------------------------------------------------------------------
// function: void rt_hw_context_switch_interrupt_do(rt_base_t flag)
//-----------------------------------------------------------------------------

    .extern rt_interrupt_enter
    .extern rt_interrupt_leave
    .extern c_interrupt_handler
    .globl mips_irq_handle
mips_irq_handle:
    SAVE_CONTEXT

    .set noreorder

    /* let k0 keep the current context sp */
    move    k0, sp
    /* switch to kernel stack */
    la      sp, _system_stack

    jal     rt_interrupt_enter
    nop
    /* Get Old SP from k0 as paremeter in a0 */
    move    a0, k0
    jal     c_interrupt_handler
    nop
    jal     rt_interrupt_leave
    nop

    /* switch sp back to thread context */
    move    sp, k0

    /*
    * if rt_thread_switch_interrupt_flag set, jump to
    * rt_hw_context_switch_interrupt_do and do not return
    */
    la      k0, rt_thread_switch_interrupt_flag
    lw      k1, 0(k0)
    beqz    k1, spurious_interrupt
    nop
    sw      zero, 0(k0)                     /* clear flag */
    nop

    /*
    * switch to the new thread
    */
    la      k0, rt_interrupt_from_thread
    lw      k1, 0(k0)
    nop
    sw      sp, 0(k1)                       /* store sp in preempted task TCB */

    la      k0, rt_interrupt_to_thread
    lw      k1, 0(k0)
    nop
    lw      sp, 0(k1)                       /* get new task stack pointer */
    j       spurious_interrupt
    nop

spurious_interrupt:
    RESTORE_CONTEXT
    eret
    nop
    
    .set reorder
    
//-----------------------------------------------------------------------------

