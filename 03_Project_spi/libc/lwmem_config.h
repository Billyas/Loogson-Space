/**
 * \file            lwmem_config_default.h
 * \brief           LwMEM default config
 */

/*
 * Copyright (c) 2020 Tilen MAJERLE
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * This file is part of LwMEM - Lightweight dynamic memory manager library.
 *
 * Author:          Tilen MAJERLE <tilen@majerle.eu>
 * Version:         v1.3.0
 */
#ifndef LWMEM_HDR_CONFIG_H
#define LWMEM_HDR_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * \defgroup        LWMEM_CONFIG Configuration
 * \brief           Configuration for LwMEM library
 * \{
 */

/**
 * \brief           Enables `1` or disables `0` operating system support in the library
 *
 * \note            When `LWMEM_CFG_OS` is enabled, user must implement functions in \ref LWMEM_SYS group.
 */
#ifndef LWMEM_CFG_OS
  #if defined(OS_RTTHREAD) || defined(OS_FREERTOS) || defined(OS_UCOS)
    /*
     * TODO
     */
    #define LWMEM_CFG_OS                        0
  #else
    #define LWMEM_CFG_OS                        0
  #endif
#endif

/**
 * \brief           Mutex handle type
 *
 * \note            This value must be set in case \ref LWMEM_CFG_OS is set to `1`.
 *                  If data type is not known to compiler, include header file with
 *                  definition before you define handle type
 */
#ifndef LWMEM_CFG_OS_MUTEX_HANDLE
#define LWMEM_CFG_OS_MUTEX_HANDLE               void *
#endif

/**
 * \brief           Number of bits to align memory address and memory size
 *
 * Some CPUs do not offer unaligned memory access (Cortex-M0 as an example)
 * therefore it is important to have alignment of data addresses and potentialy length of data
 *
 * \note            This value must be a power of `2` for number of bytes.
 *                  Usually alignment of `4` bytes fits to all processors.
 */
#define LWMEM_CFG_ALIGN_NUM                     ((size_t)4)

/**
 * \}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* LWMEM_HDR_CONFIG_H */
