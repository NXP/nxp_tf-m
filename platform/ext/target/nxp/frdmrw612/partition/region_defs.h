/*
 * Copyright (c) 2017-2023 Arm Limited. All rights reserved.
 * Copyright 2019-2023 NXP.
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
    #define S_HEAP_SIZE             (0x0000200)
#endif

#define S_MSP_STACK_SIZE        (0x0000800)
#define S_PSP_STACK_SIZE        (0x0000800)

#define NS_HEAP_SIZE            (0x0004000)
#define NS_STACK_SIZE           (0x0002000)

/* This size of buffer is big enough to store an attestation token produced by initial attestation service. */
#define PSA_INITIAL_ATTEST_TOKEN_MAX_SIZE   (0x250)

/* [RW612] The boot image should be program at offset 0x08001000. 
XIP Image layout (FlexSPI):
Offset      Width (Bytes) Field Description
0x0000_0000 256           KeyBlob for OTFAD Optional, programmed with all 0x00s if OTFAD is not enabled?
0x0000_0400 512           Flash Config Block The OSPI FLASH configuration block. This block is required if
                          the FLEXSPI_AUTO_PROBE_EN is not blown on the OTP.
0x0000_0600 4             Boot image version. When using dual image ping-pong boot, this field is used to store
                          the boot image version. More details please refer to FlexSPI boot.
0x0000_0800 2048          KeyStore Fixed KeyStore field. This field is required if the KeyStore feature is enabled.
0x0000_1000 Image size    Bootable Image. The boot image, starts with valid image header.
*/

// RAM_TOTAL_SIZE                                                 = 0x00130000
// TOTAL_RAM_SIZE: RAM_TOTAL_SIZE                                 = 0x00130000
// TOTAL_CODE_SRAM_SIZE                                           = 0x00010000
// S_DATA_OFFSET:TOTAL_CODE_SRAM_SIZE                             = 0x00010000
// S_DATA_SIZE: ((TOTAL_RAM_SIZE - S_DATA_OFFSET ) / 6)           = 0x00030000
// NS_DATA_START: S_DATA_OFFSET + S_DATA_SIZE                     = 0x00040000
// NS_DATA_SIZE: TOTAL_RAM_SIZE - S_DATA_SIZE - S_DATA_OFFSET     = 0x000F0000
// NS_DATA_LIMIT: NS_DATA_START + NS_DATA_SIZE - 1                = 0x0012FFFF

#define S_IMAGE_PRIMARY_PARTITION_OFFSET (0x1000)

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

#define IMAGE_S_CODE_SIZE   (FLASH_S_PARTITION_SIZE - BL2_HEADER_SIZE - BL2_TRAILER_SIZE)
#define IMAGE_NS_CODE_SIZE  (FLASH_NS_PARTITION_SIZE - BL2_HEADER_SIZE - BL2_TRAILER_SIZE)

#define CMSE_VENEER_REGION_SIZE     (0x340)

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

#define S_DATA_START                    (S_RAM_ALIAS(S_DATA_OFFSET))
/* Note, instead of having half SRAM split for secure and non-secure, lets assign a fixed size for secure region 
and assign bit more to non secure region. 1/6th size is reserved for secure instead of 1/2.*/
#define S_DATA_SIZE                     ((TOTAL_RAM_SIZE - S_DATA_OFFSET ) / 6)
#define S_DATA_LIMIT                    (S_DATA_START + S_DATA_SIZE - 1)

/* Size of vector table: 144 interrupt handlers(see g_pfnVectors definition) + 4 bytes MPS initial value ((144*4 + 4) = 580 --> 0x244) */
#define S_CODE_VECTOR_TABLE_SIZE        (0x244)

/* Non-secure regions */
#define NS_IMAGE_PRIMARY_AREA_OFFSET    (NS_IMAGE_PRIMARY_PARTITION_OFFSET + BL2_HEADER_SIZE)
#define NS_CODE_START                   (NS_ROM_ALIAS(NS_IMAGE_PRIMARY_AREA_OFFSET))
#define NS_CODE_SIZE                    (IMAGE_NS_CODE_SIZE)
#define NS_CODE_LIMIT                   (NS_CODE_START + NS_CODE_SIZE - 1)

#define NS_DATA_START                   (NS_RAM_ALIAS(S_DATA_OFFSET + S_DATA_SIZE))
#define NS_DATA_SIZE                    (TOTAL_RAM_SIZE - S_DATA_SIZE - S_DATA_OFFSET)
#define NS_DATA_LIMIT                   (NS_DATA_START + NS_DATA_SIZE - 1)

/* FlexSPI. Each sub-region can be assigned individual security tier by programing corresponding registers
 * in secure AHB controller.
 */
/* Region 0: 4 MB (32 * 128 KB). */
#define FLASH_REGION0_SUBREGION_NUMBER  (32)  
#define FLASH_REGION0_SUBREGION_SIZE    (1024 * 128)            /* 128 kB */
#define FLASH_REGION0_SIZE              (FLASH_REGION0_SUBREGION_NUMBER * FLASH_REGION0_SUBREGION_SIZE)     /* 4 MB */
/* Region 1: 4 MB (8 * 512 KB) */
#define FLASH_REGION1_SUBREGION_NUMBER  (8)  
#define FLASH_REGION1_SUBREGION_SIZE    (1024 * 512)            /* 512 kB */
#define FLASH_REGION1_SIZE              (FLASH_REGION1_SUBREGION_NUMBER * FLASH_REGION1_SUBREGION_SIZE)     /* 4 MB */
/* Region 2: 8 MB (4 * 2 MB) */
#define FLASH_REGION2_SUBREGION_NUMBER  (4)  
#define FLASH_REGION2_SUBREGION_SIZE    (1024 * 1024 * 2)       /* 2 MB */
#define FLASH_REGION2_SIZE              (FLASH_REGION2_SUBREGION_NUMBER * FLASH_REGION2_SUBREGION_SIZE)     /* 8 MB */
/* Region 3: 16 MB (4 * 4 MB) */
#define FLASH_REGION3_SUBREGION_NUMBER  (4)  
#define FLASH_REGION3_SUBREGION_SIZE    (1024 * 1024 * 4)       /* 4 MB */
#define FLASH_REGION3_SIZE              (FLASH_REGION3_SUBREGION_NUMBER * FLASH_REGION3_SUBREGION_SIZE)     /* 16 MB */
/* Region 4: 32 MB (4 * 8 MB) */
#define FLASH_REGION4_SUBREGION_NUMBER  (4)  
#define FLASH_REGION4_SUBREGION_SIZE    (1024 * 1024 * 8)      /* 8 MB */
#define FLASH_REGION4_SIZE              (FLASH_REGION4_SUBREGION_NUMBER * FLASH_REGION4_SUBREGION_SIZE)     /* 32 MB */


/* RAM. Each sub-region can be assigned individual security tier by programing corresponding registers in secure AHB
 * controller.
 */
/* Region 0: RAM0 to RAM18 ~1 MB, (608 * 1 KB) */
#define DATA_REGION0_SUBREGION_NUMBER   (608)
#define DATA_REGION0_SUBREGION_SIZE     (1024 * 2)      /* 2 KB*/
#define DATA_REGION0_SIZE               (DATA_REGION0_SUBREGION_NUMBER * DATA_REGION0_SUBREGION_SIZE)      /* ~1 MB KB */

/* NS partition information is used for MPC and SAU configuration */
#define NS_PARTITION_START  (NS_ROM_ALIAS(NS_IMAGE_PRIMARY_PARTITION_OFFSET))
#define NS_PARTITION_SIZE   (FLASH_NS_PARTITION_SIZE)

/* Secondary partition for new images in case of firmware upgrade */
#define SECONDARY_PARTITION_START   (NS_ROM_ALIAS(S_IMAGE_SECONDARY_PARTITION_OFFSET))
#define SECONDARY_PARTITION_SIZE    (FLASH_S_PARTITION_SIZE + FLASH_NS_PARTITION_SIZE)

/* Code SRAM area */
#define S_RAM_CODE_SIZE     (0x10000) /* SRAM X region */
#define S_RAM_CODE_START    (0x10000000)
#define NS_RAM_CODE_START   (0x00000000)

#ifdef BL2
/* Bootloader regions */
#define BL2_CODE_START    (S_ROM_ALIAS(FLASH_AREA_BL2_OFFSET))
#define BL2_CODE_SIZE     (FLASH_AREA_BL2_SIZE)
#define BL2_CODE_LIMIT    (BL2_CODE_START + BL2_CODE_SIZE - 1)

#define BL2_DATA_START    (S_RAM_ALIAS(0x0))
#define BL2_DATA_SIZE     (TOTAL_RAM_SIZE)
#define BL2_DATA_LIMIT    (BL2_DATA_START + BL2_DATA_SIZE - 1)
#endif /* BL2 */

/* Shared data area between bootloader and runtime firmware.
 * Shared data area is allocated at the beginning of the RAM, it is overlapping with TF-M Secure code's MSP stack
 */
#define BOOT_TFM_SHARED_DATA_BASE   (S_RAM_ALIAS_BASE)
#define BOOT_TFM_SHARED_DATA_SIZE   (0x400)
#define BOOT_TFM_SHARED_DATA_LIMIT  (BOOT_TFM_SHARED_DATA_BASE + BOOT_TFM_SHARED_DATA_SIZE - 1)

/* RW612 ROM supports boot from different type NOR flash such as Quad Flash, Octal Flash, Hyper Flash which connects
 * to FlexSPI pins used by ROM.
 * For FlexSPI NOR boot, the FCB should be programmed at offset 0x08000400, and the boot image should be program
 * at offset 0x08001000.
 * Flash configuration block (FCB) is used to configure the FlexSPI interface during the boot process.
 */
#if defined(__ARMCC_VERSION) || defined(__ICCARM__)
#define M_BOOT_FLASH_CONF_START     (0x18000000)
#define M_BOOT_FLASH_CONF_SIZE      (0x00001000) /* 4 KB */
#else
#define M_BOOT_FLASH_CONF_START     (0x18000400)
#define M_BOOT_FLASH_CONF_SIZE      (0x00000C00) /* 3 KB */
#endif

#ifdef TFM_WIFI_FLASH_REGION
#define WIFI_FLASH_REGION_START		(TFM_WIFI_FIRMWARE_ADDR)
#define WIFI_FLASH_REGION_SIZE		(TFM_WIFI_FIRMWARE_SIZE)
#endif /* TFM_WIFI_FLASH_REGION */

#ifdef TFM_EL2GO_DATA_IMPORT_REGION 
#define EL2GO_DATA_IMPORT_REGION_START		(TFM_EL2GO_NV_DATA_IMPORT_ADDR)
#define EL2GO_DATA_IMPORT_REGION_SIZE		(TFM_EL2GO_NV_DATA_IMPORT_SIZE)
#endif /* TFM_EL2GO_DATA_IMPORT_REGION  */

#endif /* __REGION_DEFS_H__ */
