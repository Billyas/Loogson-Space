/*
 * Copyright (C) 2020-2021 Suzhou Tiancheng Software Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * stack.c
 *
 * created: 2020/4/20
 * authour: Bian
 */
 
#include "rtthread.h"
#include "rthw.h"

#include "cpu.h"
#include "mips.h"

#include "context.h"

//-----------------------------------------------------------------------------

register unsigned int $GP __asm__ ("$28");

rt_uint8_t *rt_hw_stack_init(void *tentry, void *parameter, rt_uint8_t *stack_addr, void *texit)
{
    static unsigned int wSR = 0;
    static unsigned int wGP;
    unsigned int *stk, cause = 0;

    if (wSR == 0)
    {
        mips_get_sr(wSR);
    	wSR &= 0xfffffffe;                  /* mips32 SR_IMASK */
#if __mips_hard_float
        wSR |= SR_CU1 | SR_IE;              /* FPU enable */
#else
    	wSR |= SR_IE;                       /* mips32 SR_IE */
#endif
        wGP = $GP;
    }
    
    /* Get stack aligned */
#ifndef ARCH_CPU_STACK_GROWS_UPWARD
    stk = (unsigned int *)RT_ALIGN_DOWN((unsigned int)stack_addr, 8);
    stk -= CTX_SIZE / 4;
#else
#error "TODO here."
#endif
    
    memset((void *)stk, 0, CTX_SIZE);

    *(stk + R_SP   ) = (unsigned int)stk;
    *(stk + R_A0   ) = (unsigned int)parameter;
    *(stk + R_RA   ) = (unsigned int)texit;  

    *(stk + R_GP   ) = wGP;
    *(stk + R_SR   ) = wSR;
    *(stk + R_CAUSE) = cause;
    *(stk + R_EPC  ) = (unsigned int)tentry;

    return (rt_uint8_t *)stk;
}

//-----------------------------------------------------------------------------

