/*
 * Copyright (c) 2017-2023 Arm Limited. All rights reserved.
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

#ifndef __REGION_DEFS_H__
#define __REGION_DEFS_H__

#include "flash_layout.h"

#ifdef ENABLE_HEAP
    #define S_HEAP_SIZE             (0x0000800)
#endif

#define S_MSP_STACK_SIZE        (0x0000800)
#define S_PSP_STACK_SIZE        (0x0000600)

#define NS_HEAP_SIZE            (0x0000800)
#define NS_STACK_SIZE           (0x0000400)

#define S_IMAGE_PRIMARY_PARTITION_OFFSET (0x4000)

/* The SRAM region [0x00000-0x10000] is reserved for RAM execution. */
#define S_DATA_OFFSET    (S_RAM_CODE_SIZE)

#ifndef LINK_TO_SECONDARY_PARTITION
#define NS_IMAGE_PRIMARY_PARTITION_OFFSET   (FLASH_AREA_0_OFFSET + S_IMAGE_PRIMARY_PARTITION_OFFSET + FLASH_S_PARTITION_SIZE)
#else
#define NS_IMAGE_PRIMARY_PARTITION_OFFSET   (FLASH_AREA_2_OFFSET + FLASH_S_PARTITION_SIZE)
#endif /* !LINK_TO_SECONDARY_PARTITION */

/* Boot partition structure if MCUBoot is used:
 * 0x0_0000 Bootloader header
 * 0x0_0400 Image area
 * 0x1_FC00 Trailer
 */
/* IMAGE_CODE_SIZE is the space available for the software binary image. It is less than
 * the FLASH_S_PARTITION_SIZE + FLASH_NS_PARTITION_SIZE because we reserve space for the image header and trailer
 * introduced by the bootloader.
 */
#ifdef BL2
#define BL2_HEADER_SIZE      (0x400)       /* 1 KB */
#define BL2_TRAILER_SIZE     (0x400)       /* 1 KB */
#else
/* No header if no bootloader, but keep IMAGE_CODE_SIZE the same */
#define BL2_HEADER_SIZE      (0x0)
#define BL2_TRAILER_SIZE     (0x0)
#endif /* BL2 */

#define IMAGE_S_CODE_SIZE   (FLASH_S_PARTITION_SIZE - BL2_HEADER_SIZE - BL2_TRAILER_SIZE)
#define IMAGE_NS_CODE_SIZE  (FLASH_NS_PARTITION_SIZE - BL2_HEADER_SIZE - BL2_TRAILER_SIZE)

#define CMSE_VENEER_REGION_SIZE     (0x368)

/* Alias definitions for secure and non-secure areas*/
#define S_ROM_ALIAS(x)      (S_ROM_ALIAS_BASE + (x))
#define NS_ROM_ALIAS(x)     (NS_ROM_ALIAS_BASE + (x))

#define S_RAM_ALIAS(x)      (S_RAM_ALIAS_BASE + (x))
#define NS_RAM_ALIAS(x)     (NS_RAM_ALIAS_BASE + (x))

/* Secure regions */
#define S_IMAGE_PRIMARY_AREA_OFFSET     (S_IMAGE_PRIMARY_PARTITION_OFFSET + BL2_HEADER_SIZE)
#define S_CODE_START                    (S_ROM_ALIAS(S_IMAGE_PRIMARY_AREA_OFFSET))
#define S_CODE_SIZE                     (IMAGE_S_CODE_SIZE)
#define S_CODE_LIMIT                    (S_CODE_START + S_CODE_SIZE - 1)

#define S_DATA_START                    (S_RAM_ALIAS(S_DATA_OFFSET + RESERVED_RAM_SIZE))

/* 192 KB ram is for secure application and rest (7.5 MB - 192 KB) is available for non-secure apps*/
#define S_DATA_SIZE                     (192 * 1024) // 192 KB = 0x3000
#define S_DATA_LIMIT                    (S_DATA_START + S_DATA_SIZE - 1)

/* Size of vector table: 144 interrupt handlers(see g_pfnVectors definition) + 4 bytes MPS initial value ((144*4 + 4) = 580 --> 0x244) */
#define S_CODE_VECTOR_TABLE_SIZE        (0x300)

/* Non-secure regions */
#define NS_IMAGE_PRIMARY_AREA_OFFSET    (NS_IMAGE_PRIMARY_PARTITION_OFFSET + BL2_HEADER_SIZE)
#define NS_CODE_START                   (NS_ROM_ALIAS(NS_IMAGE_PRIMARY_AREA_OFFSET))
#define NS_CODE_SIZE                    (IMAGE_NS_CODE_SIZE)
#define NS_CODE_LIMIT                   (NS_CODE_START + NS_CODE_SIZE - 1)

#define NS_DATA_START                   (NS_RAM_ALIAS(S_DATA_OFFSET + S_DATA_SIZE + RESERVED_RAM_SIZE))
#define NS_DATA_SIZE                    (TOTAL_RAM_SIZE - S_DATA_SIZE - S_DATA_OFFSET)
#define NS_DATA_LIMIT                   (NS_DATA_START + NS_DATA_SIZE - 1)

/* FlexSPI. Each sub-region can be assigned individual security tier by programing corresponding registers
 * in secure AHB controller.
 */
/* Region 0: 8 MB (32 * 256 KB). */
#define FLASH_REGION0_SUBREGION_NUMBER  (32)  
#define FLASH_REGION0_SUBREGION_SIZE    (1024 * 256)            /* 256 kB */
#define FLASH_REGION0_SIZE              (FLASH_REGION0_SUBREGION_NUMBER * FLASH_REGION0_SUBREGION_SIZE)     /* 8 MB */
/* Region 1: 8 MB (4 * 2 MB) */
#define FLASH_REGION1_SUBREGION_NUMBER  (4)  
#define FLASH_REGION1_SUBREGION_SIZE    (1024 * 1024 * 2)            /* 2048 KB, 2MB */
#define FLASH_REGION1_SIZE              (FLASH_REGION1_SUBREGION_NUMBER * FLASH_REGION1_SUBREGION_SIZE)     /* 8 MB */
/* Region 2: 16 MB (4 * 4 MB) */
#define FLASH_REGION2_SUBREGION_NUMBER  (4)  
#define FLASH_REGION2_SUBREGION_SIZE    (1024 * 1024 * 4)       /* 4 MB */
#define FLASH_REGION2_SIZE              (FLASH_REGION2_SUBREGION_NUMBER * FLASH_REGION2_SUBREGION_SIZE)     /* 16 MB */
/* Region 3: 32 MB (4 * 8 MB) */
#define FLASH_REGION3_SUBREGION_NUMBER  (4)  
#define FLASH_REGION3_SUBREGION_SIZE    (1024 * 1024 * 8)       /* 4 MB */
#define FLASH_REGION3_SIZE              (FLASH_REGION3_SUBREGION_NUMBER * FLASH_REGION3_SUBREGION_SIZE)     /* 32 MB */
/* Region 4: 64 MB (4 * 16 MB) */
#define FLASH_REGION4_SUBREGION_NUMBER  (4)  
#define FLASH_REGION4_SUBREGION_SIZE    (1024 * 1024 * 16)      /* 8 MB */
#define FLASH_REGION4_SIZE              (FLASH_REGION4_SUBREGION_NUMBER * FLASH_REGION4_SUBREGION_SIZE)     /* 64 MB */


/* RAM. Each sub-region can be assigned individual security tier by programing corresponding registers in secure AHB
 * controller.
 */
/* Region 0: RAM0 to RAM3 , (128 * 1 KB) */
#define DATA_REGION0_SUBREGION_NUMBER   (128)
#define DATA_REGION0_SUBREGION_SIZE     (1024 * 1)      /* 1 KB*/
#define DATA_REGION0_SIZE               (DATA_REGION0_SUBREGION_NUMBER * DATA_REGION0_SUBREGION_SIZE)      /* 128 KB */

/* Region 1: RAM4 to RAM5 , (64 * 2 KB) */
#define DATA_REGION1_SUBREGION_NUMBER   (64)
#define DATA_REGION1_SUBREGION_SIZE     (1024 * 2)      /* 2 KB*/
#define DATA_REGION1_SIZE               (DATA_REGION1_SUBREGION_NUMBER * DATA_REGION1_SUBREGION_SIZE)      /* 128 KB */

/* Region 2: RAM6 to RAM7 , (64 * 4 KB) */
#define DATA_REGION2_SUBREGION_NUMBER   (64)
#define DATA_REGION2_SUBREGION_SIZE     (1024 * 4)      /* 4 KB*/
#define DATA_REGION2_SIZE               (DATA_REGION2_SUBREGION_NUMBER * DATA_REGION2_SUBREGION_SIZE)      /* 256 KB */

/* Region 3: RAM8 to RAM9, (64 * 8 KB) */
#define DATA_REGION3_SUBREGION_NUMBER   (64)
#define DATA_REGION3_SUBREGION_SIZE     (1024 * 8)      /* 8 KB*/
#define DATA_REGION3_SIZE               (DATA_REGION3_SUBREGION_NUMBER * DATA_REGION3_SUBREGION_SIZE)      /* 512 KB */

/* Region 4: RAM10 to RAM11 , (64 * 16 KB) */
#define DATA_REGION4_SUBREGION_NUMBER   (64)
#define DATA_REGION4_SUBREGION_SIZE     (1024 * 16)      /* 16 KB*/
#define DATA_REGION4_SIZE               (DATA_REGION4_SUBREGION_NUMBER * DATA_REGION4_SUBREGION_SIZE)      /* 1024 KB */

/* Region 5: RAM12 to RAM13 , (64 * 32 KB) */
#define DATA_REGION5_SUBREGION_NUMBER   (64)
#define DATA_REGION5_SUBREGION_SIZE     (1024 * 32)      /* 32 KB*/
#define DATA_REGION5_SIZE               (DATA_REGION5_SUBREGION_NUMBER * DATA_REGION5_SUBREGION_SIZE)      /* 2048 KB */

/* Region 6: RAM14 to RAM15, (64 * 16 KB) */
#define DATA_REGION6_SUBREGION_NUMBER   (64)
#define DATA_REGION6_SUBREGION_SIZE     (1024 * 16)      /* 16 KB*/
#define DATA_REGION6_SIZE               (DATA_REGION6_SUBREGION_NUMBER * DATA_REGION6_SUBREGION_SIZE)      /* 1024 KB */

/* Region 7: RAM16 to RAM17, (64 * 8 KB) */
#define DATA_REGION7_SUBREGION_NUMBER   (64)
#define DATA_REGION7_SUBREGION_SIZE     (1024 * 8)      /* 8 KB*/
#define DATA_REGION7_SIZE               (DATA_REGION7_SUBREGION_NUMBER * DATA_REGION7_SUBREGION_SIZE)      /* 512 KB */

/* Region 8: RAM18 to RAM21, (128 * 1 KB) */
#define DATA_REGION8_SUBREGION_NUMBER   (128)
#define DATA_REGION8_SUBREGION_SIZE     (1024 * 1)      /* 1 KB*/
#define DATA_REGION8_SIZE               (DATA_REGION8_SUBREGION_NUMBER * DATA_REGION8_SUBREGION_SIZE)      /* 128 KB */


/* Region 9: RAM22 to RAM23 , (64 * 2 KB) */
#define DATA_REGION9_SUBREGION_NUMBER   (64)
#define DATA_REGION9_SUBREGION_SIZE     (1024 * 2)      /* 2 KB*/
#define DATA_REGION9_SIZE               (DATA_REGION9_SUBREGION_NUMBER * DATA_REGION9_SUBREGION_SIZE)      /* 128 KB */

/* Region 10: RAM24 to RAM25 , (64 * 4 KB) */
#define DATA_REGION10_SUBREGION_NUMBER   (64)
#define DATA_REGION10_SUBREGION_SIZE     (1024 * 4)      /* 4 KB*/
#define DATA_REGION10_SIZE               (DATA_REGION10_SUBREGION_NUMBER * DATA_REGION10_SUBREGION_SIZE)      /* 256 KB */

/* Region 11: RAM26 to RAM27, (64 * 16 KB) */
#define DATA_REGION11_SUBREGION_NUMBER   (64)
#define DATA_REGION11_SUBREGION_SIZE     (1024 * 16)      /* 16 KB*/
#define DATA_REGION11_SIZE               (DATA_REGION11_SUBREGION_NUMBER * DATA_REGION11_SUBREGION_SIZE)      /* 1024 KB */

/* Region 12: RAM28 to RAM29, (64 * 8 KB) */
#define DATA_REGION12_SUBREGION_NUMBER   (64)
#define DATA_REGION12_SUBREGION_SIZE     (1024 * 8)      /* 8 KB*/
#define DATA_REGION12_SIZE               (DATA_REGION12_SUBREGION_NUMBER * DATA_REGION12_SUBREGION_SIZE)      /* 512 KB */


/* NS partition information is used for MPC and SAU configuration */
#define NS_PARTITION_START  (NS_ROM_ALIAS(NS_IMAGE_PRIMARY_PARTITION_OFFSET))
#define NS_PARTITION_SIZE   (FLASH_NS_PARTITION_SIZE)

/* Secondary partition for new images in case of firmware upgrade */
#define SECONDARY_PARTITION_START   (NS_ROM_ALIAS(S_IMAGE_SECONDARY_PARTITION_OFFSET))
#define SECONDARY_PARTITION_SIZE    (FLASH_S_PARTITION_SIZE + FLASH_NS_PARTITION_SIZE)

/* Code SRAM area */
#define S_RAM_CODE_SIZE     (0x8000) /* SRAM X region (32KB), for venners or code(relocated) to be run from secure RAM, can be specified as 0x00 */
#define S_RAM_CODE_START    (0x30080000)
#define NS_RAM_CODE_START   (0x20080000)

/* Shared data area between bootloader and runtime firmware.
 * Shared data area is allocated at the beginning of the RAM, it is overlapping with TF-M Secure code's MSP stack
 */
#define BOOT_TFM_SHARED_DATA_BASE   (S_RAM_ALIAS_BASE + RESERVED_RAM_SIZE)
#define BOOT_TFM_SHARED_DATA_SIZE   (0x400)
#define BOOT_TFM_SHARED_DATA_LIMIT  (BOOT_TFM_SHARED_DATA_BASE + BOOT_TFM_SHARED_DATA_SIZE - 1)

/* FCB address to be placed. Used in Linker*/
#define M_BOOT_FLASH_CONF_START     (0x18000000)
#define M_BOOT_FLASH_CONF_SIZE      (0x00001000) /* 4 KB */

/* RT700 expects a non-cache region in linker, it needs to be added in common tfm linker used by nxp platform */
#define TFM_NCACHE_REGION_ENABLE

#endif /* __REGION_DEFS_H__ */
