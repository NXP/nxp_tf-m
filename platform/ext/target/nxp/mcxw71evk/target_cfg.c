/*
 * Copyright (c) 2018-2022 Arm Limited. All rights reserved.
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

#include "target_cfg.h"
#include "Driver_Common.h"
#include "platform_description.h"
#include "region_defs.h"
#include "tfm_plat_defs.h"
#include "utilities.h"
#include "tfm_spm_log.h"

#include "fsl_trdc.h"

extern const struct memory_region_limits memory_regions;



struct platform_data_t tfm_peripheral_std_uart = {
        .periph_start = (uint32_t)LPUART_BASE,
        .periph_limit = (uint32_t)LPUART_BASE + 0xFFF,
        .nseEnable = true,
        .mbcIdx  = 2,
        .slaveMemoryIdx = 0,
        .memoryBlockIdx = 57,
};

struct platform_data_t tfm_peripheral_timer0 = {
        .periph_start = LPTMR0_BASE,
        .periph_limit = LPTMR0_BASE + 0xFFF,
        .nseEnable = false,
        .mbcIdx = 2,
        .slaveMemoryIdx = 0,
        .memoryBlockIdx = 45,
};

extern void BOARD_InitHardware(void);
/*------------------- Memory configuration functions -------------------------*/

int32_t mpc_init_cfg(void)
{
    uint32_t ns_region_id = 0;
    uint32_t ns_region_start_id = 0;
    uint32_t ns_region_end_id = 0;
    uint32_t ns_region_start = 0;
    uint32_t ns_region_end = 0;
    
    uint32_t i, j, m, n;
    trdc_idau_config_t idau_cfg;

    /* For KW platform's TRDC, board config will be done before setting static boundaries */
    BOARD_InitHardware();

    TRDC_GetDefaultIDAUConfig(&idau_cfg);
    TRDC_SetIDAU(TRDC, &idau_cfg);
    
    TRDC_Init(TRDC);

    /* 1. Get the hardware configuration of the TRDC module */
    trdc_hardware_config_t hwConfig;
    TRDC_GetHardwareConfig(TRDC, &hwConfig);

    /* 2. Set control policies for MBC and MRC access control configuration registers */
    trdc_memory_access_control_config_t memAccessConfig_all;
    trdc_memory_access_control_config_t memAccessConfig_sec_priv;
    trdc_memory_access_control_config_t memAccessConfig_sec;
    trdc_memory_access_control_config_t memAccessConfig_ns;
    
    (void)memset(&memAccessConfig_all, 0, sizeof(memAccessConfig_all));
    (void)memset(&memAccessConfig_sec, 0, sizeof(memAccessConfig_sec));
    (void)memset(&memAccessConfig_sec_priv, 0, sizeof(memAccessConfig_sec_priv));
    (void)memset(&memAccessConfig_ns, 0, sizeof(memAccessConfig_ns));

     /* Disable all access modes for MBC and MRC access control configuration register 1-7. */
    for (i = 0U; i < hwConfig.mbcNumber; i++)
    {
        for (j = 1U; j < 8U; j++)
        {
            TRDC_MbcSetMemoryAccessConfig(TRDC, &memAccessConfig_all, i, j);
        }
    }

    for (i = 0U; i < hwConfig.mrcNumber; i++)
    {
        for (j = 1U; j < 8U; j++)
        {
            TRDC_MrcSetMemoryAccessConfig(TRDC, &memAccessConfig_all, i, j);
        }
    }
    
    /* Enable secure privelege access modes */
    memAccessConfig_sec_priv.securePrivX    = 1U;
    memAccessConfig_sec_priv.securePrivW    = 1U;
    memAccessConfig_sec_priv.securePrivR    = 1U;
    
    /* Enable secure priv and user access modes */
    memAccessConfig_sec.securePrivX    = 1U;
    memAccessConfig_sec.securePrivW    = 1U;
    memAccessConfig_sec.securePrivR    = 1U;
    memAccessConfig_sec.secureUsrX     = 1U;
    memAccessConfig_sec.secureUsrW     = 1U;
    memAccessConfig_sec.secureUsrR     = 1U;
    
    /* Enable non-secure access modes */
    memAccessConfig_ns.nonsecureUsrX  = 1U;
    memAccessConfig_ns.nonsecureUsrW  = 1U;
    memAccessConfig_ns.nonsecureUsrR  = 1U;
    memAccessConfig_ns.nonsecurePrivX = 1U;
    memAccessConfig_ns.nonsecurePrivW = 1U;
    memAccessConfig_ns.nonsecurePrivR = 1U;
    
    /* Enable all access modes */
    memAccessConfig_all.nonsecureUsrX  = 1U;
    memAccessConfig_all.nonsecureUsrW  = 1U;
    memAccessConfig_all.nonsecureUsrR  = 1U;
    memAccessConfig_all.nonsecurePrivX = 1U;
    memAccessConfig_all.nonsecurePrivW = 1U;
    memAccessConfig_all.nonsecurePrivR = 1U;
    memAccessConfig_all.secureUsrX     = 1U;
    memAccessConfig_all.secureUsrW     = 1U;
    memAccessConfig_all.secureUsrR     = 1U;
    memAccessConfig_all.securePrivX    = 1U;
    memAccessConfig_all.securePrivW    = 1U;
    memAccessConfig_all.securePrivR    = 1U;

    /* 3. Set Memory Access Rules */
    for (i = 0U; i < hwConfig.mrcNumber; i++)
    {
        TRDC_MrcSetMemoryAccessConfig(TRDC, &memAccessConfig_sec_priv, i, TRDC_ACCESS_CONTROL_POLICY_SEC_PRIV_INDEX);
        TRDC_MrcSetMemoryAccessConfig(TRDC, &memAccessConfig_sec, i, TRDC_ACCESS_CONTROL_POLICY_SEC_INDEX);
        TRDC_MrcSetMemoryAccessConfig(TRDC, &memAccessConfig_ns, i, TRDC_ACCESS_CONTROL_POLICY_NS_INDEX);
        TRDC_MrcSetMemoryAccessConfig(TRDC, &memAccessConfig_all, i, TRDC_ACCESS_CONTROL_POLICY_ALL_INDEX);
    }

    for (i = 0U; i < hwConfig.mbcNumber; i++)
    {
        TRDC_MbcSetMemoryAccessConfig(TRDC, &memAccessConfig_sec_priv, i, TRDC_ACCESS_CONTROL_POLICY_SEC_PRIV_INDEX);
        TRDC_MbcSetMemoryAccessConfig(TRDC, &memAccessConfig_sec, i, TRDC_ACCESS_CONTROL_POLICY_SEC_INDEX);
        TRDC_MbcSetMemoryAccessConfig(TRDC, &memAccessConfig_ns, i, TRDC_ACCESS_CONTROL_POLICY_NS_INDEX);
        TRDC_MbcSetMemoryAccessConfig(TRDC, &memAccessConfig_all, i, TRDC_ACCESS_CONTROL_POLICY_ALL_INDEX);
    }
    
    /* 4. Set the configuration for all MBC slave memory blocks */
    trdc_slave_memory_hardware_config_t mbcHwConfig;
    trdc_mbc_memory_block_config_t mbcBlockConfig;
    (void)memset(&mbcBlockConfig, 0, sizeof(mbcBlockConfig));

    /* Configure MBC Block configs for various slaves.
     * By default secure priv access mode for complete flash, memory
     * non secure access enabled for all peripherals.
     */
    for (i = 0U; i < hwConfig.mbcNumber; i++)
    {
        mbcBlockConfig.mbcIdx = i;
        /* By default make all peipherals accessible to non secure world */
        if (i == TRDC_MBC_PERIPHERALS) {
              mbcBlockConfig.memoryAccessControlSelect = TRDC_ACCESS_CONTROL_POLICY_ALL_INDEX;
              mbcBlockConfig.nseEnable                 = true;
        } else {
              mbcBlockConfig.memoryAccessControlSelect = TRDC_ACCESS_CONTROL_POLICY_SEC_INDEX;
              mbcBlockConfig.nseEnable                 = false;
        }
        for (j = 0U; j < hwConfig.domainNumber; j++)
        {
            mbcBlockConfig.domainIdx = j;
            for (m = 0U; m < 4; m++)
            {
                TRDC_GetMbcHardwareConfig(TRDC, &mbcHwConfig, i, m);
                if (mbcHwConfig.blockNum == 0U)
                {
                    break;
                }
                mbcBlockConfig.slaveMemoryIdx = m;
                for (n = 0U; n < mbcHwConfig.blockNum; n++)
                {
                    mbcBlockConfig.memoryBlockIdx = n;
               
                    TRDC_MbcSetMemoryBlockConfig(TRDC, &mbcBlockConfig);
                }
            }
        }
    }

    /* 5. Set FLASH memory security access rule configuration (set to non-secure and non-privileged user access allowed).*/
    ns_region_start_id = memory_regions.non_secure_partition_base/TRDC_FLASH_SUBREGION_SIZE;
    ns_region_end_id = (memory_regions.non_secure_partition_limit+1)/FLASH_SUBREGION_SIZE;
    
    (void)memset(&mbcBlockConfig, 0, sizeof(mbcBlockConfig));
    mbcBlockConfig.memoryAccessControlSelect = TRDC_ACCESS_CONTROL_POLICY_NS_INDEX;
    mbcBlockConfig.nseEnable                 = true;
    
    mbcBlockConfig.domainIdx = TRDC_PROCESSOR_MASTER_DOMAIN_ID;       /* Core domain */
    mbcBlockConfig.mbcIdx = TRDC_MBC_FLASH;
    mbcBlockConfig.slaveMemoryIdx = TRDC_MBC_FLASH_SLAVE_PORT;
    
    for (n = ns_region_start_id; n < ns_region_end_id; n++)
    {
        mbcBlockConfig.memoryBlockIdx = n;
        
        TRDC_MbcSetMemoryBlockConfig(TRDC, &mbcBlockConfig);
    }

    /* 6. RAM memory configuration (set according to region_defs.h and flash_layout.h) */
    ns_region_start = NS_DATA_START;      /* NS region starts after S */
    ns_region_end = NS_DATA_START + NS_DATA_SIZE;
    
    (void)memset(&mbcBlockConfig, 0, sizeof(mbcBlockConfig));
    mbcBlockConfig.memoryAccessControlSelect = TRDC_ACCESS_CONTROL_POLICY_ALL_INDEX;
    mbcBlockConfig.nseEnable                 = true;
    
    mbcBlockConfig.domainIdx = 0;       /* Core domain */
    mbcBlockConfig.mbcIdx = TRDC_MBC_TCM;
    
    uint32_t start = ns_region_start;
    
    while (start < ns_region_end)
    {
        if (start >= TRDC_STCM_SLV1_START && start < TRDC_STCM_SLV1_END)
        {
	    ns_region_start_id = (start - TRDC_STCM_SLV1_START) /TRDC_STCM_SUBREGION_SIZE;
            if (ns_region_end > TRDC_STCM_SLV1_END)
            {
                ns_region_end_id = (TRDC_STCM_SLV1_END + 1 - TRDC_STCM_SLV1_START)/TRDC_STCM_SUBREGION_SIZE;
                start = TRDC_STCM_SLV1_END + 1;
            } else
            {
                ns_region_end_id = (ns_region_end + 1)/TRDC_STCM_SUBREGION_SIZE;
		start = ns_region_end;
            }
            mbcBlockConfig.slaveMemoryIdx = TRDC_MBC_STCM_SLAVE_PORT;	     
        } 
        else if (start >= TRDC_STCM_SLV2_START && start < TRDC_STCM_SLV2_END)
        {
	    ns_region_start_id = (start - TRDC_STCM_SLV2_START)/TRDC_STCM_SUBREGION_SIZE;
            if (ns_region_end > TRDC_STCM_SLV2_END)
            {
                ns_region_end_id = (TRDC_STCM_SLV2_END + 1 - TRDC_STCM_SLV2_START)/TRDC_STCM_SUBREGION_SIZE;
		start = TRDC_STCM_SLV2_END + 1;
            } else
            {
                ns_region_end_id = (ns_region_end + 1)/TRDC_STCM_SUBREGION_SIZE;
		start = ns_region_end;
            }
            mbcBlockConfig.slaveMemoryIdx = TRDC_MBC_STCM_3_4_SLAVE_PORT;  
	} 
        else if (start >= TRDC_STCM_SLV3_START && start < TRDC_STCM_SLV3_END)
        {
            ns_region_start_id = (start - TRDC_STCM_SLV3_START) /TRDC_STCM5_SUBREGION_SIZE;
            if (ns_region_end > TRDC_STCM_SLV3_END)
            {
                ns_region_end_id = (TRDC_STCM_SLV3_END + 1 - TRDC_STCM_SLV3_START)/TRDC_STCM5_SUBREGION_SIZE;
		start = TRDC_STCM_SLV3_END + 1;
            } else 
            {
                ns_region_end_id = (ns_region_end + 1)/TRDC_STCM5_SUBREGION_SIZE;
		start = ns_region_end;
            }
            mbcBlockConfig.slaveMemoryIdx = TRDC_MBC_STCM5_SLAVE_PORT;		     
	} 
        else
        {
			break;
	}
	for (n = ns_region_start_id; n < ns_region_end_id; n++)
    	{
            mbcBlockConfig.memoryBlockIdx = n;
	    TRDC_MbcSetMemoryBlockConfig(TRDC, &mbcBlockConfig);
    	}
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
     trdc_mbc_memory_block_config_t mbcBlockConfig;
     
     /* For MBC2, peripherals which need policy as Secure priveleged
      *              20,21 -> MCM,SMCM
      *              32,33,34 -> FMU, Reg files
      *              35-42 -> SEC modules
      */
    (void)memset(&mbcBlockConfig, 0, sizeof(mbcBlockConfig));
    mbcBlockConfig.memoryAccessControlSelect = TRDC_ACCESS_CONTROL_POLICY_SEC_PRIV_INDEX;
    mbcBlockConfig.nseEnable                 = false;
 
    mbcBlockConfig.domainIdx = 0;
    mbcBlockConfig.mbcIdx = 2;
    mbcBlockConfig.slaveMemoryIdx = 0;
 
    /* MCM */
    mbcBlockConfig.memoryBlockIdx = 20;    
    TRDC_MbcSetMemoryBlockConfig(TRDC, &mbcBlockConfig);

     /* SMCM */
    mbcBlockConfig.memoryBlockIdx = 21;    
    TRDC_MbcSetMemoryBlockConfig(TRDC, &mbcBlockConfig);
    
    /* FMU, Register files, Security Modules */
    for (uint32_t n = 32U; n < 43U; n++)
    {
        mbcBlockConfig.memoryBlockIdx = n;        
        TRDC_MbcSetMemoryBlockConfig(TRDC, &mbcBlockConfig);
    }

 /*
  * With TRDC, if a region needs to be shared between secure &
  * non-secure world, NSE bit is set. That region can then be accessed
  * only using non secure address. So for peripherals like UART, clock,
  * the drivers in secure world need to use Non-Secure address.
  */
    /* Enable MBC in TRDC */
    TRDC_SetMbcGlobalValid(TRDC);
 
    /* Set master domain ID for processor master */
    trdc_processor_domain_assignment_t pDomainAssignment;
    
    TRDC_GetDefaultProcessorDomainAssignment(&pDomainAssignment);
    pDomainAssignment.domainId = TRDC_PROCESSOR_MASTER_DOMAIN_ID;

    TRDC_SetProcessorDomainAssignment(TRDC, &pDomainAssignment);

    TRDC_SetDacGlobalValid(TRDC);

    return ARM_DRIVER_OK;
}

void ppc_configure_to_secure(struct platform_data_t *platform_data, bool privileged)
{
#ifdef TRDC
    /* If the peripheral is not shared with non-secure world, give it SEC access */
    if (platform_data && platform_data->nseEnable == false)
    {
        trdc_mbc_memory_block_config_t mbcBlockConfig;

        (void)memset(&mbcBlockConfig, 0, sizeof(mbcBlockConfig));
    
        mbcBlockConfig.nseEnable  = false;
    
        mbcBlockConfig.domainIdx = 0;       /* Core domain */
        mbcBlockConfig.mbcIdx = platform_data->mbcIdx;
        mbcBlockConfig.slaveMemoryIdx = platform_data->slaveMemoryIdx;
        mbcBlockConfig.memoryBlockIdx = platform_data->memoryBlockIdx;
 
        if (privileged == true)
            mbcBlockConfig.memoryAccessControlSelect = TRDC_ACCESS_CONTROL_POLICY_SEC_PRIV_INDEX;
        else
            mbcBlockConfig.memoryAccessControlSelect = TRDC_ACCESS_CONTROL_POLICY_SEC_INDEX;
        
        TRDC_MbcSetMemoryBlockConfig(TRDC, &mbcBlockConfig);
    }
#endif    
}
