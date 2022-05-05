/*
 * Copyright (C) 2020-2021 Suzhou Tiancheng Software Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "regdef.h"
#include "cpu.h"
#include "asm.h"

#include "context.h"

    .text
    .align(2)
        
//-----------------------------------------------------------------------------
// 安装到: K0BASE+0x00, K0BASE+0x80, K0BASE+0x180 处 
//-----------------------------------------------------------------------------

    .extern real_exception_entry
    .global except_common_entry
    .type   except_common_entry, @function
except_common_entry:
    la      k0, real_exception_entry
    jr      k0
    nop
    nop
    nop

//-----------------------------------------------------------------------------
// 中断处理入口 
//-----------------------------------------------------------------------------

    .extern mips_irq_handle
    .extern mips_exception_handler
    .global real_exception_entry
    .type   real_exception_entry, @function
    .set    noreorder
real_exception_entry:
    la      k1, (0x1f << 2)
    mfc0    k0, C0_CAUSE
    and     k0, k0, k1
    bnez    k0, lbl_exec
    nop
    la      k0, mips_irq_handle          /* 中断 */
    jr      k0
    nop
lbl_exec:
    la      k0, mips_exception_handler   /* 例外 */
    jr      k0
    nop
    .set reorder

//-----------------------------------------------------------------------------
// 例外处理入口 
//-----------------------------------------------------------------------------

    .global mips_exception_handler
    .type   mips_exception_handler, @function
    .set    noreorder
mips_exception_handler:
    mfc0    t0, C0_STATUS               // disable interrupt
    and     t0, t0, 0xfffffffe
    mtc0    t0, C0_STATUS

    SAVE_CONTEXT

    /* Exception Dead */
    move    a0, sp
    la      k0, c_exception_handler
    jr      k0
    nop
    .set reorder

//-----------------------------------------------------------------------------


