/*
 * Copyright (c) 2018-2022 Arm Limited. All rights reserved.
 * Copyright 2026 NXP
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

#include <assert.h>
#include "target_cfg.h"
#include "Driver_Common.h"
#include "platform_description.h"
#include "device_definition.h"
#include "region_defs.h"
#include "tfm_plat_defs.h"
#include "target_cfg_common.h"
#include "utilities.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/
extern const struct memory_region_limits memory_regions;

struct platform_data_t tfm_peripheral_std_uart =
{
    LPUART0_BASE_NS,
    LPUART0_BASE_NS + 0xFFF,
    0,
    0
};

struct platform_data_t tfm_peripheral_timer0 =
{
    CTIMER2_BASE,
    CTIMER2_BASE + 0xFFF
};

typedef struct {
    uint32_t base;           /* Base address of bank */
    uint32_t subregion_size; /* Subregion granularity */
    uint32_t num_subregions; /* Number of subregions */
} ram_region_desc_t;
 
/* RAM bank layout */
static const ram_region_desc_t ram_regions[] = {
    { 0x20000000UL, 0x400, 128 },  /* Region0: 128×1KB = 128KB */
    { 0x20020000UL, 0x800, 64 },   /* Region1: 64×2KB = 128KB */
    { 0x20040000UL, 0x1000, 64 },  /* Region2: 64×4KB = 256KB */
    { 0x20080000UL, 0x2000, 64 },  /* Region3: 64×8KB = 512KB */
    { 0x20100000UL, 0x4000, 64 },  /* Region4: 64×16KB = 1MB */
    { 0x20200000UL, 0x8000, 64 },  /* Region5: 64×32KB = 2MB */
    { 0x20400000UL, 0x4000, 64 },  /* Region6: 64×16KB = 1MB */
    { 0x20500000UL, 0x2000, 64 },  /* Region7: 64×8KB = 512KB */
    { 0x20580000UL, 0x400, 128 },  /* Region8: 128×1KB = 128KB */
    { 0x205A0000UL, 0x800, 64 },   /* Region9: 64×2KB = 128KB */
    { 0x205C0000UL, 0x1000, 64 },  /* Region10: 64×4KB = 256KB */
    { 0x20600000UL, 0x4000, 64 },  /* Region11: 64×16KB = 1MB */
    { 0x20700000UL, 0x2000, 64 },  /* Region12: 64×8KB = 512KB */
};

/*******************************************************************************
 * Code
 ******************************************************************************/

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
            /* Set regions in the AHB controller for flash memory 0x0800 0000 - 0x080F FFFF */
            AHBSC0->XSPI0_REGION0_MEM_RULE[0] &= ~(0xF << (ns_region_id * 4));
        }
        else if ((ns_region_id >= 8) && (ns_region_id < 16))
        {
            /* Set regions in the AHB controller for flash memory 0x0810 0000 - 0x081F FFFF */
            AHBSC0->XSPI0_REGION0_MEM_RULE[1] &= ~(0xF << ((ns_region_id - 8) * 4));
        }
        else if ((ns_region_id >= 16) && (ns_region_id < 24))
        {
            /* Set regions the AHB controller for flash memory 0x0820 0000 - 0x082 FFFFF */
            AHBSC0->XSPI0_REGION0_MEM_RULE[2] &= ~(0xF << ((ns_region_id - 16) * 4));
        }
        else if ((ns_region_id >= 24) && (ns_region_id < 32))
        {
            /* Set regions the AHB controller for flash memory 0x0830 0000 - 0x083F FFFF */
            AHBSC0->XSPI0_REGION0_MEM_RULE[3] &= ~(0xF << ((ns_region_id - 24) * 4));
        }
        else /* == Region 1 == */
        {
            if (ns_region_start_id > FLASH_REGION1_SUBREGION_NUMBER)
            {
                ns_region_start_id = (memory_region_base
                                      - NS_ROM_ALIAS_BASE
                                      - FLASH_REGION0_SIZE ) / FLASH_REGION1_SUBREGION_SIZE;
            }
            else
            {
                ns_region_start_id = 0;   
            }
            
            ns_region_end_id = ((memory_region_limit
                                - NS_ROM_ALIAS_BASE
                                - FLASH_REGION0_SIZE
                      			) / FLASH_REGION1_SUBREGION_SIZE) + 1;
            
            for (ns_region_id = ns_region_start_id; ns_region_id < ns_region_end_id; ns_region_id++)
            {
                if (ns_region_id < FLASH_REGION1_SUBREGION_NUMBER)
                {
                    /* Set regions in the AHB controller for flash memory 0x0840 0000 - 0x087 FFFFF */
                    AHBSC0->XSPI0_REGION1_4_MEM_RULE[0].XSPI0_REGION_MEM_RULE0 &= ~(0xF << (ns_region_id * 4));
                }
                else /* == Region 2 == */
                {
                    if (ns_region_start_id > FLASH_REGION1_SUBREGION_NUMBER)
                    {
                        ns_region_start_id = (memory_region_base
                                              - NS_ROM_ALIAS_BASE
                                              - (FLASH_REGION0_SIZE + FLASH_REGION1_SIZE))
                                             / FLASH_REGION2_SUBREGION_SIZE;
                    }
                    else
                    {
                        ns_region_start_id = 0;   
                    }
                    
                    ns_region_end_id = ((memory_region_limit
                                        - NS_ROM_ALIAS_BASE
                                        - (FLASH_REGION0_SIZE + FLASH_REGION1_SIZE)
                                        ) / FLASH_REGION2_SUBREGION_SIZE) + 1;
                    
                    for (ns_region_id = ns_region_start_id; ns_region_id < ns_region_end_id; ns_region_id++) 
                    {
                        if (ns_region_id < FLASH_REGION2_SUBREGION_NUMBER)
                        {
                            /* Set regions in the AHB controller for flash memory 0x0880 0000 - 0x8FF FFFF */
                            AHBSC0->XSPI0_REGION1_4_MEM_RULE[1].XSPI0_REGION_MEM_RULE0 &= ~(0xF << (ns_region_id * 4));
                        }
                        else /* == Region 3 == */
                        {
                            if (ns_region_start_id > FLASH_REGION2_SUBREGION_NUMBER)
                            {
                                ns_region_start_id = (memory_region_base
                                                      - NS_ROM_ALIAS_BASE
                                                      - (FLASH_REGION0_SIZE
                                                         + FLASH_REGION1_SIZE
                                                         + FLASH_REGION2_SIZE)) / FLASH_REGION3_SUBREGION_SIZE;
                            }
                            else
                            {
                                ns_region_start_id = 0;   
                            }
                        
                            ns_region_end_id = ((memory_region_limit
                                                - NS_ROM_ALIAS_BASE
                                                - (FLASH_REGION0_SIZE
                                                   + FLASH_REGION1_SIZE
                                                   + FLASH_REGION2_SIZE)
                                                ) / FLASH_REGION3_SUBREGION_SIZE) + 1;

                            for (ns_region_id = ns_region_start_id; ns_region_id < ns_region_end_id; ns_region_id++) 
                            {
                                if (ns_region_id < FLASH_REGION3_SUBREGION_NUMBER)
                                {
                                    /* Set regions in the AHB controller for flash memory 0x0900 0000 - 0x9FF FFFF */
                                    AHBSC0->XSPI0_REGION1_4_MEM_RULE[2].XSPI0_REGION_MEM_RULE0 &= ~(0xF << (ns_region_id * 4));
                                }
                                else
                                {
                                    break;
                                }
                            }
                            break;
                        }
                    }
                    break;
                }
            }
            break;
        }
    }  	
}

static void clear_ahbsc4_sram_rule(void)
{
    AHBSC4->SRAM_0_RULE[0] = 0;
    AHBSC4->SRAM_0_RULE[1] = 0;
    AHBSC4->SRAM_0_RULE[2] = 0;
    AHBSC4->SRAM_0_RULE[3] = 0;
    AHBSC4->SRAM_1_RULE[0] = 0;
    AHBSC4->SRAM_1_RULE[1] = 0;
    AHBSC4->SRAM_1_RULE[2] = 0;
    AHBSC4->SRAM_1_RULE[3] = 0;
    AHBSC4->SRAM_2_RULE[0] = 0;
    AHBSC4->SRAM_2_RULE[1] = 0;
    AHBSC4->SRAM_2_RULE[2] = 0;
    AHBSC4->SRAM_2_RULE[3] = 0;
    AHBSC4->SRAM_3_RULE[0] = 0;
    AHBSC4->SRAM_3_RULE[1] = 0;
    AHBSC4->SRAM_3_RULE[2] = 0;
    AHBSC4->SRAM_3_RULE[3] = 0;
    AHBSC4->SRAM_4_RULE[0] = 0;
    AHBSC4->SRAM_4_RULE[1] = 0;
    AHBSC4->SRAM_4_RULE[2] = 0;
    AHBSC4->SRAM_4_RULE[3] = 0;
    AHBSC4->SRAM_5_RULE[0] = 0;
    AHBSC4->SRAM_5_RULE[1] = 0;
    AHBSC4->SRAM_5_RULE[2] = 0;
    AHBSC4->SRAM_5_RULE[3] = 0;
    AHBSC4->SRAM_6_RULE[0] = 0;
    AHBSC4->SRAM_6_RULE[1] = 0;
    AHBSC4->SRAM_6_RULE[2] = 0;
    AHBSC4->SRAM_6_RULE[3] = 0;
    AHBSC4->SRAM_7_RULE[0] = 0;
    AHBSC4->SRAM_7_RULE[1] = 0;
    AHBSC4->SRAM_7_RULE[2] = 0;
    AHBSC4->SRAM_7_RULE[3] = 0;
    AHBSC4->SRAM_8_RULE[0] = 0;
    AHBSC4->SRAM_8_RULE[1] = 0;
    AHBSC4->SRAM_8_RULE[2] = 0;
    AHBSC4->SRAM_8_RULE[3] = 0;
    AHBSC4->SRAM_9_RULE[0] = 0;
    AHBSC4->SRAM_9_RULE[1] = 0;
    AHBSC4->SRAM_9_RULE[2] = 0;
    AHBSC4->SRAM_9_RULE[3] = 0;
    AHBSC4->SRAM_10_RULE[0] = 0;
    AHBSC4->SRAM_10_RULE[1] = 0;
    AHBSC4->SRAM_10_RULE[2] = 0;
    AHBSC4->SRAM_10_RULE[3] = 0;
    AHBSC4->SRAM_11_RULE[0] = 0;
    AHBSC4->SRAM_11_RULE[1] = 0;
    AHBSC4->SRAM_11_RULE[2] = 0;
    AHBSC4->SRAM_11_RULE[3] = 0;
    AHBSC4->SRAM_12_RULE[0] = 0;
    AHBSC4->SRAM_12_RULE[1] = 0;
    AHBSC4->SRAM_12_RULE[2] = 0;
    AHBSC4->SRAM_12_RULE[3] = 0;
    AHBSC4->SRAM_13_RULE[0] = 0;
    AHBSC4->SRAM_13_RULE[1] = 0;
    AHBSC4->SRAM_13_RULE[2] = 0;
    AHBSC4->SRAM_13_RULE[3] = 0;
    AHBSC4->SRAM_14_RULE[0] = 0;
    AHBSC4->SRAM_14_RULE[1] = 0;
    AHBSC4->SRAM_14_RULE[2] = 0;
    AHBSC4->SRAM_14_RULE[3] = 0;
    AHBSC4->SRAM_15_RULE[0] = 0;
    AHBSC4->SRAM_15_RULE[1] = 0;
    AHBSC4->SRAM_15_RULE[2] = 0;
    AHBSC4->SRAM_15_RULE[3] = 0;
    AHBSC4->SRAM_16_RULE[0] = 0;
    AHBSC4->SRAM_16_RULE[1] = 0;
    AHBSC4->SRAM_16_RULE[2] = 0;
    AHBSC4->SRAM_16_RULE[3] = 0;
    AHBSC4->SRAM_17_RULE[0] = 0;
    AHBSC4->SRAM_17_RULE[1] = 0;
    AHBSC4->SRAM_17_RULE[2] = 0;
    AHBSC4->SRAM_17_RULE[3] = 0;
    AHBSC4->SRAM_18_RULE[0] = 0;
    AHBSC4->SRAM_18_RULE[1] = 0;
    AHBSC4->SRAM_18_RULE[2] = 0;
    AHBSC4->SRAM_18_RULE[3] = 0;
    AHBSC4->SRAM_19_RULE[0] = 0;
    AHBSC4->SRAM_19_RULE[1] = 0;
    AHBSC4->SRAM_19_RULE[2] = 0;
    AHBSC4->SRAM_19_RULE[3] = 0;
    AHBSC4->SRAM_20_RULE[0] = 0;
    AHBSC4->SRAM_20_RULE[1] = 0;
    AHBSC4->SRAM_20_RULE[2] = 0;
    AHBSC4->SRAM_20_RULE[3] = 0;
    AHBSC4->SRAM_21_RULE[0] = 0;
    AHBSC4->SRAM_21_RULE[1] = 0;
    AHBSC4->SRAM_21_RULE[2] = 0;
    AHBSC4->SRAM_21_RULE[3] = 0;
    AHBSC4->SRAM_22_RULE[0] = 0;
    AHBSC4->SRAM_22_RULE[1] = 0;
    AHBSC4->SRAM_22_RULE[2] = 0;
    AHBSC4->SRAM_22_RULE[3] = 0;
    AHBSC4->SRAM_23_RULE[0] = 0;
    AHBSC4->SRAM_23_RULE[1] = 0;
    AHBSC4->SRAM_23_RULE[2] = 0;
    AHBSC4->SRAM_23_RULE[3] = 0;
    AHBSC4->SRAM_24_RULE[0] = 0;
    AHBSC4->SRAM_24_RULE[1] = 0;
    AHBSC4->SRAM_24_RULE[2] = 0;
    AHBSC4->SRAM_24_RULE[3] = 0;
    AHBSC4->SRAM_25_RULE[0] = 0;
    AHBSC4->SRAM_25_RULE[1] = 0;
    AHBSC4->SRAM_25_RULE[2] = 0;
    AHBSC4->SRAM_25_RULE[3] = 0;
    AHBSC4->SRAM_26_RULE[0] = 0;
    AHBSC4->SRAM_26_RULE[1] = 0;
    AHBSC4->SRAM_26_RULE[2] = 0;
    AHBSC4->SRAM_26_RULE[3] = 0;
    AHBSC4->SRAM_27_RULE[0] = 0;
    AHBSC4->SRAM_27_RULE[1] = 0;
    AHBSC4->SRAM_27_RULE[2] = 0;
    AHBSC4->SRAM_27_RULE[3] = 0;
    AHBSC4->SRAM_28_RULE[0] = 0;
    AHBSC4->SRAM_28_RULE[1] = 0;
    AHBSC4->SRAM_28_RULE[2] = 0;
    AHBSC4->SRAM_28_RULE[3] = 0;
    AHBSC4->SRAM_29_RULE[0] = 0;
    AHBSC4->SRAM_29_RULE[1] = 0;
    AHBSC4->SRAM_29_RULE[2] = 0;
    AHBSC4->SRAM_29_RULE[3] = 0;  
}

/**
* Configure non-secure RAM dynamically.
*/
static enum tfm_plat_err_t platform_config_non_secure_region(uint32_t* ram_rule_sfr[])
{
        
    uint32_t ns_start = NS_DATA_START;
    uint32_t ns_end   = NS_DATA_START + NS_DATA_SIZE;
 
    for (size_t bank = 0; bank < sizeof(ram_regions)/sizeof(ram_regions[0]); bank++) 
    {
        const ram_region_desc_t *r = &ram_regions[bank];
        uint32_t bank_end = r->base + (r->subregion_size * r->num_subregions);
 
        /* Find overlap of NS region with this bank */
        uint32_t overlap_start = (ns_start > r->base) ? ns_start : r->base;
        uint32_t overlap_end   = (ns_end   < bank_end) ? ns_end : bank_end;
 
        if (overlap_start >= overlap_end) 
        {
            continue; /* No overlap */
        }
 
        /* Subregion IDs relative to this bank */
        uint32_t start_id = (overlap_start - r->base) / r->subregion_size;
        uint32_t end_id   = (overlap_end   - r->base + r->subregion_size - 1) /
                            r->subregion_size;
 
        for (uint32_t region_id = start_id; region_id < end_id; region_id++) 
        {
            uint32_t sfr_index  = region_id / 32;         /* 32 regions per group */
            uint32_t sfr_offset = (region_id / 8) % 4;    /* 8 regions per word */
            uint32_t sfr_shift  = (region_id % 8) * 4;    /* 4 bits per region */
 
            uint32_t *reg = ram_rule_sfr[bank] + sfr_offset;
 
            /* Clear field: mark as NS RWX (0x0 = NS on most TRDC/AHBSC) */
            *reg &= ~(0xFu << sfr_shift);
        }
    }
 
    return TFM_PLAT_ERR_SUCCESS;
}

/*------------------- Memory configuration functions -------------------------*/
int32_t mpc_init_cfg(void)
{
    uint32_t* ram_rule_sfr[] = {
      (uint32_t*)&AHBSC0->SRAM_0_RULE[0],
      (uint32_t*)&AHBSC0->SRAM_1_RULE[0],
      (uint32_t*)&AHBSC0->SRAM_2_RULE[0],
      (uint32_t*)&AHBSC0->SRAM_3_RULE[0],
      (uint32_t*)&AHBSC0->SRAM_4_RULE[0],
      (uint32_t*)&AHBSC0->SRAM_5_RULE[0],
      (uint32_t*)&AHBSC0->SRAM_6_RULE[0],
      (uint32_t*)&AHBSC0->SRAM_7_RULE[0],
      (uint32_t*)&AHBSC0->SRAM_8_RULE[0],
      (uint32_t*)&AHBSC0->SRAM_9_RULE[0],
      (uint32_t*)&AHBSC0->SRAM_10_RULE[0],
      (uint32_t*)&AHBSC0->SRAM_11_RULE[0],
      (uint32_t*)&AHBSC0->SRAM_12_RULE[0],
      (uint32_t*)&AHBSC0->SRAM_13_RULE[0],
      (uint32_t*)&AHBSC0->SRAM_14_RULE[0],
      (uint32_t*)&AHBSC0->SRAM_15_RULE[0],
      (uint32_t*)&AHBSC0->SRAM_16_RULE[0],
      (uint32_t*)&AHBSC0->SRAM_17_RULE[0],
      (uint32_t*)&AHBSC0->SRAM_18_RULE[0],
      (uint32_t*)&AHBSC0->SRAM_19_RULE[0],
      (uint32_t*)&AHBSC0->SRAM_20_RULE[0],
      (uint32_t*)&AHBSC0->SRAM_21_RULE[0],
      (uint32_t*)&AHBSC0->SRAM_22_RULE[0],
      (uint32_t*)&AHBSC0->SRAM_23_RULE[0],
      (uint32_t*)&AHBSC0->SRAM_24_RULE[0],
      (uint32_t*)&AHBSC0->SRAM_25_RULE[0],
      (uint32_t*)&AHBSC0->SRAM_26_RULE[0],
      (uint32_t*)&AHBSC0->SRAM_27_RULE[0],
      (uint32_t*)&AHBSC0->SRAM_28_RULE[0],
      (uint32_t*)&AHBSC0->SRAM_29_RULE[0],
    };
    int i, j;
    uint32_t ns_region_id       = 0;
    uint32_t ns_region_start_id = 0;
    uint32_t ns_region_end_id   = 0;

    /* Starts changing actual configuration so issue DMB to ensure every transaction has completed by now */
    __DMB();

    //only_tz_configs();

    /* Configuration of AHB Secure Controller
     * Possible values for every memory sector or peripheral rule:
     *  0    Non-secure, user access allowed.
     *  1    Non-secure, privileged access allowed.
     *  2    Secure, user access allowed.
     *  3    Secure, privileged access allowed. */

    /* == ROM region == */

    /* Security access rules for boot ROM memory sectors. Each sector is 8 KB. Up to 32 sectors are supported. */
    /* Security control ROM memory configuration (0x3 = all regions set to secure and privileged user access). */
    for (i = 0; i < ARRAY_SIZE(AHBSC0->ROM_MEM_RULE); i++)
    {
        AHBSC0->ROM_MEM_RULE[i] = 0x33333333U;
    }
    
    /* == Flash region == */
    /* XSPI1: 0x0800 0000 to 0x0BFF FFFF 64 MB
        Region #: size (# of blocks, block size)
        Region 0: 8 MB (32 * 256 KB)
        Region 1: 8 MB (4 * 2 MB)
        Region 2: 16 MB (4 * 4 MB)
        Region 3: 32 MB (4 * 8 MB)
        Region 4: 64 MB (4 * 16 MB) */

    /* 1) Set FLASH memory security access rule configuration to init value (0x3 = all regions set to secure and
     *    privileged user access).
     */
    /* Region XSP1-Region0 has x regions of x KB each, totaling x MB. */
    for (i = 0; i < ARRAY_SIZE(AHBSC0->XSPI1_REGION0_MEM_RULE); i++)
    {
        AHBSC0->XSPI1_REGION0_MEM_RULE[i] = 0x33333333U;
    }
    for (i = 0; i < ARRAY_SIZE(AHBSC0->XSPI1_REGION1_4_MEM_RULE); i++)
    {
        AHBSC0->XSPI1_REGION1_4_MEM_RULE[i].XSPI1_REGION_MEM_RULE0 = 0x00003333U;
    }   
    
     /* Region XSP1-Region0 has x regions of x KB each, totaling x MB. */
    for (i = 0; i < ARRAY_SIZE(AHBSC0->XSPI0_REGION0_MEM_RULE); i++)
    {
        AHBSC0->XSPI0_REGION0_MEM_RULE[i] = 0x33333333U;
    }
    for (i = 0; i < ARRAY_SIZE(AHBSC0->XSPI0_REGION1_4_MEM_RULE); i++)
    {
        AHBSC0->XSPI0_REGION1_4_MEM_RULE[i].XSPI0_REGION_MEM_RULE0 = 0x00003333U;
    }  
    
    /* PKC RAM Rule */
    AHBSC0->PKC_RAM_RULE[0] = 0x00000033U;

    /* 2) Set FLASH memory security access rule configuration (set to non-secure and non-privileged user access
     *    allowed).
     */
    /* == Region 0 == */
    /* The regions have to be alligned to FLASH_REGION0_SUBREGION_SIZE to cover the AHB Flash Region. */
    assert(memory_regions.non_secure_partition_base >= NS_ROM_ALIAS_BASE);
    assert(((memory_regions.non_secure_partition_base - NS_ROM_ALIAS_BASE) % FLASH_REGION0_SUBREGION_SIZE) == 0);
    assert(((memory_regions.non_secure_partition_limit - NS_ROM_ALIAS_BASE + 1) % FLASH_REGION0_SUBREGION_SIZE)
               == 0);
    enable_mem_rule_for_partition(memory_regions.non_secure_partition_base, memory_regions.non_secure_partition_limit);

    /* == SRAM region == */
    /* RAM0 to RAM18 ~1 MB, each 64 KB (32 * 2 KB) */

    /* The regions have to be alligned to 2 kB to cover the AHB RAM Region */
    assert((S_DATA_SIZE % DATA_REGION0_SUBREGION_SIZE) == 0);
    assert(((S_DATA_SIZE + NS_DATA_SIZE) % DATA_REGION0_SUBREGION_SIZE) == 0);

    /* Security access rules for RAM (0x3 = all regions set to secure and privileged user access) */
    for (i = 0; i < (sizeof(ram_rule_sfr)/sizeof(uint32_t*)); i++)
    {
          for (j = 0; j < ARRAY_SIZE(AHBSC0->SRAM_0_RULE); j++)
        {
            *(ram_rule_sfr[i] + j) = 0x33333333U;
        }        
    }
      
    /* == Region 0 == */
    /* RAM memory configuration (set according to region_defs.h and flash_layout.h) */
    ns_region_start_id  = (NS_DATA_START - NS_RAM_ALIAS_BASE) / DATA_REGION0_SUBREGION_SIZE; /* NS starts after S */
    ns_region_end_id    = (NS_DATA_START - NS_RAM_ALIAS_BASE + NS_DATA_SIZE) / DATA_REGION0_SUBREGION_SIZE;

    /* Configure ram non-secure regions*/
    platform_config_non_secure_region(ram_rule_sfr);

    /* HIFI4 TCM Rules*/
    AHBSC0->HIFI4_TCM_RULE[0] = 0x00330033; 
    
    /* XSPI2 control on 0 and 1 bits.*/
    AHBSC0->COMPUTE2MEDIA_RULE = 0x00000000;

    /* PBUS domain on on 0 and 1 bits.*/
    AHBSC0->MBUS2PBUS_RULE = 0x00000000;
    
#if TARGET_DEBUG_LOG
    SPMLOG_DBGMSG("=== [AHB MPC NS] =======\r\n");
    SPMLOG_DBGMSGVAL("NS ROM starts from : ", memory_regions.non_secure_partition_base);
    SPMLOG_DBGMSGVAL("NS ROM ends at : ",
                     memory_regions.non_secure_partition_base + memory_regions.non_secure_partition_limit);
    SPMLOG_DBGMSGVAL("NS DATA start from : ", NS_DATA_START);
    SPMLOG_DBGMSGVAL("NS DATA ends at : ", NS_DATA_START + NS_DATA_LIMIT);
#endif /* TARGET_DEBUG_LOG */

    /* Add barriers to assure the MPC configuration is done before continue the execution. */
    __DSB();
    __ISB();

    return ARM_DRIVER_OK;
}
   
/*---------------------- PPC configuration functions -------------------------*/
int32_t ppc_init_cfg(void)
{
    int i;
     /* Configuration of AHB Secure Controller. Grant user access to peripherals.
     * Possible values for every memory sector or peripheral rule:
     *  0    Non-secure, user access allowed.
     *  1    Non-secure, privileged access allowed.
     *  2    Secure, user access allowed.
     *  3    Secure, privileged access allowed. */
    
    /* Write access attributes for AHBSC0 module are tier-4 (secure privileged). */ 
    
    /* Security access rules for APB Bridge 0 peripherals. */
    AHBSC0->AHB_PERIPHERAL0_SLAVE_RULE1 =
        (0x33333333U) |            
        AHBSC0_AHB_PERIPHERAL0_SLAVE_RULE1_LP_FLEXCOMM0(0x00)  |  
        AHBSC0_AHB_PERIPHERAL0_SLAVE_RULE1_LP_FLEXCOMM1(0x00)  |  
        AHBSC0_AHB_PERIPHERAL0_SLAVE_RULE1_LP_FLEXCOMM2(0x00)  | 
        AHBSC0_AHB_PERIPHERAL0_SLAVE_RULE1_LP_FLEXCOMM3(0x00)  | 
        AHBSC0_AHB_PERIPHERAL0_SLAVE_RULE1_SCT0(0x00)          | 
        AHBSC0_AHB_PERIPHERAL0_SLAVE_RULE1_CDOG0(0x03)         |       
        AHBSC0_AHB_PERIPHERAL0_SLAVE_RULE1_CDOG1(0x00)         |        
        AHBSC0_AHB_PERIPHERAL0_SLAVE_RULE1_CDOG2(0x00);          
    

    for (i = 0; i < ARRAY_SIZE(AHBSC0->AHB_PERIPHERAL0_SLAVE_GPION_RULE); i++)
    {    
      AHBSC0->AHB_PERIPHERAL0_SLAVE_GPION_RULE[i].AHB_PERIPHERAL0_SLAVE_GPIO_RULE =    
        (0x00000033U) |                                                                 
          AHBSC0_AHB_PERIPHERAL0_SLAVE_GPIO_RULE_GPIOn(0x00)  |  
          AHBSC0_AHB_PERIPHERAL0_SLAVE_GPIO_RULE_GPIOn_ALIAS(0x00); 
    }

    
    AHBSC0->AHB_PERIPHERAL1_SLAVE_RULE =
    (0x33333300U) |            
    AHBSC0_AHB_PERIPHERAL1_SLAVE_RULE_eDMA1_CH15(0x00) |  
    AHBSC0_AHB_PERIPHERAL1_SLAVE_RULE_FLEXCOMM4(0x00)  |  
    AHBSC0_AHB_PERIPHERAL1_SLAVE_RULE_FLEXCOMM5(0x00)  | 
    AHBSC0_AHB_PERIPHERAL1_SLAVE_RULE_FLEXCOMM6(0x00)  | 
    AHBSC0_AHB_PERIPHERAL1_SLAVE_RULE_FLEXCOMM7(0x00)  | 
    AHBSC0_AHB_PERIPHERAL1_SLAVE_RULE_NPU(0x00);  
    
    AHBSC0->AIPS1_BRIDGE_GROUP0_MEM_RULE0 =
    (0x33333333U) |            
    AHBSC0_AIPS1_BRIDGE_GROUP0_MEM_RULE0_eDMA0_MP(0x00)  |  
    AHBSC0_AIPS1_BRIDGE_GROUP0_MEM_RULE0_eDMA0_CH0(0x00)  |  
    AHBSC0_AIPS1_BRIDGE_GROUP0_MEM_RULE0_eDMA0_CH1(0x00)  | 
    AHBSC0_AIPS1_BRIDGE_GROUP0_MEM_RULE0_eDMA0_CH2(0x00)  | 
    AHBSC0_AIPS1_BRIDGE_GROUP0_MEM_RULE0_eDMA0_CH3(0x00)  | 
    AHBSC0_AIPS1_BRIDGE_GROUP0_MEM_RULE0_eDMA0_CH4(0x00)  |
    AHBSC0_AIPS1_BRIDGE_GROUP0_MEM_RULE0_eDMA0_CH5(0x00)  |
    AHBSC0_AIPS1_BRIDGE_GROUP0_MEM_RULE0_eDMA0_CH6(0x00);      
    

    AHBSC0->AIPS1_BRIDGE_GROUP0_MEM_RULE1 =
    (0x33333333U) |            
    AHBSC0_AIPS1_BRIDGE_GROUP0_MEM_RULE1_eDMA0_CH7(0x00)  |  
    AHBSC0_AIPS1_BRIDGE_GROUP0_MEM_RULE1_eDMA0_CH8(0x00)  |  
    AHBSC0_AIPS1_BRIDGE_GROUP0_MEM_RULE1_eDMA0_CH9(0x00)  | 
    AHBSC0_AIPS1_BRIDGE_GROUP0_MEM_RULE1_eDMA0_CH10(0x00) | 
    AHBSC0_AIPS1_BRIDGE_GROUP0_MEM_RULE1_eDMA_CH11(0x00)  | 
    AHBSC0_AIPS1_BRIDGE_GROUP0_MEM_RULE1_eDMA0_CH12(0x00) |
    AHBSC0_AIPS1_BRIDGE_GROUP0_MEM_RULE1_eDMA0_CH13(0x00) |
    AHBSC0_AIPS1_BRIDGE_GROUP0_MEM_RULE1_eDMA0_CH14(0x00);        
    
    AHBSC0->AIPS1_BRIDGE_GROUP0_MEM_RULE2 =
    (0x00033333U) |            
    AHBSC0_AIPS1_BRIDGE_GROUP0_MEM_RULE2_eDMA0_CH15(0x00)  |  
    AHBSC0_AIPS1_BRIDGE_GROUP0_MEM_RULE2_CRC(0x00)  |  
    AHBSC0_AIPS1_BRIDGE_GROUP0_MEM_RULE2_SAI0(0x00)  | 
    AHBSC0_AIPS1_BRIDGE_GROUP0_MEM_RULE2_SAI1(0x00) | 
    AHBSC0_AIPS1_BRIDGE_GROUP0_MEM_RULE2_SAI2(0x00);        
    
    
    AHBSC0->AIPS1_BRIDGE_GROUP1_MEM_RULE0 =
    (0x33333333U) |            
    AHBSC0_AIPS1_BRIDGE_GROUP1_MEM_RULE0_DMA_1_MP(0x00)  |  
    AHBSC0_AIPS1_BRIDGE_GROUP1_MEM_RULE0_DMA_1_CH0(0x00) |  
    AHBSC0_AIPS1_BRIDGE_GROUP1_MEM_RULE0_DMA_1_CH1(0x00) | 
    AHBSC0_AIPS1_BRIDGE_GROUP1_MEM_RULE0_DMA_1_CH2(0x00) | 
    AHBSC0_AIPS1_BRIDGE_GROUP1_MEM_RULE0_DMA_1_CH3(0x00) | 
    AHBSC0_AIPS1_BRIDGE_GROUP1_MEM_RULE0_DMA_1_CH4(0x00) |
    AHBSC0_AIPS1_BRIDGE_GROUP1_MEM_RULE0_DMA_1_CH5(0x00) |
    AHBSC0_AIPS1_BRIDGE_GROUP1_MEM_RULE0_DMA_1_CH6(0x00);      

    AHBSC0->AIPS1_BRIDGE_GROUP1_MEM_RULE1 =
    (0x33333333U) |            
    AHBSC0_AIPS1_BRIDGE_GROUP1_MEM_RULE1_DMA_1_CH7(0x00)  |  
    AHBSC0_AIPS1_BRIDGE_GROUP1_MEM_RULE1_DMA_1_CH8(0x00)  |  
    AHBSC0_AIPS1_BRIDGE_GROUP1_MEM_RULE1_DMA_1_CH9(0x00)  | 
    AHBSC0_AIPS1_BRIDGE_GROUP1_MEM_RULE1_DMA_1_CH10(0x00) | 
    AHBSC0_AIPS1_BRIDGE_GROUP1_MEM_RULE1_DMA_1_CH11(0x00) | 
    AHBSC0_AIPS1_BRIDGE_GROUP1_MEM_RULE1_DMA_1_CH12(0x00) |
    AHBSC0_AIPS1_BRIDGE_GROUP1_MEM_RULE1_DMA_1_CH13(0x00) |
    AHBSC0_AIPS1_BRIDGE_GROUP1_MEM_RULE1_DMA_1_CH14(0x00);      


    AHBSC0->AHB_SECURE_CTRL_PERIPHERAL_RULE0 =
    (0x00003333U) |            
    AHBSC0_AHB_SECURE_CTRL_PERIPHERAL_RULE0_RULE0(0x00)  |  
    AHBSC0_AHB_SECURE_CTRL_PERIPHERAL_RULE0_RULE1(0x00)  |  
    AHBSC0_AHB_SECURE_CTRL_PERIPHERAL_RULE0_RULE2(0x00)  | 
    AHBSC0_AHB_SECURE_CTRL_PERIPHERAL_RULE0_RULE3(0x00);   

    AHBSC0->AHB_PERIPHERAL2_SLAVE_RULE0 =
    (0x33333300U) |            
    AHBSC0_AHB_PERIPHERAL2_SLAVE_RULE0_DEBUG_MAILBOX(0x00)  |  
    AHBSC0_AHB_PERIPHERAL2_SLAVE_RULE0_FLEXCOMM8(0x00)  |  
    AHBSC0_AHB_PERIPHERAL2_SLAVE_RULE0_FLEXCOMM9(0x00)  | 
    AHBSC0_AHB_PERIPHERAL2_SLAVE_RULE0_FLEXCOMM10(0x00) | 
    AHBSC0_AHB_PERIPHERAL2_SLAVE_RULE0_FLEXCOMM11(0x00) | 
    AHBSC0_AHB_PERIPHERAL2_SLAVE_RULE0_FLEXCOMM12(0x00);  

    AHBSC0->AHB_PERIPHERAL2_SLAVE_RULE1 =
    (0x00000003U) |             
    AHBSC0_AHB_PERIPHERAL2_SLAVE_RULE1_FLEXCOMM13(0x00);  
    
    
    AHBSC0->AIPS0_BRIDGE_GROUP0_MEM_RULE0 =
    (0x30330000U) |            
    AHBSC0_AIPS0_BRIDGE_GROUP0_MEM_RULE0_XSPI0(0x00)  |  
    AHBSC0_AIPS0_BRIDGE_GROUP0_MEM_RULE0_XSPI1(0x00)  |  
    AHBSC0_AIPS0_BRIDGE_GROUP0_MEM_RULE0_TRNG(0x00);  
    
    AHBSC0->AIPS0_BRIDGE_GROUP0_MEM_RULE1 =
    (0x33330333U) |            
    AHBSC0_AIPS0_BRIDGE_GROUP0_MEM_RULE1_CMX_PERFMON0(0x00)  |  
    AHBSC0_AIPS0_BRIDGE_GROUP0_MEM_RULE1_CMX_PERFMON1(0x00)  |  
    AHBSC0_AIPS0_BRIDGE_GROUP0_MEM_RULE1_CMX_PERFMON2(0x00)  | 
    AHBSC0_AIPS0_BRIDGE_GROUP0_MEM_RULE1_CMX_PERFMON3(0x00) | 
    AHBSC0_AIPS0_BRIDGE_GROUP0_MEM_RULE1_MU4_A(0x00) | 
    AHBSC0_AIPS0_BRIDGE_GROUP0_MEM_RULE1_MU4_B(0x00) |
    AHBSC0_AIPS0_BRIDGE_GROUP0_MEM_RULE1_SEMA42_4(0x00);      
    
    AHBSC0->AIPS0_BRIDGE_GROUP1_MEM_RULE0 =
    (0x33333333U) |            
    AHBSC0_AIPS0_BRIDGE_GROUP1_MEM_RULE0_ELS(0x03)         |  
    AHBSC0_AIPS0_BRIDGE_GROUP1_MEM_RULE0_ELS_ALIAS1(0x00)  |  
    AHBSC0_AIPS0_BRIDGE_GROUP1_MEM_RULE0_ELS_ALIAS2(0x00)  | 
    AHBSC0_AIPS0_BRIDGE_GROUP1_MEM_RULE0_ELS_ALIAS3(0x00)  |
    AHBSC0_AIPS0_BRIDGE_GROUP1_MEM_RULE0_PUF(0x03)         |
    AHBSC0_AIPS0_BRIDGE_GROUP1_MEM_RULE0_PUF_ALIAS1(0x00)  | 
    AHBSC0_AIPS0_BRIDGE_GROUP1_MEM_RULE0_PUF_ALIAS2(0x00)  |
    AHBSC0_AIPS0_BRIDGE_GROUP1_MEM_RULE0_PUF_ALIAS3(0x00);      
        
    AHBSC0->APB_SLAVE_GROUP0_RULE0 =
    (0x00333333U) |            
    AHBSC0_APB_SLAVE_GROUP0_RULE0_RSTCTL0(0x00)    |  
    AHBSC0_APB_SLAVE_GROUP0_RULE0_CLKCTL0(0x00)    |  
    AHBSC0_APB_SLAVE_GROUP0_RULE0_SYSCON0(0x00)    | 
    AHBSC0_APB_SLAVE_GROUP0_RULE0_SLEEPCON0(0x00)  |
    AHBSC0_APB_SLAVE_GROUP0_RULE0_IOCON0(0x00)     |
    AHBSC0_APB_SLAVE_GROUP0_RULE0_PVTS0(0x00);      
    
    AHBSC0->APB_SLAVE_GROUP0_RULE1 =
    (0x33000000U) |            
    AHBSC0_APB_SLAVE_GROUP0_RULE1_UTICK0(0x00)    |  
    AHBSC0_APB_SLAVE_GROUP0_RULE1_WWDT0(0x00);      
    
    AHBSC0->APB_SLAVE_GROUP0_RULE2 =
    (0x00003330U) |            
    AHBSC0_APB_SLAVE_GROUP0_RULE2_GDET0(0x03)    |  
    AHBSC0_APB_SLAVE_GROUP0_RULE2_PKC(0x03)      |  
    AHBSC0_APB_SLAVE_GROUP0_RULE2_ITRC(0x03);        
    
    AHBSC0->APB_SLAVE_GROUP0_RULE3 =
    (0x00000303U) |            
    AHBSC0_APB_SLAVE_GROUP0_RULE3_OCOTP(0x00)    |  
    AHBSC0_APB_SLAVE_GROUP0_RULE3_SAFO_SGI(0x00);       
    
    AHBSC0->APB_SLAVE_GROUP1_RULE0 =
    (0x03303000U) |            
    AHBSC0_APB_SLAVE_GROUP1_RULE0_FREQME(0x00)    |  
    AHBSC0_APB_SLAVE_GROUP1_RULE0_PINT0(0x00)     |  
    AHBSC0_APB_SLAVE_GROUP1_RULE0_INPUTMUX0(0x00);      


    AHBSC0->APB_SLAVE_GROUP1_RULE1 =
    (0x03333333U) |            
    AHBSC0_APB_SLAVE_GROUP1_RULE1_CTIMER0(0x03)    |  
    AHBSC0_APB_SLAVE_GROUP1_RULE1_CTIMER1(0x00)    |  
    AHBSC0_APB_SLAVE_GROUP1_RULE1_CTIMER2(0x00)    | 
    AHBSC0_APB_SLAVE_GROUP1_RULE1_CTIMER3(0x00)    |
    AHBSC0_APB_SLAVE_GROUP1_RULE1_CTIMER4(0x00)    |
    AHBSC0_APB_SLAVE_GROUP1_RULE1_MRT0(0x00)       |
    AHBSC0_APB_SLAVE_GROUP1_RULE1_WWDT1(0x00);  

    AHBSC0->APB_SLAVE_GROUP1_RULE2 =
    (0x33333033U) |            
    AHBSC0_APB_SLAVE_GROUP1_RULE2_I3C0(0x00)       |  
    AHBSC0_APB_SLAVE_GROUP1_RULE2_MMU0(0x00)       |  
    AHBSC0_APB_SLAVE_GROUP1_RULE2_MMU1(0x00)       | 
    AHBSC0_APB_SLAVE_GROUP1_RULE2_XCACHE0(0x00)    |
    AHBSC0_APB_SLAVE_GROUP1_RULE2_XCACHE1(0x00)    |
    AHBSC0_APB_SLAVE_GROUP1_RULE2_XCACHE2(0x00)    |
    AHBSC0_APB_SLAVE_GROUP1_RULE2_XCACHE3(0x00);  

    AHBSC0->APB_SLAVE_GROUP1_RULE3 =
    (0x00000003U) |            
    AHBSC0_APB_SLAVE_GROUP1_RULE3_I3C1(0x00);  
    
    
    AHBSC0->SHARED_APB_SLAVE_GROUP0_RULE0 =
    (0x33333333U) |            
    AHBSC0_SHARED_APB_SLAVE_GROUP0_RULE0_RSTCTL3(0x00)       |  
    AHBSC0_SHARED_APB_SLAVE_GROUP0_RULE0_RSTCTL2(0x00)       |  
    AHBSC0_SHARED_APB_SLAVE_GROUP0_RULE0_CLKCTL2(0x00)       | 
    AHBSC0_SHARED_APB_SLAVE_GROUP0_RULE0_CLKCTL3(0x00)       |
    AHBSC0_SHARED_APB_SLAVE_GROUP0_RULE0_IOPCTL1(0x00)       |
    AHBSC0_SHARED_APB_SLAVE_GROUP0_RULE0_OSC32KNP(0x00)      |
    AHBSC0_SHARED_APB_SLAVE_GROUP0_RULE0_SYSCON2(0x00)       |
    AHBSC0_SHARED_APB_SLAVE_GROUP0_RULE0_SYSCON3(0x00);      
    
    AHBSC0->SHARED_APB_SLAVE_GROUP0_RULE1 =
    (0x00003033U) |            
    AHBSC0_SHARED_APB_SLAVE_GROUP0_RULE1_GDET3(0x00)      |  
    AHBSC0_SHARED_APB_SLAVE_GROUP0_RULE1_RTC0(0x00)       |  
    AHBSC0_SHARED_APB_SLAVE_GROUP0_RULE1_RTC1(0x00);     


    AHBSC0->AIPS2_BRIDGE_GROUP0_MEM_RULE0 =
    (0x33030303U) |            
    AHBSC0_AIPS2_BRIDGE_GROUP0_MEM_RULE0_MU0_A(0x00)       |  
    AHBSC0_AIPS2_BRIDGE_GROUP0_MEM_RULE0_MU1_A(0x00)       |  
    AHBSC0_AIPS2_BRIDGE_GROUP0_MEM_RULE0_MU2_A(0x00)       | 
    AHBSC0_AIPS2_BRIDGE_GROUP0_MEM_RULE0_SEMA42_0(0x00)    |
    AHBSC0_AIPS2_BRIDGE_GROUP0_MEM_RULE0_OSTIMER(0x00);    

    AHBSC0->AIPS2_BRIDGE_GROUP0_MEM_RULE1 =
    (0x33333003U) |            
    AHBSC0_AIPS2_BRIDGE_GROUP0_MEM_RULE1_OSTIMER_ALIAS1(0x00)  |  
    AHBSC0_AIPS2_BRIDGE_GROUP0_MEM_RULE1_ACMP0(0x00)           |  
    AHBSC0_AIPS2_BRIDGE_GROUP0_MEM_RULE1_ADC0(0x00)            | 
    AHBSC0_AIPS2_BRIDGE_GROUP0_MEM_RULE1_SDADC(0x00)           |
    AHBSC0_AIPS2_BRIDGE_GROUP0_MEM_RULE1_MICFIL(0x00)          |
    AHBSC0_AIPS2_BRIDGE_GROUP0_MEM_RULE1_PMC0(0x00);   

    AHBSC0->AIPS2_BRIDGE_GROUP1_MEM_RULE0 =
    (0x00003000U) |            
    AHBSC0_AIPS2_BRIDGE_GROUP1_MEM_RULE0_LPI2C15(0x00);   


    AHBSC0->AHB_SECURE_CTRL_SENSE_RULE0 =
    (0x00003333U) |            
    AHBSC0_AHB_SECURE_CTRL_SENSE_RULE0_RULE0(0x00)           |  
    AHBSC0_AHB_SECURE_CTRL_SENSE_RULE0_RULE1(0x00)           |  
    AHBSC0_AHB_SECURE_CTRL_SENSE_RULE0_RULE2(0x00)           | 
    AHBSC0_AHB_SECURE_CTRL_SENSE_RULE0_RULE3(0x00);   


    AHBSC0->MEDIA_AHB_PERIPHERAL_SLAVE_RULE0 =
    (0x00000003U) |            
    AHBSC0_MEDIA_AHB_PERIPHERAL_SLAVE_RULE0_MEDIA_AHB_PERIPHERAL(0x00);  


    AHBSC0->MEDIA_APB_PERIPHERAL_SLAVE_RULE0 =
    (0x00000003U) |            
    AHBSC0_MEDIA_APB_PERIPHERAL_SLAVE_RULE0_MEDIA_APB_PERIPHERAL(0x00);  
    
    
    AHBSC0->EZHV_SRAM_RULE0 =
    (0x00000003U) |            
    AHBSC0_EZHV_SRAM_RULE0_EZHV_SRAM(0x00);       
    
    
     /* Set Master Security Level of PKC and ELS Security Level to secure and privileged master, and lock it. */
    GlikeyWriteEnable(GLIKEY0, 6U); /* Enable write to MASTER_SEC_LEVEL */
    SECURE_READ_MODIFY_WRITE_REGISTER(&(AHBSC0->MASTER_SEC_LEVEL),
        (AHBSC0_MASTER_SEC_LEVEL_HIFI4_MASK |
        AHBSC0_MASTER_SEC_LEVEL_MEDIA_MASK|
        AHBSC0_MASTER_SEC_LEVEL_PKC_MASK  |
        AHBSC0_MASTER_SEC_LEVEL_DMA0_MASK  |
        AHBSC0_MASTER_SEC_LEVEL_DMA1_MASK  |
        AHBSC0_MASTER_SEC_LEVEL_ETR_MASK ),
        (AHBSC0_MASTER_SEC_LEVEL_HIFI4(0x00) |        /* MASTER SEC LEVEL HIFI4 */
        AHBSC0_MASTER_SEC_LEVEL_MEDIA(0x00) |        /* MASTER SEC LEVEL MEDIA */
        AHBSC0_MASTER_SEC_LEVEL_PKC(0x03)   |        /* MASTER SEC LEVEL PKC  */
        AHBSC0_MASTER_SEC_LEVEL_DMA0(0x00)  |        /* MASTER SEC LEVEL DMA0  */
        AHBSC0_MASTER_SEC_LEVEL_DMA1(0x00)   |       /* MASTER SEC LEVEL DMA1  */
        AHBSC0_MASTER_SEC_LEVEL_ETR(0x00)));         /* MASTER SEC LEVEL ETR */

    SECURE_READ_MODIFY_WRITE_REGISTER(&(AHBSC0->MASTER_SEC_ANTI_POL_REG),
        (AHBSC0_MASTER_SEC_ANTI_POL_REG_HIFI4_MASK |
        AHBSC0_MASTER_SEC_ANTI_POL_REG_MEDIA_MASK|
        AHBSC0_MASTER_SEC_ANTI_POL_REG_PKC_MASK  |
        AHBSC0_MASTER_SEC_ANTI_POL_REG_DMA0_MASK  |
        AHBSC0_MASTER_SEC_ANTI_POL_REG_DMA1_MASK  |
        AHBSC0_MASTER_SEC_ANTI_POL_REG_ETR_MASK ),
        (AHBSC0_MASTER_SEC_ANTI_POL_REG_HIFI4(0x00) |        /* MASTER SEC ANTI_POL_REG HIFI4 */
        AHBSC0_MASTER_SEC_ANTI_POL_REG_MEDIA(0x00) |        /* MASTER SEC ANTI_POL_REG MEDIA */
        AHBSC0_MASTER_SEC_ANTI_POL_REG_PKC(0x03)   |        /* MASTER SEC ANTI_POL_REG PKC  */
        AHBSC0_MASTER_SEC_ANTI_POL_REG_DMA0(0x00)  |        /* MASTER SEC ANTI_POL_REG DMA0  */
        AHBSC0_MASTER_SEC_ANTI_POL_REG_DMA1(0x00)   |       /* MASTER SEC ANTI_POL_REG DMA1  */
        AHBSC0_MASTER_SEC_ANTI_POL_REG_ETR(0x00)));         /* MASTER SEC ANTI_POL_REG ETR */
    
    
    
    /*Do here AHBSC4 configs, if required*/
    /* AHBSC4: Security level configuration of MPC checker */
    clear_ahbsc4_sram_rule();
    AHBSC4->EZHV_SRAM_RULE[0] = 0;
    AHBSC4->EZHV_SRAM_RULE[1] = 0;
    AHBSC4->EZHV_SRAM_RULE[2] = 0;
    AHBSC4->EZHV_SRAM_RULE[3] = 0;
    AHBSC4->XSPI2_REGION0_MEM_RULE[0] = 0;
    AHBSC4->XSPI2_REGION0_MEM_RULE[1] = 0;
    AHBSC4->XSPI2_REGION0_MEM_RULE[2] = 0;
    AHBSC4->XSPI2_REGION0_MEM_RULE[3] = 0;
    AHBSC4->XSPI2_REGION1_5_MEM_RULE[0].XSPI2_REGION_MEM_RULE0 = 0;
    AHBSC4->XSPI2_REGION1_5_MEM_RULE[1].XSPI2_REGION_MEM_RULE0 = 0;
    AHBSC4->XSPI2_REGION1_5_MEM_RULE[2].XSPI2_REGION_MEM_RULE0 = 0;
    AHBSC4->XSPI2_REGION1_5_MEM_RULE[3].XSPI2_REGION_MEM_RULE0 = 0;
    AHBSC4->XSPI2_REGION1_5_MEM_RULE[4].XSPI2_REGION_MEM_RULE0 = 0;

    /* AHBSC4: Security level configuration of PPC checker */
    AHBSC4->MEDIA_APB_SLAVE_GROUP0_RULE0 = 0x00033000U;
    AHBSC4->MEDIA_APB_SLAVE_GROUP0_RULE1 = 0x33333330U;
    AHBSC4->AHB_SECURE_CTRL_PERIPHERAL_RULE0 = 0;
    AHBSC4->AHB_PERIPHERAL_SLAVE_P0_SLAVE_RULE0 = 0x00000303U;
    AHBSC4->AIPS4_BRIDGE_MEM_RULE0 = 0x00300003U;
    AHBSC4->AIPS4_BRIDGE_MEM_RULE1 = 0x33333300U;
    AHBSC4->AHB_PERIPHERAL_SLAVE_P1_SLAVE_RULE0 = 0x00000300U;

    /* AHBSC4: Security level configuration of masters */
    GlikeyWriteEnable(GLIKEY2, 3U); /* Enable write to MASTER_SEC_LEVEL */
    AHBSC4->MASTER_SEC_LEVEL = 0;
    AHBSC4->MASTER_SEC_ANTI_POL_REG = 0xFFFFFFFFU;
    
    /* Do here AHBSC3 configs, if required, currently configs are used as configured by ROM*/    
    
    /* ---- Complete the AHBSC0 configurations ---- */
    
    /* Enable write to arbitration for masters accessing SRAM */
    GlikeyWriteEnable(GLIKEY0, 7U); 
    AHBSC0->COMPUTE_ARB0RAM_ACCESS_ENABLE = 0x0003FFFFU;
    AHBSC0->SENSE_ARB0RAM_ACCESS_ENABLE = 0x0003FFFFU;
    AHBSC0->MEDIA_ARB0RAM_ACCESS_ENABLE = 0;
    AHBSC0->NPU_ARB0RAM_ACCESS_ENABLE = 0;
    AHBSC0->HIFI4_ARB0RAM_ACCESS_ENABLE = 0;
    
    /* m33 lock control register (0x2: CM33 Non-secure VTOR is not locked.) */
    GlikeyWriteEnable(GLIKEY0, 5U); /* Enable write to LOCK_SAU (bits 9-8) */
    AHBSC0->CPU0_LOCK_REG = (AHBSC0->CPU0_LOCK_REG & 0xFFFFFCFFU) | 0x00000200U;
    GlikeyWriteEnable(GLIKEY0, 4U); /* Enable write to LOCK_S_MPU (bits 7-6) and LOCK_S_VTAIRCR (bits 5-4) */
    AHBSC0->CPU0_LOCK_REG = (AHBSC0->CPU0_LOCK_REG & 0xFFFFFF0FU) | 0x000000A0U;
    GlikeyWriteEnable(GLIKEY0, 3U); /* Enable write to LOCK_NS_MPU (bits 3-2) and LOCK_NS_VTOR (bits 1-0) */
    AHBSC0->CPU0_LOCK_REG = (AHBSC0->CPU0_LOCK_REG & 0xFFFFFFF0U) | 0x0000000AU;
    
    GlikeyWriteEnable(GLIKEY0, 2U); /* Enable write to IDAU_ALL_NS (bits 15-14) */
    GlikeyWriteEnable(GLIKEY0, 1U); /* Enable write to MISC_CTRL_REG, MISC_CTRL_DP_REG (bits 11-2) */
    GlikeyWriteEnable(GLIKEY0, 0U); /* Enable write to WRITE_LOCK (bits 1-0) */
    
    /* Update AHB Secure control register */
    SECURE_READ_MODIFY_WRITE_REGISTER(&(AHBSC0->MISC_CTRL_REG),
        ~(AHBSC0_MISC_CTRL_REG_WRITE_LOCK_MASK                |
          AHBSC0_MISC_CTRL_REG_ENABLE_SECURE_CHECKING_MASK    |
          AHBSC0_MISC_CTRL_REG_ENABLE_S_PRIV_CHECK_MASK       |
          AHBSC0_MISC_CTRL_REG_ENABLE_NS_PRIV_CHECK_MASK      |
          AHBSC0_MISC_CTRL_REG_DISABLE_STRICT_MODE_MASK       |
          AHBSC0_MISC_CTRL_REG_DISABLE_VIOLATION_ABORT_MASK   |
          AHBSC0_MISC_CTRL_REG_IDAU_ALL_NS_MASK               ),
         (AHBSC0_MISC_CTRL_REG_WRITE_LOCK              (0x2U) |         /* 2 = Writes to this register and to the Memory and Peripheral RULE registers are allowed */
          AHBSC0_MISC_CTRL_REG_ENABLE_SECURE_CHECKING  (0x1U) |         /* 1 = Enable Secure Checking (restrictive mode) */
          AHBSC0_MISC_CTRL_REG_ENABLE_S_PRIV_CHECK     (0x2U) |         /* 2 = Disabled Secure Privilege Checking */
          AHBSC0_MISC_CTRL_REG_ENABLE_NS_PRIV_CHECK    (0x2U) |         /* 2 = Disabled Non-Secure Privilege Checking */
          AHBSC0_MISC_CTRL_REG_DISABLE_VIOLATION_ABORT (0x2U) |         /* 2 = The violation detected by the secure checker will cause an abort. */
          AHBSC0_MISC_CTRL_REG_DISABLE_STRICT_MODE     (0x1U) |         /* 1 = AHB master in tier mode. Can read and write to memories at same or below level. */
          AHBSC0_MISC_CTRL_REG_IDAU_ALL_NS             (0x2U)));        /* 2 = IDAU is enabled (restrictive mode) */

    /* Secure control duplicate register */
    SECURE_READ_MODIFY_WRITE_REGISTER(&(AHBSC0->MISC_CTRL_DP_REG),
        ~(AHBSC0_MISC_CTRL_DP_REG_WRITE_LOCK_MASK                |
          AHBSC0_MISC_CTRL_DP_REG_ENABLE_SECURE_CHECKING_MASK    |
          AHBSC0_MISC_CTRL_DP_REG_ENABLE_S_PRIV_CHECK_MASK       |
          AHBSC0_MISC_CTRL_DP_REG_ENABLE_NS_PRIV_CHECK_MASK      |
          AHBSC0_MISC_CTRL_DP_REG_DISABLE_STRICT_MODE_MASK       |
          AHBSC0_MISC_CTRL_DP_REG_DISABLE_VIOLATION_ABORT_MASK   |
          AHBSC0_MISC_CTRL_DP_REG_IDAU_ALL_NS_MASK               ),
         (AHBSC0_MISC_CTRL_DP_REG_WRITE_LOCK              (0x2U) |      /* 2 = Writes to this register and to the Memory and Peripheral RULE registers are allowed */
          AHBSC0_MISC_CTRL_DP_REG_ENABLE_SECURE_CHECKING  (0x1U) |      /* 1 = Enable Secure Checking (restrictive mode) */
          AHBSC0_MISC_CTRL_DP_REG_ENABLE_S_PRIV_CHECK     (0x2U) |      /* 2 = Disabled Secure Privilege Checking */
          AHBSC0_MISC_CTRL_DP_REG_ENABLE_NS_PRIV_CHECK    (0x2U) |      /* 2 = Disabled Non-Secure Privilege Checking */
          AHBSC0_MISC_CTRL_DP_REG_DISABLE_VIOLATION_ABORT (0x2U) |      /* 2 = The violation detected by the secure checker will cause an abort. */
          AHBSC0_MISC_CTRL_DP_REG_DISABLE_STRICT_MODE     (0x1U) |      /* 1 = AHB master in tier mode. Can read and write to memories at same or below level. */
          AHBSC0_MISC_CTRL_DP_REG_IDAU_ALL_NS             (0x2U)));     /* 2 = IDAU is enabled (restrictive mode) */
    
    GlikeyWriteEnable(GLIKEY2, 2U); /* Enable write to IDAU_ALL_NS (bits 15-14) */
    GlikeyWriteEnable(GLIKEY2, 1U); /* Enable write to DISABLE_STRICT_MODE (bits 11-10) */
    GlikeyWriteEnable(GLIKEY2, 1U); /* Enable write to MISC_CTRL_REG, MISC_CTRL_DP_REG (bits 9-2) */
    GlikeyWriteEnable(GLIKEY2, 0U); /* Enable write to WRITE_LOCK (bits 1-0) */
    /* Clear GLIKEY protection */
    GlikeyClearConfig(GLIKEY0);
    GlikeyClearConfig(GLIKEY2);


    return ARM_DRIVER_OK;
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

