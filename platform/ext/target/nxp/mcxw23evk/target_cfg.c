/*
 * Copyright (c) 2018-2022 Arm Limited. All rights reserved.
 * Copyright 2025 NXP
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

#include "target_cfg.h"
#include "Driver_Common.h"
#include "platform_description.h"
#include "device_definition.h"
#include "region_defs.h"
#include "tfm_plat_defs.h"
#include "utilities.h"
#include "tfm_spm_log.h"

extern const struct memory_region_limits memory_regions;

struct platform_data_t tfm_peripheral_std_uart = {
        USART0_BASE_NS,
        USART0_BASE_NS + 0xFFF,
        0,
        0
};

struct platform_data_t tfm_peripheral_timer0 = {
        CTIMER2_BASE,
        CTIMER2_BASE + 0xFFF,
        &(AHB_SECURE_CTRL->SEC_CTRL_APB_BRIDGE[0].SEC_CTRL_APB_BRIDGE1_MEM_CTRL1),
        AHB_SECURE_CTRL_SEC_CTRL_APB_BRIDGE1_MEM_CTRL1_CTIMER2_RULE_SHIFT
};

/*------------------- SAU/IDAU configuration functions -----------------------*/
// Define secure-only peripheral region

/* Define Peripherals NS address range for the platform */
#ifndef PERIPHERALS_BASE_NS_START
#define PERIPHERALS_BASE_NS_START (0x40000000)
#endif
#ifndef PERIPHERALS_BASE_NS_END
#define PERIPHERALS_BASE_NS_END   (0x4FFFFFFF)
#endif

#define PERIPHERALS_FLASH_CTRL_START      (0x40034000)
#define PERIPHERALS_FLASH_CTRL_END        (0x40034FFF)

void sau_and_idau_cfg(void)
{
    /* Ensure all memory accesses are completed */
    __DMB();
    
    /* Enables SAU */
    //TZ_SAU_Enable();

    /* Enables SAU Control register: Enable SAU and All Secure (applied only if disabled) */
    SECURE_WRITE_REGISTER(&(SAU->CTRL), ((1U << SAU_CTRL_ENABLE_Pos) & SAU_CTRL_ENABLE_Msk));
    
    /* Configures SAU regions to be non-secure */
    SECURE_WRITE_REGISTER(&(SAU->RNR), 0U);
    SAU->RBAR = (memory_regions.non_secure_partition_base
                & SAU_RBAR_BADDR_Msk);
    SAU->RLAR = (memory_regions.non_secure_partition_limit
                & SAU_RLAR_LADDR_Msk)
                | SAU_RLAR_ENABLE_Msk;

    /* Configures Non secure data start region */
    SECURE_WRITE_REGISTER(&(SAU->RNR), 1U);
    SECURE_WRITE_REGISTER(&(SAU->RBAR), (NS_DATA_START & SAU_RBAR_BADDR_Msk));
    SECURE_WRITE_REGISTER(&(SAU->RLAR), ((NS_DATA_LIMIT & SAU_RLAR_LADDR_Msk) | SAU_RLAR_ENABLE_Msk));
    
    /* Configures veneers region to be non-secure callable */
    SECURE_WRITE_REGISTER(&(SAU->RNR), 2U);
    SAU->RBAR = (memory_regions.veneer_base  & SAU_RBAR_BADDR_Msk);
    SAU->RLAR = (memory_regions.veneer_limit & SAU_RLAR_LADDR_Msk)
                | SAU_RLAR_ENABLE_Msk
                | SAU_RLAR_NSC_Msk;

    /* Configure the peripherals space */
    SECURE_WRITE_REGISTER(&(SAU->RNR), 3U);
    SECURE_WRITE_REGISTER(&(SAU->RBAR), (PERIPHERALS_BASE_NS_START & SAU_RBAR_BADDR_Msk));
    SECURE_WRITE_REGISTER(&(SAU->RLAR), (((PERIPHERALS_FLASH_CTRL_START - 1) & SAU_RLAR_LADDR_Msk) 
                                         | SAU_RLAR_ENABLE_Msk));
    
    /* Configure the peripherals space after the flash controller */
    SAU->RNR  = 4U;
    SAU->RBAR = ((PERIPHERALS_FLASH_CTRL_END + 1) & SAU_RBAR_BADDR_Msk);
    SAU->RLAR = (PERIPHERALS_BASE_NS_END & SAU_RLAR_LADDR_Msk)
                | SAU_RLAR_ENABLE_Msk;

#ifdef BL2
    /* Secondary image partition */
    SECURE_WRITE_REGISTER(&(SAU->RNR), 4U);
    SAU->RBAR = (memory_regions.secondary_partition_base  & SAU_RBAR_BADDR_Msk);
    SAU->RLAR = (memory_regions.secondary_partition_limit & SAU_RLAR_LADDR_Msk)
                | SAU_RLAR_ENABLE_Msk;
#endif /* BL2 */

#ifdef TFM_WIFI_FLASH_REGION
    /* Wifi Flash region */
    SECURE_WRITE_REGISTER(&(SAU->RNR), 5U);
    SAU->RBAR = (memory_regions.wifi_flash_region_base & SAU_RBAR_BADDR_Msk);
    SAU->RLAR = (memory_regions.wifi_flash_region_limit & SAU_RLAR_LADDR_Msk)
	            | SAU_RLAR_ENABLE_Msk;
#endif /* TFM_WIFI_FLASH_REGION */

#ifdef TFM_EL2GO_DATA_IMPORT_REGION
    /* EL2GO data import region */
    SECURE_WRITE_REGISTER(&(SAU->RNR), 6U);
    SAU->RBAR = (memory_regions.el2go_data_import_region_base & SAU_RBAR_BADDR_Msk);
    SAU->RLAR = (memory_regions.el2go_data_import_region_limit & SAU_RLAR_LADDR_Msk)
	           | SAU_RLAR_ENABLE_Msk;
#endif /* TFM_EL2GO_DATA_IMPORT_REGION */

    /* Ensure the write is completed and flush pipeline */
    __DSB();
    __ISB();
}

/*------------------- Memory configuration functions -------------------------*/

int32_t mpc_init_cfg(void)
{
    uint32_t ns_region_id = 0;
    uint32_t ns_region_start_id = 0;
    uint32_t ns_region_end_id = 0;

    /*
    * Starts changing actual configuration so issue DMB to ensure every
    * transaction has completed by now
    */
    __DMB();

    /* Configuration of AHB Secure Controller
     * Possible values for every memory sector or peripheral rule:
     *  0    Non-secure, user access allowed.
     *  1    Non-secure, privileged access allowed.
     *  2    Secure, user access allowed.
     *  3    Secure, privileged access allowed. */

    /* == Flash region == */

    /* The regions have to be alligned to 64 kB to cover the AHB Flash Region. */
    assert((memory_regions.non_secure_partition_base % FLASH_SUBREGION_SIZE) == 0);
    assert(((memory_regions.non_secure_partition_limit+1) % FLASH_SUBREGION_SIZE) == 0);

    /* Flash region is divided into 8 sub-regions (sector). Each flash sub-regions (sector) is 64 kbytes. */
    /* 1) Set FLASH memory security access rule configuration to init value (0x3 = all regions set to secure and privileged user access) */
    AHB_SECURE_CTRL->SEC_CTRL_FLASH_ROM[0].SEC_CTRL_FLASH_MEM_RULE[0] = 0x33333333U;
    AHB_SECURE_CTRL->SEC_CTRL_FLASH_ROM[0].SEC_CTRL_FLASH_MEM_RULE[1] = 0x33333333U;
    AHB_SECURE_CTRL->SEC_CTRL_FLASH_ROM[0].SEC_CTRL_FLASH_MEM_RULE[2] = 0x33333333U;

    /* 2) Set FLASH memory security access rule configuration (set to non-secure and non-privileged user access allowed).*/
    ns_region_start_id = memory_regions.non_secure_partition_base/FLASH_SUBREGION_SIZE;
    ns_region_end_id = (memory_regions.non_secure_partition_limit+1)/FLASH_SUBREGION_SIZE;

    /* Set to non-secure and non-privileged user access allowed */
    for(ns_region_id = ns_region_start_id; ns_region_id < ns_region_end_id; ns_region_id++)
    {
        if(ns_region_id < 8)
        {
            /* Set regions the AHB controller for flash memory 0x0000_0000 - 0x0004_0000 */
            AHB_SECURE_CTRL->SEC_CTRL_FLASH_ROM[0].SEC_CTRL_FLASH_MEM_RULE[0] &= ~(0xF << (ns_region_id*4));
        }
        else if((ns_region_id >= 8) && (ns_region_id < 16))
        {
            /* Set regions in the AHB controller for flash memory 0x0004_0000 - 0x0008_0000 */
            AHB_SECURE_CTRL->SEC_CTRL_FLASH_ROM[0].SEC_CTRL_FLASH_MEM_RULE[1] &= ~(0xF << ((ns_region_id-8)*4));
        }else if((ns_region_id >= 16) && (ns_region_id < 24))
        {
            /* Set regions the AHB controller for flash memory 0x0008_0000 - 0x0009_8000 */
            AHB_SECURE_CTRL->SEC_CTRL_FLASH_ROM[0].SEC_CTRL_FLASH_MEM_RULE[2] &= ~(0xF << ((ns_region_id-16)*4));
        }
    }

#ifdef BL2 /* Set secondary image region to NS, when BL2 is enabled */
    /* The regions have to be alligned to 32 kB to cover the AHB Flash Region. */
    assert((memory_regions.secondary_partition_base % FLASH_SUBREGION_SIZE) == 0);
    assert(((memory_regions.secondary_partition_limit+1) % FLASH_SUBREGION_SIZE) == 0);

    /* 2) Set FLASH memory security access rule configuration (set to non-secure and non-privileged user access allowed).*/
    ns_region_start_id = memory_regions.secondary_partition_base/FLASH_SUBREGION_SIZE;
    ns_region_end_id = (memory_regions.secondary_partition_limit+1)/FLASH_SUBREGION_SIZE;

    /* Set to non-secure and non-privileged user access allowed */
    for(ns_region_id = ns_region_start_id; ns_region_id < ns_region_end_id; ns_region_id++)
    {
        if(ns_region_id < 8)
        {
            /* Set regions the AHB controller for flash memory 0x0000_0000 - 0x0004_0000 */
            AHB_SECURE_CTRL->SEC_CTRL_FLASH_ROM[0].SEC_CTRL_FLASH_MEM_RULE[0] &= ~(0xF << (ns_region_id*4));
        }
        else if((ns_region_id >= 8) && (ns_region_id < 16))
        {
            /* Set regions in the AHB controller for flash memory 0x0004_0000 - 0x0008_0000 */
            AHB_SECURE_CTRL->SEC_CTRL_FLASH_ROM[0].SEC_CTRL_FLASH_MEM_RULE[1] &= ~(0xF << ((ns_region_id-8)*4));
        }else if((ns_region_id >= 16) && (ns_region_id < 24))
        {
            /* Set regions the AHB controller for flash memory 0x0008_0000 - 0x0009_8000 */
            AHB_SECURE_CTRL->SEC_CTRL_FLASH_ROM[0].SEC_CTRL_FLASH_MEM_RULE[2] &= ~(0xF << ((ns_region_id-16)*4));
        }
    }
#endif

    /* == ROM region == */

    /* Each ROM sector is 4 kbytes. There are 32 ROM sectors in total. */
    /* Security control ROM memory configuration (0x3 = all regions set to secure and privileged user access). */
    AHB_SECURE_CTRL->SEC_CTRL_FLASH_ROM[0].SEC_CTRL_ROM_MEM_RULE[0] = 0x33333333U;
    AHB_SECURE_CTRL->SEC_CTRL_FLASH_ROM[0].SEC_CTRL_ROM_MEM_RULE[1] = 0x33333333U;
    AHB_SECURE_CTRL->SEC_CTRL_FLASH_ROM[0].SEC_CTRL_ROM_MEM_RULE[2] = 0x33333333U;
    AHB_SECURE_CTRL->SEC_CTRL_FLASH_ROM[0].SEC_CTRL_ROM_MEM_RULE[3] = 0x33333333U;

    /* == RAMX region == */

    /* Each RAMX sub region is 4 kbytes.*/
    /* Security access rules for RAMX (0x3 = all regions set to secure and privileged user access). */
    AHB_SECURE_CTRL->SEC_CTRL_RAMX[0].MEM_RULE[0]= 0x33333333U; /* 0x0400_0000 - 0x0400_7FFF */

    /* == SRAM region == */

    /* The regions have to be alligned to 4 kB to cover the AHB RAM Region */
    assert((S_DATA_SIZE % DATA_SUBREGION_SIZE) == 0);
    assert(((S_DATA_SIZE + NS_DATA_SIZE) % DATA_SUBREGION_SIZE) == 0);

    /* Security access rules for RAM (0x3 = all regions set to secure and privileged user access*/
    AHB_SECURE_CTRL->SEC_CTRL_RAM0[0].MEM_RULE[0]= 0x00003333U; /* 0x2000_4000 - 0x2000_7FFF */
    AHB_SECURE_CTRL->SEC_CTRL_RAM1[0].MEM_RULE[0]= 0x33333333U; /* 0x2000_8000 - 0x2000_FFFF */
    AHB_SECURE_CTRL->SEC_CTRL_RAM2[0].MEM_RULE[0]= 0x33333333U; /* 0x2001_0000 - 0x2001_7FFF */
    AHB_SECURE_CTRL->SEC_CTRL_RAM2[0].MEM_RULE[1]= 0x33333333U; /* 0x2001_8000 - 0x2001_FFFF */

    /* RAM memory configuration (set according to region_defs.h and flash_layout.h) */
    ns_region_start_id = S_DATA_SIZE/DATA_SUBREGION_SIZE; /* NS starts after S */
    ns_region_end_id = ((S_DATA_SIZE + NS_DATA_SIZE)/DATA_SUBREGION_SIZE) + 1;
    
    for(ns_region_id = ns_region_start_id; ns_region_id < ns_region_end_id; ns_region_id++)
    {
        /* Set regions the AHB controller for ram memory 0x2000_4000 - 0x2000_7FFF */
        if(ns_region_id < 4) 
        {
            AHB_SECURE_CTRL->SEC_CTRL_RAM0[0].MEM_RULE[0] &= ~(0xF << (ns_region_id*4));
        }
        /* Set regions the AHB controller for ram memory 0x2000_8000 - 0x2000_FFFF */
        else if((ns_region_id >= 4) && (ns_region_id < 12)) 
        {
                AHB_SECURE_CTRL->SEC_CTRL_RAM1[0].MEM_RULE[0] &= ~(0xF << ((ns_region_id-4)*4));
        }

        /* Set regions the AHB controller for ram memory 0x2001_0000 - 0x2001_7FFF */
        else if((ns_region_id >= 12) && (ns_region_id < 20)) 
        {
                AHB_SECURE_CTRL->SEC_CTRL_RAM2[0].MEM_RULE[0] &= ~(0xF << ((ns_region_id-12)*4));
        }

        /* Set regions the AHB controller for ram memory 0x2001_8000 - 0x2001_FFFF */
        else if((ns_region_id >= 20) && (ns_region_id < 28)) 
        {
                AHB_SECURE_CTRL->SEC_CTRL_RAM2[0].MEM_RULE[1] &= ~(0xF << ((ns_region_id-20)*4));
        }
    }
    

    for (int i=0 ; i< AHB_SECURE_CTRL_SEC_CTRL_SPIFI_SEC_CTRL_SPIFI_MEM_RULE_COUNT ; i++)
    {
         AHB_SECURE_CTRL->SEC_CTRL_SPIFI[0].MEM_RULE[i] = 
             AHB_SECURE_CTRL_SEC_CTRL_SPIFI_SEC_CTRL_SPIFI_MEM_RULE_MEM_RULE_RULE0(0x0U) |              /* Address space: 0x2800_0000 - 0x283F_FFFF */
             AHB_SECURE_CTRL_SEC_CTRL_SPIFI_SEC_CTRL_SPIFI_MEM_RULE_MEM_RULE_RULE1(0x0U) |              /* Address space: 0x2840_0000 - 0x287F_FFFF */
             AHB_SECURE_CTRL_SEC_CTRL_SPIFI_SEC_CTRL_SPIFI_MEM_RULE_MEM_RULE_RULE2(0x0U) |              /* Address space: 0x2880_0000 - 0x28BF_FFFF */
             AHB_SECURE_CTRL_SEC_CTRL_SPIFI_SEC_CTRL_SPIFI_MEM_RULE_MEM_RULE_RULE3(0x0U) |              /* Address space: 0x28C0_0000 - 0x28FF_FFFF */
             AHB_SECURE_CTRL_SEC_CTRL_SPIFI_SEC_CTRL_SPIFI_MEM_RULE_MEM_RULE_RULE4(0x0U) |              /* Address space: 0x2900_0000 - 0x293F_FFFF */
             AHB_SECURE_CTRL_SEC_CTRL_SPIFI_SEC_CTRL_SPIFI_MEM_RULE_MEM_RULE_RULE5(0x0U) |              /* Address space: 0x2940_0000 - 0x297F_FFFF */
             AHB_SECURE_CTRL_SEC_CTRL_SPIFI_SEC_CTRL_SPIFI_MEM_RULE_MEM_RULE_RULE6(0x0U) |              /* Address space: 0x2980_0000 - 0x29BF_FFFF */
             AHB_SECURE_CTRL_SEC_CTRL_SPIFI_SEC_CTRL_SPIFI_MEM_RULE_MEM_RULE_RULE7(0x0U);               /* Address space: 0x29C0_0000 - 0x29FF_FFFF */
    }   



#if TARGET_DEBUG_LOG
    SPMLOG_DBGMSG("=== [AHB MPC NS] =======\r\n");
    SPMLOG_DBGMSGVAL("NS ROM starts from : ",
                                      memory_regions.non_secure_partition_base);
    SPMLOG_DBGMSGVAL("NS ROM ends at : ",
                                      memory_regions.non_secure_partition_base +
                                     memory_regions.non_secure_partition_limit);
    SPMLOG_DBGMSGVAL("NS DATA start from : ", NS_DATA_START);
    SPMLOG_DBGMSGVAL("NS DATA ends at : ", NS_DATA_START + NS_DATA_LIMIT);
#endif

    /* Add barriers to assure the MPC configuration is done before continue
     * the execution.
     */
    __DSB();
    __ISB();

    return ARM_DRIVER_OK;
}

/*---------------------- PPC configuration functions -------------------------*/

int32_t ppc_init_cfg(void)
{
     /* Configuration of AHB Secure Controller. Grant user access to peripherals.
     * Possible values for every memory sector or peripheral rule:
     *  0    Non-secure, user access allowed.
     *  1    Non-secure, privileged access allowed.
     *  2    Secure, user access allowed.
     *  3    Secure, privileged access allowed. */
    
    /* Write access attributes for AHB_SECURE_CTRL module are tier-4 (secure privileged). */ 
    
    /* Security access rules for APB Bridge 0 peripherals. */
    AHB_SECURE_CTRL->SEC_CTRL_APB_BRIDGE[0].SEC_CTRL_APB_BRIDGE0_MEM_CTRL0 =
        (0x30000000U) |                                                                     /* Bits have to be set to '1' according to UM.*/
        AHB_SECURE_CTRL_SEC_CTRL_APB_BRIDGE0_MEM_CTRL0_SYSCON_RULE(0x0U) |                  /* System configuration */
        AHB_SECURE_CTRL_SEC_CTRL_APB_BRIDGE0_MEM_CTRL0_IOCON_RULE(0x0U) |                   /* I/O configuration */
        AHB_SECURE_CTRL_SEC_CTRL_APB_BRIDGE0_MEM_CTRL0_GINT0_RULE(0x0U) |                   /* GPIO input Interrupt 0 */
        AHB_SECURE_CTRL_SEC_CTRL_APB_BRIDGE0_MEM_CTRL0_PINT_RULE(0x0U) |                    /* Pin Interrupt and Pattern match */
        AHB_SECURE_CTRL_SEC_CTRL_APB_BRIDGE0_MEM_CTRL0_SEC_PINT_RULE(0x0U) |                /* Secure Pin Interrupt and Pattern match */
        AHB_SECURE_CTRL_SEC_CTRL_APB_BRIDGE0_MEM_CTRL0_INPUTMUX_RULE(0x0U);                 /* Peripheral Input Multiplexing. */

    AHB_SECURE_CTRL->SEC_CTRL_APB_BRIDGE[0].SEC_CTRL_APB_BRIDGE0_MEM_CTRL1 =
        (0x30003300U) |                                                                     /* Bits have to be set to '1' according to UM.*/
        AHB_SECURE_CTRL_SEC_CTRL_APB_BRIDGE0_MEM_CTRL1_CTIMER0_RULE(0x0U) |                 /* Standard counter/Timer 0 */
        AHB_SECURE_CTRL_SEC_CTRL_APB_BRIDGE0_MEM_CTRL1_CTIMER1_RULE(0x0U) |                 /* Standard counter/Timer 1 */
        AHB_SECURE_CTRL_SEC_CTRL_APB_BRIDGE0_MEM_CTRL1_WWDT_RULE(0x0U) |                    /* Windowed watchdog Timer */
        AHB_SECURE_CTRL_SEC_CTRL_APB_BRIDGE0_MEM_CTRL1_MRT_RULE(0x0U) |                     /* Multi-rate Timer */
        AHB_SECURE_CTRL_SEC_CTRL_APB_BRIDGE0_MEM_CTRL1_UTICK_RULE(0x0U);                    /* Micro-Timer */

    AHB_SECURE_CTRL->SEC_CTRL_APB_BRIDGE[0].SEC_CTRL_APB_BRIDGE0_MEM_CTRL2 =
        (0x33330333U) |                                                                     /* Bits have to be set to '1' according to UM.*/
        AHB_SECURE_CTRL_SEC_CTRL_APB_BRIDGE0_MEM_CTRL2_ANACTRL_RULE(0x0U);                  /* Analog modules controller */

    /* Security access rules for APB Bridge 1 peripherals. */
    AHB_SECURE_CTRL->SEC_CTRL_APB_BRIDGE[0].SEC_CTRL_APB_BRIDGE1_MEM_CTRL0 =
        (0x33300330U) |                                                                     /* Bits have to be set to '1' according to UM.*/
        AHB_SECURE_CTRL_SEC_CTRL_APB_BRIDGE1_MEM_CTRL0_PMC_RULE(0x0U) |                     /* Power Management Controller */
        AHB_SECURE_CTRL_SEC_CTRL_APB_BRIDGE1_MEM_CTRL0_SYSCTRL_RULE(0x3U) ;                 /* System Controller */

    AHB_SECURE_CTRL->SEC_CTRL_APB_BRIDGE[0].SEC_CTRL_APB_BRIDGE1_MEM_CTRL1 =
        (0x33003000U) |                                                                     /* Bits have to be set to '1' according to UM.*/
        AHB_SECURE_CTRL_SEC_CTRL_APB_BRIDGE1_MEM_CTRL1_CTIMER2_RULE(0x0U) |                 /* Standard counter/Timer 2 */
        AHB_SECURE_CTRL_SEC_CTRL_APB_BRIDGE1_MEM_CTRL1_CTIMER3_RULE(0x0U) |                 /* Standard counter/Timer 3 */
        AHB_SECURE_CTRL_SEC_CTRL_APB_BRIDGE1_MEM_CTRL1_CTIMER4_RULE(0x0U) |                 /* Standard counter/Timer 4 */
        AHB_SECURE_CTRL_SEC_CTRL_APB_BRIDGE1_MEM_CTRL1_RTC_RULE(0x0U) |                     /* Real Time Counter */
        AHB_SECURE_CTRL_SEC_CTRL_APB_BRIDGE1_MEM_CTRL1_OSEVENT_RULE(0x0U);                  /* OS Event Timer */

    AHB_SECURE_CTRL->SEC_CTRL_APB_BRIDGE[0].SEC_CTRL_APB_BRIDGE1_MEM_CTRL2 =
        (0x33003333U) |                                                                     /* Bits have to be set to '1' according to UM.*/
        AHB_SECURE_CTRL_SEC_CTRL_APB_BRIDGE1_MEM_CTRL2_FLASH_CTRL_RULE(0x3U) |              /* Flash controller = S*/
        AHB_SECURE_CTRL_SEC_CTRL_APB_BRIDGE1_MEM_CTRL2_PRINCE_RULE(0x3U);                   /* PRINCE = S */

    AHB_SECURE_CTRL->SEC_CTRL_APB_BRIDGE[0].SEC_CTRL_APB_BRIDGE1_MEM_CTRL3 =
        (0x30030033U) |                                                                     /* Bits have to be set to '1' according to UM.*/
        AHB_SECURE_CTRL_SEC_CTRL_APB_BRIDGE1_MEM_CTRL3_ROMPC_RULE(0x3U) |                   /* ROMPC_RULE - ROM patch controller */
        AHB_SECURE_CTRL_SEC_CTRL_APB_BRIDGE1_MEM_CTRL3_RNG_RULE(0x3U) |                     /* True Random Number Generator = S */
        AHB_SECURE_CTRL_SEC_CTRL_APB_BRIDGE1_MEM_CTRL3_PUF_RULE(0x3U) |                     /* PUF = S*/
        AHB_SECURE_CTRL_SEC_CTRL_APB_BRIDGE1_MEM_CTRL3_PLU_RULE(0x0U);                      /* Programmable Look-Up logic */

    /* Security access rules for AHB peripherals on AHB Slave Port P7*/
    AHB_SECURE_CTRL->SEC_CTRL_AHB_PORT7_SLAVE0_RULE =
        (0x00033033U) |                                                                     /* Bits have to be set to '1' according to UM.*/    
        AHB_SECURE_CTRL_SEC_CTRL_AHB_PORT7_SLAVE0_RULE_DMA0_RULE(0x0U) |                    /* DMA0 */
        AHB_SECURE_CTRL_SEC_CTRL_AHB_PORT7_SLAVE0_RULE_SCT_RULE(0x0U) |                     /* SCT */
        AHB_SECURE_CTRL_SEC_CTRL_AHB_PORT7_SLAVE0_RULE_FLEXCOMM0_RULE(0x0U) |               /* FLEXCOMM0 */
        AHB_SECURE_CTRL_SEC_CTRL_AHB_PORT7_SLAVE0_RULE_FLEXCOMM1_RULE(0x0U);                /* FLEXCOMM1 */

    AHB_SECURE_CTRL->SEC_CTRL_AHB_PORT7_SLAVE1_RULE = 
        (0x33303330U) |                                                                     /* Bits have to be set to '1' according to UM.*/
        AHB_SECURE_CTRL_SEC_CTRL_AHB_PORT7_SLAVE1_RULE_FLEXCOMM2_RULE(0x0U) |               /* FLEXCOMM2 */
        AHB_SECURE_CTRL_SEC_CTRL_AHB_PORT7_SLAVE1_RULE_GPIO0_RULE(0x0U);                    /* High Speed GPIO */

    /* Security access rules for AHB peripherals on AHB Slave Port P8 */
    AHB_SECURE_CTRL->SEC_CTRL_AHB_PORT8_SLAVE0_RULE =
        (0x33033333U) |                                                                     /* Bits have to be set to '1' according to UM.*/
        AHB_SECURE_CTRL_SEC_CTRL_AHB_PORT8_SLAVE0_RULE_CRC_RULE(0x0U);                      /* CRC engine */

    AHB_SECURE_CTRL->SEC_CTRL_AHB_PORT8_SLAVE1_RULE =
        (0x03303333U) |                                                                     /* Bits have to be set to '1' according to UM.*/
        AHB_SECURE_CTRL_SEC_CTRL_AHB_PORT8_SLAVE1_RULE_DBG_MAILBOX_RULE(0x0U) |             /* Debug mailbox (aka ISP-AP) */
        AHB_SECURE_CTRL_SEC_CTRL_AHB_PORT8_SLAVE1_RULE_SPIFI_RULE(0x0U);                    /* SPIFI_RULE - SPIFI */

    /* Security access rules for AHB peripherals on AHB Slave Port P9. */
    AHB_SECURE_CTRL->SEC_CTRL_AHB_PORT9[0].SLAVE0_RULE =
        (0x03003333U) |                                                                     /* Bits have to be set to '1' according to UM.*/
        AHB_SECURE_CTRL_SEC_CTRL_AHB_PORT9_SLAVE0_RULE_HASH_RULE(0x3U) |                    /* SHA-2 crypto registers  = S*/
        AHB_SECURE_CTRL_SEC_CTRL_AHB_PORT9_SLAVE0_RULE_CASPER_RULE(0x3U) |                  /* RSA/ECC crypto accelerator = S*/
        AHB_SECURE_CTRL_SEC_CTRL_AHB_PORT9_SLAVE0_RULE_DMA1_RULE(0x3U);                     /* DMA Controller (Secure) */

    AHB_SECURE_CTRL->SEC_CTRL_AHB_PORT9[0].SLAVE1_RULE =
        (0x33333300U) |  
        AHB_SECURE_CTRL_SEC_CTRL_AHB_PORT9_SLAVE1_RULE_GPIO1_RULE(0x0U) |                   /* Secure High Speed GPIO */
        AHB_SECURE_CTRL_SEC_CTRL_AHB_PORT9_SLAVE1_RULE_AHB_SEC_CTRL_RULE(0x3U);             /* AHB Secure Controller = S */

    /* Security access rules for AHB secure control. */
    AHB_SECURE_CTRL->SEC_CTRL_AHB_SEC_CTRL_MEM_RULE[0] = 
        (0x33330000U) | 
        AHB_SECURE_CTRL_SEC_CTRL_AHB_SEC_CTRL_MEM_RULE_AHB_SEC_CTRL_SECT_0_RULE(0x0U) |     /* Address space: 0x400A_C000 - 0x400A_CFFF */
        AHB_SECURE_CTRL_SEC_CTRL_AHB_SEC_CTRL_MEM_RULE_AHB_SEC_CTRL_SECT_1_RULE(0x0U) |     /* Address space: 0x400A_D000 - 0x400A_DFFF */
        AHB_SECURE_CTRL_SEC_CTRL_AHB_SEC_CTRL_MEM_RULE_AHB_SEC_CTRL_SECT_2_RULE(0x0U) |     /* Address space: 0x400A_E000 - 0x400A_EFFF */
        AHB_SECURE_CTRL_SEC_CTRL_AHB_SEC_CTRL_MEM_RULE_AHB_SEC_CTRL_SECT_3_RULE(0x0U);      /* Address space: 0x400A_F000 - 0x400A_FFFF */

    AHB_SECURE_CTRL->SEC_CTRL_AHB_PORT10_SLAVE0_RULE =
        (0x33333330U) |  
        AHB_SECURE_CTRL_SEC_CTRL_AHB_PORT10_SLAVE0_RULE_BLE_RADIO_RULE(0x0U);               /* BLE_RADIO_RULE - AHB Secure Controller */

    /* Security level configuration of masters */
    SECURE_READ_MODIFY_WRITE_REGISTER(&(AHB_SECURE_CTRL->MASTER_SEC_LEVEL),
        ~(AHB_SECURE_CTRL_MASTER_SEC_LEVEL_SDMA0_MASK |
 	AHB_SECURE_CTRL_MASTER_SEC_LEVEL_HASH_MASK |
	AHB_SECURE_CTRL_MASTER_SEC_LEVEL_SDMA1_MASK |
	AHB_SECURE_CTRL_MASTER_SEC_LEVEL_MASTER_SEC_LEVEL_LOCK_MASK),
        (AHB_SECURE_CTRL_MASTER_SEC_LEVEL_SDMA0(0x0U) |
        AHB_SECURE_CTRL_MASTER_SEC_LEVEL_HASH(0x0U) |
        AHB_SECURE_CTRL_MASTER_SEC_LEVEL_SDMA1(0x0U) |
        AHB_SECURE_CTRL_MASTER_SEC_LEVEL_MASTER_SEC_LEVEL_LOCK(0x2U)));

    /* Security level configuration of masters */
    SECURE_READ_MODIFY_WRITE_REGISTER(&(AHB_SECURE_CTRL->MASTER_SEC_ANTI_POL_REG),
        ~(AHB_SECURE_CTRL_MASTER_SEC_ANTI_POL_REG_SDMA0_MASK |
 	AHB_SECURE_CTRL_MASTER_SEC_ANTI_POL_REG_HASH_MASK |
	AHB_SECURE_CTRL_MASTER_SEC_ANTI_POL_REG_SDMA1_MASK |
	AHB_SECURE_CTRL_MASTER_SEC_ANTI_POL_REG_MASTER_SEC_LEVEL_ANTIPOL_LOCK_MASK),
        (AHB_SECURE_CTRL_MASTER_SEC_ANTI_POL_REG_SDMA0(0x3U) |
        AHB_SECURE_CTRL_MASTER_SEC_ANTI_POL_REG_HASH(0x3U) |
        AHB_SECURE_CTRL_MASTER_SEC_ANTI_POL_REG_SDMA1(0x3U) |
        AHB_SECURE_CTRL_MASTER_SEC_ANTI_POL_REG_MASTER_SEC_LEVEL_ANTIPOL_LOCK(0x2U)));

    /*--------------------------------------------------------------------
     - Pins: Reading GPIO state
     -------------------------------------------------------------------*/
    /* Possible values for every pin: -- TODO contradiction between comments in header file and in UM!
     *  0b0    Deny
     *  0b1    Allow */
    AHB_SECURE_CTRL->SEC_GPIO_MASK0 = 0x007FFFFFU;
    AHB_SECURE_CTRL->SEC_MASK_LOCK = 0x00000002U;

    AHB_SECURE_CTRL->CPU0_LOCK_REG = 0x800002AAU;

    /*--------------------------------------------------------------------
     - Interrupts: Interrupt security configuration
     -------------------------------------------------------------------*/
    /* Possible values for every interrupt:
     *  0b0    Secure
     *  0b1    Non-secure */
    NVIC->ITNS[0] = 0;
    NVIC->ITNS[1] = 0;

    /* Global Options */
    SCB->AIRCR = (SCB->AIRCR & 0x000009FF7U) | 0x005FA0000U;
    SCB->SCR &= 0x0FFFFFFF7U;
    SCB->SHCSR &= 0x0FFF7FFFFU;
    SCB->NSACR &= 0x0FFFFF3FCU;
    SCnSCB->CPPWR &= 0x0FF0FFFF0U;

    /* Update AHB Secure control register */
    SECURE_READ_MODIFY_WRITE_REGISTER(&(AHB_SECURE_CTRL->MISC_CTRL_REG),
        ~(AHB_SECURE_CTRL_MISC_CTRL_REG_ENABLE_SECURE_CHECKING_MASK   |
          AHB_SECURE_CTRL_MISC_CTRL_REG_DISABLE_SMART_MASTER_STRICT_MODE_MASK |
          AHB_SECURE_CTRL_MISC_CTRL_REG_DISABLE_SIMPLE_MASTER_STRICT_MODE_MASK |
          AHB_SECURE_CTRL_MISC_CTRL_REG_DISABLE_VIOLATION_ABORT_MASK   |
          AHB_SECURE_CTRL_MISC_CTRL_REG_IDAU_ALL_NS_MASK               ),
         (AHB_SECURE_CTRL_MISC_CTRL_REG_WRITE_LOCK              (0x2U) |         /* 2 = Writes to this register and to the Memory and Peripheral RULE registers are allowed */
          AHB_SECURE_CTRL_MISC_CTRL_REG_ENABLE_SECURE_CHECKING  (0x1U) |         /* 1 = Enable Secure Checking (restrictive mode) */
          AHB_SECURE_CTRL_MISC_CTRL_REG_ENABLE_S_PRIV_CHECK     (0x2U) |         /* 2 = Disabled Secure Privilege Checking */
          AHB_SECURE_CTRL_MISC_CTRL_REG_ENABLE_NS_PRIV_CHECK    (0x2U) |         /* 2 = Disabled Non-Secure Privilege Checking */
          AHB_SECURE_CTRL_MISC_CTRL_REG_DISABLE_VIOLATION_ABORT (0x2U) |         /* 2 = The violation detected by the secure checker will cause an abort. */
          AHB_SECURE_CTRL_MISC_CTRL_REG_DISABLE_SMART_MASTER_STRICT_MODE     (0x1U) |    /* 1 = AHB smart master in tier mode. */
          AHB_SECURE_CTRL_MISC_CTRL_REG_DISABLE_SIMPLE_MASTER_STRICT_MODE    (0x1U) |    /* 1 = AHB simple master in tier mode.*/
          AHB_SECURE_CTRL_MISC_CTRL_REG_IDAU_ALL_NS             (0x2U)));        /* 2 = IDAU is enabled (restrictive mode) */

    /* Secure control duplicate register */
    SECURE_READ_MODIFY_WRITE_REGISTER(&(AHB_SECURE_CTRL->MISC_CTRL_DP_REG),
        ~(AHB_SECURE_CTRL_MISC_CTRL_DP_REG_WRITE_LOCK_MASK                |
          AHB_SECURE_CTRL_MISC_CTRL_DP_REG_ENABLE_SECURE_CHECKING_MASK    |
          AHB_SECURE_CTRL_MISC_CTRL_DP_REG_DISABLE_SIMPLE_MASTER_STRICT_MODE_MASK |
          AHB_SECURE_CTRL_MISC_CTRL_DP_REG_DISABLE_SMART_MASTER_STRICT_MODE_MASK |
          AHB_SECURE_CTRL_MISC_CTRL_DP_REG_DISABLE_VIOLATION_ABORT_MASK   |
          AHB_SECURE_CTRL_MISC_CTRL_DP_REG_IDAU_ALL_NS_MASK               ),
         (AHB_SECURE_CTRL_MISC_CTRL_DP_REG_WRITE_LOCK              (0x2U) |      /* 2 = Writes to this register and to the Memory and Peripheral RULE registers are allowed */
          AHB_SECURE_CTRL_MISC_CTRL_DP_REG_ENABLE_SECURE_CHECKING  (0x1U) |      /* 1 = Enable Secure Checking (restrictive mode) */
          AHB_SECURE_CTRL_MISC_CTRL_DP_REG_ENABLE_S_PRIV_CHECK     (0x2U) |      /* 2 = Disabled Secure Privilege Checking */
          AHB_SECURE_CTRL_MISC_CTRL_DP_REG_ENABLE_NS_PRIV_CHECK    (0x2U) |      /* 2 = Disabled Non-Secure Privilege Checking */
          AHB_SECURE_CTRL_MISC_CTRL_DP_REG_DISABLE_VIOLATION_ABORT (0x2U) |      /* 2 = The violation detected by the secure checker will cause an abort. */
          AHB_SECURE_CTRL_MISC_CTRL_DP_REG_DISABLE_SMART_MASTER_STRICT_MODE     (0x1U) |    /* 1 = AHB smart master in tier mode. */
          AHB_SECURE_CTRL_MISC_CTRL_DP_REG_DISABLE_SIMPLE_MASTER_STRICT_MODE    (0x1U) |    /* 1 = AHB simple master in tier mode.*/
          AHB_SECURE_CTRL_MISC_CTRL_DP_REG_IDAU_ALL_NS             (0x2U)));     /* 2 = IDAU is enabled (restrictive mode) */

    return ARM_DRIVER_OK;
}
