/*
 * Copyright (c) 2018-2020 Arm Limited
 * Copyright 2019-2024 NXP
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __TARGET_CFG_H__
#define __TARGET_CFG_H__

#include "target_cfg_common.h"

#ifdef __cplusplus
extern "C" {
#endif

#define TFM_DRIVER_STDIO    (Driver_USART)
#define NS_DRIVER_STDIO     (Driver_USART)

/* Predefined NOR flash config options (Macronix Octal SPI Flash MX25U51245G flash, connected to FlexSPI Port A)
 * [UM11147, Table 1007]
 */
#define NOR_FLASH_INSTANCE      (0U)            /* This is the instance number of FLEXSPI, only support 0 */
#define NOR_FLASH_OPTION0       (0xC0000008U)
#define NOR_FLASH_OPTION1       (0x00000000U)
#define FLASH_NEED_RESET        (1)

#define FLEXSPI_FLASH_CTX       (RF_SYSCON->RW_SCRATCH_REG3)

#define SEC_VIO_IRQn            SECUREVIOLATION_IRQn       /* Security Violation Interrupt number, synonim */
#define SEC_VIO_IRQHandler      SECUREVIOLATION_IRQHandler /* Security Violation Interrupt handler, synonim */

/**
 * \brief defines the enum values for secure access levels
 */
typedef enum _TFM_SECURE_ACCESS_LEVEL_ATTRIBUTE_TYPE
{
    TFM_SEC_ACCESS_LVL_USER_NS  = 0x0U,        /* User non-secure access allowed.      */
    TFM_SEC_ACCESS_LVL_PRIV_NS  = 0x1U,        /* Privilege non-secure access allowed. */
    TFM_SEC_ACCESS_LVL_USER_S   = 0x2U,        /* User secure access allowed.          */
    TFM_SEC_ACCESS_LVL_PRIV_S   = 0x3U,        /* Privilege secure access allowed.     */
}TFM_SECURE_ACCESS_LEVEL_ATTRIBUTE_TYPE;

/**
 * \brief Holds the data necessary to do isolation for a specific peripheral.
 */
struct platform_data_t
{
    uint32_t periph_start;
    uint32_t periph_limit;
    volatile uint32_t *periph_ppc_bank; /* Secure control register address */
    uint32_t periph_ppc_loc;            /* Position in the secure control register */
};

#ifdef __cplusplus
}
#endif

#endif /* __TARGET_CFG_H__ */
