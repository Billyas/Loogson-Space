/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 */
#ifndef __ARCH_CC_H__
#define __ARCH_CC_H__

#include <stdio.h>  /* printf, fflush, FILE */
#include <stdlib.h> /* abort */

#define LWIP_PROVIDE_ERRNO

/*
 * Define platform endianness (might already be defined)
 */
#define BYTE_ORDER  LITTLE_ENDIAN

/*
 * Define generic types used in lwIP
 */
typedef unsigned   char    u8_t;
typedef signed     char    s8_t;
typedef unsigned   short   u16_t;
typedef signed     short   s16_t;
typedef unsigned   int     u32_t;
typedef signed     int     s32_t;

typedef u32_t mem_ptr_t;
typedef u32_t sys_prot_t;

/*
 * Define (sn)printf formatters for these lwIP types
 */
#define U16_F "hu"
#define S16_F "hd"
#define X16_F "hx"
#define U32_F "lu"
#define S32_F "ld"
#define X32_F "lx"

/*
 * Compiler hints for packing structures
 */
#define PACK_STRUCT_BEGIN 
#define PACK_STRUCT_STRUCT   __attribute__ ((__packed__))
#define PACK_STRUCT_END
#define PACK_STRUCT_FIELD(x) x

/*
 * Plaform specific diagnostic output
 */
#if defined(OS_RTTHREAD)
#define LWIP_PLATFORM_DIAG(x)   do { rt_kprintf x; } while(0)

#define LWIP_PLATFORM_ASSERT(x) do { rt_kprintf("Assertion \"%s\" failed at line %d in %s\n", \
                                                 x, __LINE__, __FILE__); } while(0)
#else
#define LWIP_PLATFORM_DIAG(x)   do { printk x; } while(0)

#define LWIP_PLATFORM_ASSERT(x) do { printk("Assertion \"%s\" failed at line %d in %s\n", \
                                             x, __LINE__, __FILE__); } while(0)
#endif

#endif /* __ARCH_CC_H__ */


