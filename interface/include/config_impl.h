/*
 * Copyright (c) 2021-2022, Arm Limited. All rights reserved.
 * Copyright (c) 2024, Cypress Semiconductor Corporation (an Infineon company)
 * or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
/***********  WARNING: This is an auto-generated file. Do not edit!  ***********/

#ifndef __CONFIG_IMPL_H__
#define __CONFIG_IMPL_H__

#include "config_tfm.h"

/* Backends */
#if CONFIG_TFM_SPM_BACKEND_IPC == 1
#define CONFIG_TFM_SPM_BACKEND_IPC                               1
#define CONFIG_TFM_SPM_BACKEND_SFN                               0
#else
#define CONFIG_TFM_SPM_BACKEND_IPC                               0
#define CONFIG_TFM_SPM_BACKEND_SFN                               1
#endif

#define CONFIG_TFM_CONNECTION_BASED_SERVICE_API                  1
#define CONFIG_TFM_MMIO_REGION_ENABLE                            1
#define CONFIG_TFM_FLIH_API                                      1
#define CONFIG_TFM_SLIH_API                                      1

#if CONFIG_TFM_SPM_BACKEND_IPC == 1
/* Trustzone NS agent working stack size. */
#if defined(TFM_FIH_PROFILE_ON) && TFM_ISOLATION_LEVEL == 1
#define CONFIG_TFM_NS_AGENT_TZ_STACK_SIZE                        1768 //NXP adds 512 bytes
#else
#define CONFIG_TFM_NS_AGENT_TZ_STACK_SIZE                        1536 //NXP adds 512 bytes
#endif

#if !defined CONFIG_TFM_USE_TRUSTZONE
/* SPM has to have its own stack if Trustzone isn't present. */
#if defined(TFM_FIH_PROFILE_ON)
#define CONFIG_TFM_SPM_THREAD_STACK_SIZE                          1536
#else
#define CONFIG_TFM_SPM_THREAD_STACK_SIZE                          1024
#endif
#endif

#elif CONFIG_TFM_SPM_BACKEND_SFN == 1

/* Helper toggles */
#if TFM_S_REG_TEST
#define TFM_REGRESSION_STACK_SIZE \
        (0x0D00 + 0x0220 + \
         0x0500 + 0x0300 + \
         0x0400 + 0x0400 + 0x0400 + \
         0x0200)
#else
#define TFM_REGRESSION_STACK_SIZE  0            /* nothing */
#endif

#ifdef TFM_PARTITION_NS_AGENT_MAILBOX
#define ADD_NS_AGENT_MAILBOX_STACK()            + NS_AGENT_MAILBOX_STACK_SIZE
#else
#define ADD_NS_AGENT_MAILBOX_STACK()            /* nothing */
#endif

#ifdef TFM_PARTITION_PROTECTED_STORAGE
#define ADD_PS_STACK()                          + PS_STACK_SIZE
#else
#define ADD_PS_STACK()                          /* nothing */
#endif

#ifdef TFM_PARTITION_INITIAL_ATTESTATION
#define ADD_INIT_ATTEST_STACK()                 + ATTEST_STACK_SIZE
#else
#define ADD_INIT_ATTEST_STACK()                 /* nothing */
#endif

/*
 * In isolation level 1 SFN model, all subsequent components work on NS agent
 * stack. It is observed that half of the sum of all partition stack sizes is
 * enough for working. Define a divisor factor
 * CONFIG_TFM_NS_AGENT_TZ_STK_SIZE_SHIFT_FACTOR for reference, and allow
 * modification of the factor based on application situation. The stack size
 * value is aligned to 8 bytes.
 * The minimum value is 0x400 to satisfy the SPM functional requirement.
 * Manifest tool will assure this.
 */
#define CONFIG_TFM_TOTAL_STACK_SIZE (                  \
      0                                                \
    ADD_NS_AGENT_MAILBOX_STACK()                       \
    + ITS_STACK_SIZE                                   \
    + CRYPTO_STACK_SIZE                                \
    + PLATFORM_SP_STACK_SIZE                           \
    ADD_PS_STACK()                                     \
    ADD_INIT_ATTEST_STACK()                            \
    + TFM_REGRESSION_STACK_SIZE                        \
    + 0x0300                                           \
)

#if (CONFIG_TFM_TOTAL_STACK_SIZE < 2048)
#undef CONFIG_TFM_TOTAL_STACK_SIZE                             
#define CONFIG_TFM_TOTAL_STACK_SIZE                              2048
#endif

#define CONFIG_TFM_NS_AGENT_TZ_STK_SIZE_SHIFT_FACTOR             1
#define CONFIG_TFM_NS_AGENT_TZ_STACK_SIZE                         \
    (((CONFIG_TFM_TOTAL_STACK_SIZE >> CONFIG_TFM_NS_AGENT_TZ_STK_SIZE_SHIFT_FACTOR) + 0x7) & (~0x7))

#endif /* CONFIG_TFM_SPM_BACKEND_IPC == 1 */

/* Define whether ARoT partitions are present. Can be used when applying protections. */
#define CONFIG_TFM_AROT_PRESENT                                  1

#endif /* __CONFIG_IMPL_H__ */
