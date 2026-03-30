/*
 * Copyright (c) 2018-2022 Arm Limited. All rights reserved.
 * Copyright 2025-2026 NXP
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
#include "fsl_trdc.h"
#include "fsl_glikey.h"

extern const struct memory_region_limits memory_regions;

struct platform_data_t tfm_peripheral_std_uart = {
        LPUART0_BASE_NS,
        LPUART0_BASE_NS + 0xFFF,
        0,
        0
};

struct platform_data_t tfm_peripheral_timer0 = {
        CTIMER1_BASE,
        CTIMER1_BASE + 0xFFF,
};

const int glikey_index_mbc = 15;

/*******************************************************************************
 * Definitions
 ******************************************************************************/
static uint32_t APP_GetMbc0Mem0BlockIdx(const void* mem_ptr)
{
  uint32_t idx = (uint32_t)mem_ptr;
  const uint32_t shift = (MBC0->MBC_INDEX[0].MBC_MEM_GLBCFG[0] & 
                          TRDC_MBC_MEM_GLBCFG_SIZE_LOG2_MASK ) >>
                          TRDC_MBC_MEM_GLBCFG_SIZE_LOG2_SHIFT;
  const uint32_t nblks = (MBC0->MBC_INDEX[0].MBC_MEM_GLBCFG[0] & 
                          TRDC_MBC_MEM_GLBCFG_NBLKS_MASK ) >>
                          TRDC_MBC_MEM_GLBCFG_NBLKS_SHIFT;
  
  /* Zero secure bit from address */
  idx &= ~0x10000000U;
  
  /* Shift by GLBCFG_NBLKS_LOG2*/
  idx >>= shift;
  
  /* Check if mem_ptr fits to MBC0_MEM0 */
  assert(idx < nblks);
  
  return idx;  
}

static void enable_mem_rule_for_partition(uint32_t memory_region_base, uint32_t memory_region_limit)
{
    uint32_t ns_region_id       = 0;
    uint32_t ns_region_start_id = 0;
    uint32_t ns_region_end_id   = 0;

    ns_region_start_id  = (memory_region_base - NS_ROM_ALIAS_BASE)
                          / FLASH_REGION0_SUBREGION_SIZE;
    ns_region_end_id    = ((memory_region_limit - NS_ROM_ALIAS_BASE)
                          / FLASH_REGION0_SUBREGION_SIZE) + 1;

    /* Set to non-secure and non-privileged user access allowed */
    for (ns_region_id = ns_region_start_id; ns_region_id < ns_region_end_id; ns_region_id++) /* == Region 0 == */
    {
        if (ns_region_id < 8) 
        {
            /* Set regions in the AHB controller for flash memory 0x00000000 - 0x0003FFFF*/
             AHBSC__AHBSC0->FLASH00_MEM_RULE[0] &= ~(0xF << (ns_region_id * 4));
        }
        else if ((ns_region_id >= 8) && (ns_region_id < 16))
        {
            /* Set regions in the AHB controller for flash memory 0x00040000 - 0x0007FFFF */
             AHBSC__AHBSC0->FLASH00_MEM_RULE[1] &= ~(0xF << ((ns_region_id - 8) * 4));
        }
        else if ((ns_region_id >= 16) && (ns_region_id < 24))
        {
            /* Set regions the AHB controller for flash memory 0x00080000 - 0x000BFFFF */
             AHBSC__AHBSC0->FLASH00_MEM_RULE[2] &= ~(0xF << ((ns_region_id - 16) * 4));
        }
        else if ((ns_region_id >= 24) && (ns_region_id < 32))
        {
            /* Set regions the AHB controller for flash memory 0x000C0000 -  0x000FFFFF */
             AHBSC__AHBSC0->FLASH00_MEM_RULE[3] &= ~(0xF << ((ns_region_id - 24) * 4));
        }
        else /* == Region does not exist == */
        {
            break;
        }
    }      
}

/*------------------- Memory configuration functions -------------------------*/
static void trdc_configs(void)
{
    uint32_t i, j, m, n;
    uint32_t ns_region_id = 0;
    uint32_t ns_region_start_id = 0;
    uint32_t ns_region_end_id = 0;

    TRDC_Init(MBC0);
    
    /* Make the all flash region accessible. */
    *(volatile uint32_t *)0x5008E020 = 0x00007700;  /* GLBAC idx: 0, being configured to have only Sec Prv and Sec user access  */
    *(volatile uint32_t *)0x5008E030 = 0x00007777;  /* GLBAC idx: 4, being configured to have all type of access */
    *(volatile uint32_t *)0x5008E040 = 0x00000000;
    *(volatile uint32_t *)0x5008E044 = 0x00000000;
    *(volatile uint32_t *)0x5008E048 = 0x00000000;
    *(volatile uint32_t *)0x5008E04C = 0x00000000;
    *(volatile uint32_t *)0x5008E050 = 0x00000000;
    *(volatile uint32_t *)0x5008E054 = 0x00000000;
    *(volatile uint32_t *)0x5008E058 = 0x00000000;
    *(volatile uint32_t *)0x5008E05C = 0x00000000;

    /* 1. Set control policies for MBC access control configuration registers */
    trdc_memory_access_control_config_t memAccessConfig_sec;
    trdc_memory_access_control_config_t memAccessConfig_all;
    trdc_memory_access_control_config_t memAccessConfig_ns;
    trdc_mbc_memory_block_config_t mbcBlockConfig;
    
    (void)memset(&memAccessConfig_sec, 0, sizeof(memAccessConfig_sec));
    (void)memset(&memAccessConfig_all, 0, sizeof(memAccessConfig_all));

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

    /* Enable secure priv and user access modes */
    memAccessConfig_sec.securePrivX    = 1U;
    memAccessConfig_sec.securePrivW    = 1U;
    memAccessConfig_sec.securePrivR    = 1U;
    memAccessConfig_sec.secureUsrX     = 1U;
    memAccessConfig_sec.secureUsrW     = 1U;
    memAccessConfig_sec.secureUsrR     = 1U;
    
    TRDC_MbcSetMemoryAccessConfig(MBC0, &memAccessConfig_sec, 0U, TRDC_ACCESS_CONTROL_POLICY_SEC_PRIV_INDEX);
    TRDC_MbcSetMemoryAccessConfig(MBC0, &memAccessConfig_all, 0U, TRDC_ACCESS_CONTROL_POLICY_NS_INDEX);

    /* 2. Set the configuration for the MBC slave 'Flash' as all SEC_PRIV by default */

    (void)memset(&mbcBlockConfig, 0, sizeof(mbcBlockConfig));
    mbcBlockConfig.memoryAccessControlSelect = TRDC_ACCESS_CONTROL_POLICY_SEC_PRIV_INDEX;
    mbcBlockConfig.nseEnable                 = false;
    mbcBlockConfig.mbcIdx                    = 0U; /* Only have one MBC */
    mbcBlockConfig.domainIdx                 = 0U; /* Only have one domain */
    mbcBlockConfig.slaveMemoryIdx            = MBC_SLAVE_FLASH;
    //mbcBlockConfig.memoryBlockIdx            = APP_GetMbc0Mem0BlockIdx((void*)g_mbc_mem);

    for (uint8_t i = 0U; i < 64U; i++)
    {
        mbcBlockConfig.memoryBlockIdx = i;
        TRDC_MbcSetMemoryBlockConfig(MBC0, &mbcBlockConfig);
    }

    /*Now, only flash regions of non-secure, shall be configured with NS access */
    /* The regions have to be alligned to FLASH_REGION0_SUBREGION_SIZE to cover the AHB Flash Region. */
    assert(((memory_regions.non_secure_partition_base - NS_ROM_ALIAS_BASE) % FLASH_REGION0_SUBREGION_SIZE) == 0);
    assert(((memory_regions.non_secure_partition_limit - NS_ROM_ALIAS_BASE + 1) % FLASH_REGION0_SUBREGION_SIZE) == 0);

    ns_region_start_id  = (memory_regions.non_secure_partition_base - NS_ROM_ALIAS_BASE)
                          / FLASH_REGION0_SUBREGION_SIZE;
    ns_region_end_id    = ((memory_regions.non_secure_partition_limit - NS_ROM_ALIAS_BASE)
                          / FLASH_REGION0_SUBREGION_SIZE) + 1;
    
    
    (void)memset(&mbcBlockConfig, 0, sizeof(mbcBlockConfig));
    mbcBlockConfig.memoryAccessControlSelect = TRDC_ACCESS_CONTROL_POLICY_NS_INDEX;
    mbcBlockConfig.nseEnable                 = true;
    mbcBlockConfig.mbcIdx                    = 0U; /* Only have one MBC */
    mbcBlockConfig.domainIdx                 = 0U; /* Only have one domain */
    mbcBlockConfig.slaveMemoryIdx            = MBC_SLAVE_FLASH;
    
    for (uint8_t i = ns_region_start_id; i < ns_region_end_id; i++)
    {
        mbcBlockConfig.memoryBlockIdx = APP_GetMbc0Mem0BlockIdx((void*)(i*FLASH_REGION0_SUBREGION_SIZE));
        TRDC_MbcSetMemoryBlockConfig(MBC0, &mbcBlockConfig);
    }    
    
}

static void app_set_glikey_accessible(uint32_t idx)
{
    /* Use Glikey to enable modifications of MBC registers: */

    status_t status = GLIKEY_IsLocked(GLIKEY0);
    assert(kStatus_GLIKEY_NotLocked == status);

    status = GLIKEY_SyncReset(GLIKEY0);
    assert(kStatus_Success == status);

    status = GLIKEY_StartEnable(GLIKEY0, idx);
    assert(kStatus_Success == status);

    status = GLIKEY_ContinueEnable(GLIKEY0, GLIKEY_CODEWORD_STEP1);
    assert(kStatus_Success == status);

    status = GLIKEY_ContinueEnable(GLIKEY0, GLIKEY_CODEWORD_STEP2);
    assert(kStatus_Success == status);

    status = GLIKEY_ContinueEnable(GLIKEY0, GLIKEY_CODEWORD_STEP3);
    assert(kStatus_Success == status);

    status = GLIKEY_ContinueEnable(GLIKEY0, GLIKEY_CODEWORD_STEP_EN);
    assert(kStatus_Success == status);

    (void)status; /* In case of release build there is not assert. */
}

int32_t mpc_init_cfg(void)
{
    uint32_t ns_region_id = 0;
    uint32_t ns_region_start_id = 0;
    uint32_t ns_region_end_id = 0;
    int i;

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

    /* == On-Chip Flash region == */
    /* Flash region has 512 KB of Flash, It is divided into 32 sub-regions (sector). Each flash sub-regions (sector) is 16 kbytes. */
    /* Two other banks are 32 KB (IFR0) and 8 KB. Each divided into 4 sub-regions (sectro) with sub-region size of 8 KB and 2KB respectively.*/
    /* 1) Set FLASH memory security access rule configuration to init value (0x3 = all regions set to secure and privileged user access) */
    for(i = 0; i < 4; i++)
    {
         AHBSC__AHBSC0->FLASH00_MEM_RULE[i] = 0x33333333U;
    }
    AHBSC__AHBSC0->FLASH01_MEM_RULE[0] = 0x33333333U;
    AHBSC__AHBSC0->FLASH02_MEM_RULE[0] = 0x33333333U;
    
    /* 2) Set FLASH memory security access rule configuration (set to non-secure and non-privileged user access allowed).*/

    /* == Region 0 == */
    /* The regions have to be alligned to FLASH_REGION0_SUBREGION_SIZE to cover the AHB Flash Region. */
    assert(((memory_regions.non_secure_partition_base - NS_ROM_ALIAS_BASE) % FLASH_REGION0_SUBREGION_SIZE) == 0);
    assert(((memory_regions.non_secure_partition_limit - NS_ROM_ALIAS_BASE + 1) % FLASH_REGION0_SUBREGION_SIZE) == 0);

    enable_mem_rule_for_partition(memory_regions.non_secure_partition_base, memory_regions.non_secure_partition_limit);

    /* == ROM region == */
    /* Each ROM sector is 1 kbytes. There are 32 ROM sectors in total. */
    /* Security control ROM memory configuration (0x3 = all regions set to secure and privileged user access). */
    for(i=0; i < 4; i++)
    {
         AHBSC__AHBSC0->ROM_MEM_RULE[i] = 0x33333333U;
    }

    /* == RAMX region == */

    /* Each RAMX sub region is 2 kbytes. There are 8 such regions*/
    /* Security access rules for RAMX (0x3 = all regions set to secure and privileged user access). */
    AHBSC__AHBSC0->RAMX_MEM_RULE[0] = 0x33333333U; /* 0x0400_0800 - 0x0400_2FFF */

    /* == SRAM region == */

    /* The regions have to be alligned to 4 kB to cover the AHB RAM Region */
    assert((S_DATA_SIZE % DATA_SUBREGION_SIZE) == 0);
    assert(((S_DATA_SIZE + NS_DATA_SIZE) % DATA_SUBREGION_SIZE) == 0);

    /* Security access rules for RAM (0x3 = all regions set to secure and privileged user access*/
    
     AHBSC__AHBSC0->RAMA_MEM_RULE[0] = 0x33333333U; /* 0x2800_0000 - 0x2800_4FFF */
     AHBSC__AHBSC0->RAMA_MEM_RULE[1] = 0x33333333U; /* 0x2800_5000 - 0x2800_5FFF */
     AHBSC__AHBSC0->RAMA_MEM_RULE[2] = 0x33333333U; /* 0x2800_6000 - 0x2800_6FFF */
     AHBSC__AHBSC0->RAMA_MEM_RULE[3] = 0x33333333U; /* 0x2800_7000 - 0x2800_7FFF */

     AHBSC__AHBSC0->RAMB_MEM_RULE[0] = 0x33333333U; /* 0x2801_0000 - 0x2801_0FFF */
     AHBSC__AHBSC0->RAMB_MEM_RULE[1] = 0x33333333U; /* 0x2801_5000 - 0x2801_1FFF */
     AHBSC__AHBSC0->RAMB_MEM_RULE[2] = 0x33333333U; /* 0x2801_6000 - 0x2801_2FFF */
     AHBSC__AHBSC0->RAMB_MEM_RULE[3] = 0x33333333U; /* 0x2801_7000 - 0x2801_3FFF */
    
    /* SRAM memory configuration (set according to region_defs.h and flash_layout.h) */
    /* == SRAM Region 0 (0-32KB) == */
    ns_region_start_id  = (NS_DATA_START - NS_RAM_ALIAS_BASE) / DATA_SUBREGION_SIZE; /* NS starts after S */
    ns_region_end_id  = (NS_DATA_START - NS_RAM_ALIAS_BASE + NS_DATA_SIZE) / DATA_SUBREGION_SIZE;

    for(ns_region_id = ns_region_start_id; ns_region_id < ns_region_end_id; ns_region_id++)
    {
        if(ns_region_id < 8) {
             AHBSC__AHBSC0->RAMA_MEM_RULE[0] &= ~(0xF << (ns_region_id*4));
        }
        else if((ns_region_id >= 8) && (ns_region_id < 16)) {
             AHBSC__AHBSC0->RAMA_MEM_RULE[1] &= ~(0xF << ((ns_region_id-8)*4));
        }
        else if((ns_region_id >= 16) && (ns_region_id < 24)) {
             AHBSC__AHBSC0->RAMA_MEM_RULE[2] &= ~(0xF << ((ns_region_id-16)*4));
        }
        else if((ns_region_id >= 24) && (ns_region_id < 32)) {
             AHBSC__AHBSC0->RAMA_MEM_RULE[3] &= ~(0xF << ((ns_region_id-24)*4));
        }
        else if((ns_region_id >= 32) && (ns_region_id < 40)) {
             AHBSC__AHBSC0->RAMB_MEM_RULE[0] &= ~(0xF << ((ns_region_id-32)*4));
        }
        else if((ns_region_id >= 40) && (ns_region_id < 48)) {
             AHBSC__AHBSC0->RAMB_MEM_RULE[1] &= ~(0xF << ((ns_region_id-40)*4));
        }
        else if((ns_region_id >= 48) && (ns_region_id < 56)) {
             AHBSC__AHBSC0->RAMB_MEM_RULE[2] &= ~(0xF << ((ns_region_id-48)*4));
        }
        else if((ns_region_id >= 56) && (ns_region_id < 64)) {
             AHBSC__AHBSC0->RAMB_MEM_RULE[3] &= ~(0xF << ((ns_region_id-56)*4));
        }
    }

#if TARGET_DEBUG_LOG
    SPMLOG_DBGMSG("=== [AHB MPC NS] =======\r\n");
    SPMLOG_DBGMSGVAL("NS ROM starts from : ",
                                      memory_regions.non_secure_partition_base);
    SPMLOG_DBGMSGVAL("NS ROM ends at : ",
                                     memory_regions.non_secure_partition_limit);
    SPMLOG_DBGMSGVAL("NS DATA start from : ", NS_DATA_START);
    SPMLOG_DBGMSGVAL("NS DATA ends at : ", NS_DATA_LIMIT);
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

    AHBSC__AHBSC0->AHB_PERIPHERAL_SLAVE_PORT_P5_SLAVE_RULE0  = 
       (0x00033333U)|
       AHBSC_AHB_PERIPHERAL_SLAVE_PORT_P5_SLAVE_RULE0_CDOG0(0x3U) |
       AHBSC_AHB_PERIPHERAL_SLAVE_PORT_P5_SLAVE_RULE0_DEBUG_MAILBOX(0x3U) |
       AHBSC_AHB_PERIPHERAL_SLAVE_PORT_P5_SLAVE_RULE0_GPIO1(0x0U);

    AHBSC__AHBSC0->AHB_PERIPHERAL_SLAVE_PORT_P5_SLAVE_RULE1  = 
       (0x33333000U)|
       AHBSC_AHB_PERIPHERAL_SLAVE_PORT_P5_SLAVE_RULE1_CDOG1(0x3U) |
       AHBSC_AHB_PERIPHERAL_SLAVE_PORT_P5_SLAVE_RULE1_GPIO2(0x0U) |
       AHBSC_AHB_PERIPHERAL_SLAVE_PORT_P5_SLAVE_RULE1_GPIO3(0x0U);

    /* Write access attributes for AHBSC module are tier-4 (secure privileged). */
    AHBSC__AHBSC0->APB_PERIPHERAL_GROUP0_MEM_RULE0 =
       (0x00003303U) | 
       AHBSC_APB_PERIPHERAL_GROUP0_MEM_RULE0_CTIMER0(0x0U) |                   /* Standard counter/Timer 0 */
       AHBSC_APB_PERIPHERAL_GROUP0_MEM_RULE0_CTIMER1(0x0U) |                   /* Standard counter/Timer 1 */
       AHBSC_APB_PERIPHERAL_GROUP0_MEM_RULE0_CTIMER2(0x0U) |                   /* Standard counter/Timer 2 */
       AHBSC_APB_PERIPHERAL_GROUP0_MEM_RULE0_INPUTMUX0(0x0U);                  /* INPUTMUX */

    AHBSC__AHBSC0->APB_PERIPHERAL_GROUP0_MEM_RULE1 =
       (0x33300303U) |
       AHBSC_APB_PERIPHERAL_GROUP0_MEM_RULE1_FREQME00(0x0U) |
       AHBSC_APB_PERIPHERAL_GROUP0_MEM_RULE1_UTICK0(0x0U) |  
       AHBSC_APB_PERIPHERAL_GROUP0_MEM_RULE1_WWDT0(0x0U);

    AHBSC__AHBSC0->AIPS_BRIDGE_GROUP0_MEM_RULE0 =
       (0x33300000U) |
       AHBSC_AIPS_BRIDGE_GROUP0_MEM_RULE0_eDMA_0_TCD3(0x3U) |
       AHBSC_AIPS_BRIDGE_GROUP0_MEM_RULE0_eDMA_0_TCD2(0x3U) |
       AHBSC_AIPS_BRIDGE_GROUP0_MEM_RULE0_eDMA_0_TCD1(0x3U) |
       AHBSC_AIPS_BRIDGE_GROUP0_MEM_RULE0_eDMA_0_TCD0(0x3U) |
       AHBSC_AIPS_BRIDGE_GROUP0_MEM_RULE0_eDMA_0_MP(0x3U) ;

    AHBSC__AHBSC0->AIPS_BRIDGE_GROUP0_MEM_RULE1 =
       (0x00030003U) |
       AHBSC_AIPS_BRIDGE_GROUP0_MEM_RULE1_SCG0(0x0U) |
       AHBSC_AIPS_BRIDGE_GROUP0_MEM_RULE1_MBC0(0x3U) |
       AHBSC_AIPS_BRIDGE_GROUP0_MEM_RULE1_ERM0(0x3U) |
       AHBSC_AIPS_BRIDGE_GROUP0_MEM_RULE1_CRC0(0x3U) |
       AHBSC_AIPS_BRIDGE_GROUP0_MEM_RULE1_CRC0(0x3U) |
       AHBSC_AIPS_BRIDGE_GROUP0_MEM_RULE1_AOI0(0x3U);

    AHBSC__AHBSC0->AIPS_BRIDGE_GROUP0_MEM_RULE2 = 
       (0x30003003U) | 
       AHBSC_AIPS_BRIDGE_GROUP0_MEM_RULE2_SYSCON(0x0U) |
       AHBSC_AIPS_BRIDGE_GROUP0_MEM_RULE2_WUU0(0x0U) |
       AHBSC_AIPS_BRIDGE_GROUP0_MEM_RULE2_FMC0(0x0U) |
       AHBSC_AIPS_BRIDGE_GROUP0_MEM_RULE2_FMU0(0x0U) |
       AHBSC_AIPS_BRIDGE_GROUP0_MEM_RULE2_FMU0_TST(0x0U);

    AHBSC__AHBSC0->AIPS_BRIDGE_GROUP0_MEM_RULE3 =
       (0x03000033U) |
       AHBSC_AIPS_BRIDGE_GROUP0_MEM_RULE3_LPUART0(0x0U) |
       AHBSC_AIPS_BRIDGE_GROUP0_MEM_RULE3_LPSPI1(0x0U) |
       AHBSC_AIPS_BRIDGE_GROUP0_MEM_RULE3_LPSPI0(0x0U) |
       AHBSC_AIPS_BRIDGE_GROUP0_MEM_RULE3_LPI2C1(0x0U) |
       AHBSC_AIPS_BRIDGE_GROUP0_MEM_RULE3_LPI2C0(0x0U);

    AHBSC__AHBSC0->AIPS_BRIDGE_GROUP2_MEM_RULE0 =
      (0x33333330U) |
      AHBSC_AIPS_BRIDGE_GROUP2_MEM_RULE0_LPUART1(0x0U);

    AHBSC__AHBSC0->AIPS_BRIDGE_GROUP2_MEM_RULE1 =
      (0x03033303U) |
      AHBSC_AIPS_BRIDGE_GROUP2_MEM_RULE1_FLEXPWM0(0x0U) |
      AHBSC_AIPS_BRIDGE_GROUP2_MEM_RULE1_OSTIMER0(0x0U) |
      AHBSC_AIPS_BRIDGE_GROUP2_MEM_RULE1_HSADC0(0x0U); 

    AHBSC__AHBSC0->AIPS_BRIDGE_GROUP2_MEM_RULE2 =
      (0x33333303U) |  
      AHBSC_AIPS_BRIDGE_GROUP2_MEM_RULE2_CMP0(0x0U); 

    AHBSC__AHBSC0->AIPS_BRIDGE_GROUP2_MEM_RULE3 =
      (0x30003333U) |
      AHBSC_AIPS_BRIDGE_GROUP2_MEM_RULE3_PORT1(0x0U) |
      AHBSC_AIPS_BRIDGE_GROUP2_MEM_RULE3_PORT2(0x0U) |
      AHBSC_AIPS_BRIDGE_GROUP2_MEM_RULE3_PORT3(0x0U); 

    AHBSC__AHBSC0->AIPS_BRIDGE_GROUP3_MEM_RULE0 =
      (0x33303333U) |
      AHBSC_AIPS_BRIDGE_GROUP3_MEM_RULE0_MU0(0x0U); 

    AHBSC__AHBSC0->AIPS_BRIDGE_GROUP3_MEM_RULE2 =
       (0x03330000U) |
       AHBSC_AIPS_BRIDGE_GROUP3_MEM_RULE2_eMDA_1_MP(0x0U) |
       AHBSC_AIPS_BRIDGE_GROUP3_MEM_RULE2_PKC0(0x3U) |
       AHBSC_AIPS_BRIDGE_GROUP3_MEM_RULE2_SGI0(0x3U) |
       AHBSC_AIPS_BRIDGE_GROUP3_MEM_RULE2_TRNG0(0x3U) |
       AHBSC_AIPS_BRIDGE_GROUP3_MEM_RULE2_UDF0(0x0U); 
     
    AHBSC__AHBSC0->AIPS_BRIDGE_GROUP3_MEM_RULE3 =
      (0x33330000U) |
      AHBSC_AIPS_BRIDGE_GROUP3_MEM_RULE3_eDMA_1_TCD0(0x0U) |
      AHBSC_AIPS_BRIDGE_GROUP3_MEM_RULE3_eDMA_1_TCD1(0x0U) |
      AHBSC_AIPS_BRIDGE_GROUP3_MEM_RULE3_eDMA_1_TCD2(0x0U) |
      AHBSC_AIPS_BRIDGE_GROUP3_MEM_RULE3_eDMA_1_TCD3(0x0U);

    AHBSC__AHBSC0->AHB_SECURE_CTRL_PERIPHERAL_RULE0 =
      (0x33330000U) |
      AHBSC_AHB_SECURE_CTRL_PERIPHERAL_RULE0_RULE0(0x0U) |
      AHBSC_AHB_SECURE_CTRL_PERIPHERAL_RULE0_RULE1(0x0U) |
      AHBSC_AHB_SECURE_CTRL_PERIPHERAL_RULE0_RULE2(0x0U) |
      AHBSC_AHB_SECURE_CTRL_PERIPHERAL_RULE0_RULE3(0x0U);

    AHBSC__AHBSC0->AON_DOMAIN_PERIPHERAL_MEM_RULE0 =
      (0x30003030U) |
      AHBSC_AON_DOMAIN_PERIPHERAL_MEM_RULE0_LPI2C(0x0U) |
      AHBSC_AON_DOMAIN_PERIPHERAL_MEM_RULE0_ACMP(0x0U) |
      AHBSC_AON_DOMAIN_PERIPHERAL_MEM_RULE0_PORT0(0x0U) |
      AHBSC_AON_DOMAIN_PERIPHERAL_MEM_RULE0_MUB(0x0U) |
      AHBSC_AON_DOMAIN_PERIPHERAL_MEM_RULE0_LPUART(0x0U) ;

    AHBSC__AHBSC0->AON_DOMAIN_PERIPHERAL_MEM_RULE1 =
      (0x00333330U) |
      AHBSC_AON_DOMAIN_PERIPHERAL_MEM_RULE1_LPTIMER(0x0U) |
      AHBSC_AON_DOMAIN_PERIPHERAL_MEM_RULE1_QTMR32_0(0x0U) |
      AHBSC_AON_DOMAIN_PERIPHERAL_MEM_RULE1_QTMR32_1(0x0U);
 
     AHBSC__AHBSC0->AON_DOMAIN_PERIPHERAL_MEM_RULE2 =
      (0x03003000U) |
      AHBSC_AON_DOMAIN_PERIPHERAL_MEM_RULE2_ADVC2(0x0U) |
      AHBSC_AON_DOMAIN_PERIPHERAL_MEM_RULE2_KPP(0x0U) |
      AHBSC_AON_DOMAIN_PERIPHERAL_MEM_RULE2_SGLCD(0x0U) |
      AHBSC_AON_DOMAIN_PERIPHERAL_MEM_RULE2_LPADC(0x0U) |
      AHBSC_AON_DOMAIN_PERIPHERAL_MEM_RULE2_INPUTMUX(0x0U) |
      AHBSC_AON_DOMAIN_PERIPHERAL_MEM_RULE2_GP_GPR(0x0U) ;

     AHBSC__AHBSC0->AON_DOMAIN_PERIPHERAL_MEM_RULE3 =
      (0x03330000U) |
      AHBSC_AON_DOMAIN_PERIPHERAL_MEM_RULE3_AON_GPIO(0x0U) |
      AHBSC_AON_DOMAIN_PERIPHERAL_MEM_RULE3_LPCMP(0x0U) |
      AHBSC_AON_DOMAIN_PERIPHERAL_MEM_RULE3_SMM(0x0U) |
      AHBSC_AON_DOMAIN_PERIPHERAL_MEM_RULE3_CGU(0x0U) |
      AHBSC_AON_DOMAIN_PERIPHERAL_MEM_RULE3_PMIC_CTRL(0x0U);

     AHBSC__AHBSC0->AON_DOMAIN_SRAM_MEM_RULE[0] =
      (0x00000000U) |
      AHBSC_AON_DOMAIN_SRAM_MEM_RULE_RULE0(0x0U) |
      AHBSC_AON_DOMAIN_SRAM_MEM_RULE_RULE1(0x0U) |
      AHBSC_AON_DOMAIN_SRAM_MEM_RULE_RULE4(0x0U) |
      AHBSC_AON_DOMAIN_SRAM_MEM_RULE_RULE5(0x0U) |
      AHBSC_AON_DOMAIN_SRAM_MEM_RULE_RULE6(0x0U) |
      AHBSC_AON_DOMAIN_SRAM_MEM_RULE_RULE7(0x0U);

    /* TRDC- MBC0 configs*/
    app_set_glikey_accessible(glikey_index_mbc);
    trdc_configs();
    
     /* CPU Lock register*/
    AHBSC__AHBSC0->CPU0_LOCK_REG = 0x000002AAU;
     
    /* Global Options */
    SCB->AIRCR = (SCB->AIRCR & 0x00009FF7U) | 0x05FA0000U;
    SCB->SCR &= 0xFFFFFFF7U;
    SCB->SHCSR &= 0xFFF7FFFFU;
    SCB->CPACR = 0x00F00000U;
    SCB->NSACR = 0x00000C03U;
    SCnSCB->CPPWR = 0;

    /* Set Master Security Level of PKC and ELS Security Level to secure and privileged master, and lock it. */
    SECURE_READ_MODIFY_WRITE_REGISTER(&( AHBSC__AHBSC0->MASTER_SEC_LEVEL),                                   
        ~(AHBSC_MASTER_SEC_LEVEL_DMA0_MASK |
          AHBSC_MASTER_SEC_LEVEL_DMA1_MASK |
          AHBSC_MASTER_SEC_LEVEL_PKC_MASK ),
         (AHBSC_MASTER_SEC_LEVEL_DMA0(0x0U) |
          AHBSC_MASTER_SEC_LEVEL_DMA1(0x0U) |
          AHBSC_MASTER_SEC_LEVEL_PKC(0x3U)));                              /* PKC */

    /* Set the corresponding ANTI POL register */
    SECURE_READ_MODIFY_WRITE_REGISTER((&( AHBSC__AHBSC0->MASTER_SEC_ANTI_POL_REG)),
        ~(AHBSC_MASTER_SEC_ANTI_POL_REG_DMA0_MASK |
          AHBSC_MASTER_SEC_ANTI_POL_REG_DMA1_MASK |
          AHBSC_MASTER_SEC_ANTI_POL_REG_PKC_MASK ),
        (AHBSC_MASTER_SEC_ANTI_POL_REG_DMA0(0x0U) |
         AHBSC_MASTER_SEC_ANTI_POL_REG_DMA1(0x0U) |
         AHBSC_MASTER_SEC_ANTI_POL_REG_PKC(0x3U)));   

    /* Update AHB Secure control register */
    app_set_glikey_accessible(10);
    SECURE_READ_MODIFY_WRITE_REGISTER((&( AHBSC__AHBSC0->MISC_CTRL_REG)),
        ~(AHBSC_MISC_CTRL_REG_WRITE_LOCK_MASK                |
          AHBSC_MISC_CTRL_REG_ENABLE_SECURE_CHECKING_MASK    |
          AHBSC_MISC_CTRL_REG_ENABLE_S_PRIV_CHECK_MASK       |
          AHBSC_MISC_CTRL_REG_ENABLE_NS_PRIV_CHECK_MASK      |
          AHBSC_MISC_CTRL_REG_DISABLE_VIOLATION_ABORT_MASK   |
          AHBSC_MISC_CTRL_REG_DISABLE_STRICT_MODE_MASK       |
          AHBSC_MISC_CTRL_REG_IDAU_ALL_NS_MASK               ),
         (AHBSC_MISC_CTRL_REG_WRITE_LOCK              (0x2U) |         /* 2 = Writes to this register and to the Memory and Peripheral RULE registers are allowed */
          AHBSC_MISC_CTRL_REG_ENABLE_SECURE_CHECKING  (0x1U) |         /* 1 = Enable Secure Checking (restrictive mode) */
          AHBSC_MISC_CTRL_REG_ENABLE_S_PRIV_CHECK     (0x2U) |         /* 2 = Disabled Secure Privilege Checking */
          AHBSC_MISC_CTRL_REG_ENABLE_NS_PRIV_CHECK    (0x2U) |         /* 2 = Disabled Non-Secure Privilege Checking */
          AHBSC_MISC_CTRL_REG_DISABLE_VIOLATION_ABORT (0x2U) |         /* 2 = The violation detected by the secure checker will cause an abort. */
          AHBSC_MISC_CTRL_REG_DISABLE_STRICT_MODE     (0x1U) |         /* 1 = AHB master in tier mode. Can read and write to memories at same or below level. */
          AHBSC_MISC_CTRL_REG_IDAU_ALL_NS             (0x2U)));        /* 2 = IDAU is enabled (restrictive mode) */

    /* Secure control duplicate register */
    SECURE_READ_MODIFY_WRITE_REGISTER((&( AHBSC__AHBSC0->MISC_CTRL_DP_REG)),
        ~(AHBSC_MISC_CTRL_DP_REG_WRITE_LOCK_MASK                |
          AHBSC_MISC_CTRL_DP_REG_ENABLE_SECURE_CHECKING_MASK    |
          AHBSC_MISC_CTRL_DP_REG_ENABLE_S_PRIV_CHECK_MASK       |
          AHBSC_MISC_CTRL_DP_REG_ENABLE_NS_PRIV_CHECK_MASK      |
          AHBSC_MISC_CTRL_DP_REG_DISABLE_VIOLATION_ABORT_MASK   |
          AHBSC_MISC_CTRL_DP_REG_DISABLE_STRICT_MODE_MASK       |
          AHBSC_MISC_CTRL_DP_REG_IDAU_ALL_NS_MASK               ),
         (AHBSC_MISC_CTRL_DP_REG_WRITE_LOCK              (0x2U) |      /* 2 = Writes to this register and to the Memory and Peripheral RULE registers are allowed */
          AHBSC_MISC_CTRL_DP_REG_ENABLE_SECURE_CHECKING  (0x1U) |      /* 1 = Enable Secure Checking (restrictive mode) */
          AHBSC_MISC_CTRL_DP_REG_ENABLE_S_PRIV_CHECK     (0x2U) |      /* 2 = Disabled Secure Privilege Checking */
          AHBSC_MISC_CTRL_DP_REG_ENABLE_NS_PRIV_CHECK    (0x2U) |      /* 2 = Disabled Non-Secure Privilege Checking */
          AHBSC_MISC_CTRL_DP_REG_DISABLE_VIOLATION_ABORT (0x2U) |      /* 2 = The violation detected by the secure checker will cause an abort. */
          AHBSC_MISC_CTRL_DP_REG_DISABLE_STRICT_MODE     (0x1U) |      /* 1 = AHB master in tier mode. Can read and write to memories at same or below level. */
          AHBSC_MISC_CTRL_DP_REG_IDAU_ALL_NS             (0x2U)));     /* 2 = IDAU is enabled (restrictive mode) */

    return ARM_DRIVER_OK;
}

void ppc_configure_to_secure(struct platform_data_t *platform_data, bool privileged)
{

}

/* HARDENING_MACROS_ENABLED is defined*/
#ifdef HARDENING_MACROS_ENABLED
/* NOTE: __attribute__((used)) is used because IAR toolchain inline assembly 
is not able to extern the function via MACRO defined in target_cfg_common.h*/
/* fault_detect handling function
 */
__attribute__((used)) static void fault_detect_handling(void)
{
    SPMLOG_ERRMSG("fault detected during secure REG write!!\n");
    tfm_core_panic();  
}
#endif
