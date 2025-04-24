/*
 * Copyright (c) 2018-2020 Arm Limited
 * Copyright 2019-2023, 2025 NXP
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

#define TFM_DRIVER_STDIO    Driver_LPUART
#define NS_DRIVER_STDIO     Driver_LPUART

#define TRDC_ACCESS_CONTROL_POLICY_SEC_PRIV_INDEX    0
#define TRDC_ACCESS_CONTROL_POLICY_SEC_INDEX    1
#define TRDC_ACCESS_CONTROL_POLICY_ALL_INDEX    2
#define TRDC_ACCESS_CONTROL_POLICY_NS_INDEX     3
  
#define TRDC_PROCESSOR_MASTER_DOMAIN_ID         0
  
#define TRDC_MBC_FLASH                          0
#define TRDC_MBC_FLASH_SLAVE_PORT               0
#define TRDC_FLASH_SUBREGION_SIZE              (0x8000)     /* 32 kB */
  
#define TRDC_MBC_TCM                           1
#define TRDC_MBC_CTCM_SLAVE_PORT               0
#define TRDC_CTCM_SUBREGION_SIZE              (0x2000)     /* 8 KB */
  
#define TRDC_MBC_STCM_SLAVE_PORT               1
#define TRDC_MBC_STCM_3_4_SLAVE_PORT            2
#define TRDC_STCM_SUBREGION_SIZE              (0x2000)    /* 8KB */
#define TRDC_MBC_STCM5_SLAVE_PORT              3
#define TRDC_STCM5_SUBREGION_SIZE              (0x1000)    /* 4KB */

#define TRDC_STCM_SLV1_START                   0x20000000
#define TRDC_STCM_SLV1_END                     0x2000FFFF
#define TRDC_STCM_SLV2_START                   0x20010000
#define TRDC_STCM_SLV2_END                     0x20019FFF
#define TRDC_STCM_SLV3_START                   0x2001A000
#define TRDC_STCM_SLV3_END                     0x2001BFFF
  
  
#define TRDC_MBC_PERIPHERALS                    2
  
//#define SEC_VIO_IRQn            SECUREVIOLATION_IRQn /* Security Violation Interrupt number synonim */
/**
 * \brief Holds the data necessary to do isolation for a specific peripheral.
 */
struct platform_data_t
{
    uint32_t periph_start;
    uint32_t periph_limit;
    
    uint32_t nseEnable;
    uint32_t mbcIdx;         /*!< The index of the MBC for this configuration to take effect. */
    uint32_t slaveMemoryIdx; /*!< The index of the slave memory for this configuration to take effect. */
    uint32_t memoryBlockIdx; /*!< The index of the memory block for this configuration to take effect. */
};

void ppc_configure_to_secure(struct platform_data_t *platform_data, bool privileged);

#ifdef __cplusplus
}
#endif

#endif /* __TARGET_CFG_H__ */
