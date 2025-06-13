/*
 * Copyright (c) 2018-2021 Arm Limited. All rights reserved.
 * Copyright 2019-2025 NXP.
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

#ifndef __FLASH_LAYOUT_H__
#define __FLASH_LAYOUT_H__

/* Flash layout with BL2 (multiple image boot):
 *
 * 0x0000_0000 (0x400) 1 KB
 * 0x0000_0400 Flash Config Block (0xC00) 3 KB
 * 0x0000_1000 BL2 - MCUBoot (0x1F000) 124 KB
 * 0x0002_0000 Secure image     primary slot   (0xA0000)  640 KB ---> must be alinged to 128KB 
 * 0x000C_0000 Non-secure image primary slot   (0x300000)   3 MB
 * 0x003C_0000 Secure image     secondary slot (0xA0000)  640 KB
 * 0x0046_0000 Non-secure image secondary slot (0x300000)   3 MB 
 * 0x0076_0000 Protected Storage area          (0x20000)  128 KB 
 * 0x0078_0000 Internal Trusted Storage area   (0x20000)  128 KB 
 * 0x007A_0000 OTP area                         (0x4000)   16 KB
 * 0x0840_0000 WiFi firmware
 * 0x084A_0000 EL2GO blobs
 * 0xXXXX_XXXX Unused
 */
 
 
/* Flash layout without BL2:
 *
 * 0x0000_0000
 * 0x0000_0400 Flash Config Block
 * 0x0000_1000 Secure code  (0x9F000) 636 KB 
 * 0x000A_0000 Non-Secure code   (0x300000) 3 MB
 * 0x003A_0000 Protected Storage area (0x20000)  128 KB 
 * 0x003C_0000 Internal Trusted Storage area (0x20000)  128 KB 
 * 0x003E_4000 OTP area (0x4000)  16 KB
 * 0x0840_0000 WiFi firmware
 * 0x084A_0000 EL2GO blobs
 * 0xXXXX_XXXX Unused
 */


/* This header file is included from linker scatter file as well, where only a limited C constructs are allowed.
 * Therefore it is not possible to include here the platform_base_address.h to access flash related defines. To resolve
 * this some of the values are redefined here with different names, these are marked with comment.
 */

/* Image header size*/
#define FLASH_IMAGE_HEADER_SIZE         (0x1000) /* 4 KB*/

/* Size of a Secure and of a Non-secure image */
#ifdef BL2
#define FLASH_S_PARTITION_SIZE          0xA0000    /* S partition: 640 kB: 0xA0000 */
#define FLASH_NS_PARTITION_SIZE         (0x300000) /* NS partition: 3 MB : 0x300000*/

#if (FLASH_S_PARTITION_SIZE > FLASH_NS_PARTITION_SIZE)
#define FLASH_MAX_PARTITION_SIZE FLASH_S_PARTITION_SIZE
#else
#define FLASH_MAX_PARTITION_SIZE FLASH_NS_PARTITION_SIZE
#endif

#else
#define FLASH_S_PARTITION_SIZE          (0xA0000 - FLASH_IMAGE_HEADER_SIZE)  /* S partition: 640 kB - 4KB Offset */
#define FLASH_NS_PARTITION_SIZE         (0x300000)           /* NS partition: 3 MB*/
#endif

/* Sector size (erase) */
#define FLASH_AREA_IMAGE_SECTOR_SIZE    (1024 * 4)          /* Sector (4K-byte)*/
#define FLASH_AREA_PROGRAM_SIZE         (256)

/* Flash layout info for BL2 bootloader */
#define FLASH_BASE_ADDRESS              NS_ROM_ALIAS_BASE 

/* FLASH size */
#define FLASH_TOTAL_SIZE                (1024 * 1024 * 64)  /* 64 MBytes of physical Flash memory */

/* RAM size */
#define RAM_TOTAL_SIZE                  (0x130000)          /* 1.2 MBytes of physical SRAM memory */

#ifdef BL2
/* Offset and size definitions of the flash partitions that are handled by the
 * bootloader. The image swapping is done between IMAGE_PRIMARY and
 * IMAGE_SECONDARY, SCRATCH is used as a temporary storage during image
 * swapping.
 */
#define FLASH_AREA_BL2_OFFSET      FLASH_IMAGE_HEADER_SIZE
#define FLASH_AREA_BL2_SIZE        0x1F000 /* 124 KB  */


#if !defined(MCUBOOT_IMAGE_NUMBER) || (MCUBOOT_IMAGE_NUMBER == 1)
/* Secure + Non-secure image primary slot */
#define FLASH_AREA_0_ID            (1)
#define FLASH_AREA_0_OFFSET        (FLASH_AREA_BL2_OFFSET + FLASH_AREA_BL2_SIZE)
#define FLASH_AREA_0_SIZE          (FLASH_S_PARTITION_SIZE + \
                                    FLASH_NS_PARTITION_SIZE)
/* Secure + Non-secure secondary slot */
#define FLASH_AREA_2_ID            (FLASH_AREA_0_ID + 1)
#define FLASH_AREA_2_OFFSET        (FLASH_AREA_0_OFFSET + FLASH_AREA_0_SIZE)
#define FLASH_AREA_2_SIZE          (FLASH_S_PARTITION_SIZE + \
                                    FLASH_NS_PARTITION_SIZE)
/* Not used (scratch area), the 'Swap' firmware upgrade operation is not
 * supported on RW61x.
 */
#define FLASH_AREA_SCRATCH_ID      (FLASH_AREA_2_ID + 1)
#define FLASH_AREA_SCRATCH_OFFSET  (FLASH_AREA_2_OFFSET + FLASH_AREA_2_SIZE)
#define FLASH_AREA_SCRATCH_SIZE    (0)
/* Maximum number of image sectors supported by the bootloader. */
#define MCUBOOT_MAX_IMG_SECTORS    ((FLASH_S_PARTITION_SIZE + \
                                     FLASH_NS_PARTITION_SIZE) / \
                                    FLASH_AREA_IMAGE_SECTOR_SIZE)
#elif (MCUBOOT_IMAGE_NUMBER == 2)
/* Secure image primary slot */
#define FLASH_AREA_0_ID            (1)
#define FLASH_AREA_0_OFFSET        (FLASH_AREA_BL2_OFFSET + FLASH_AREA_BL2_SIZE)
#define FLASH_AREA_0_SIZE          (FLASH_S_PARTITION_SIZE)
/* Non-secure image primary slot */
#define FLASH_AREA_1_ID            (FLASH_AREA_0_ID + 1)
#define FLASH_AREA_1_OFFSET        (FLASH_AREA_0_OFFSET + FLASH_AREA_0_SIZE)
#define FLASH_AREA_1_SIZE          (FLASH_NS_PARTITION_SIZE)
/* Secure image secondary slot */
#define FLASH_AREA_2_ID            (FLASH_AREA_1_ID + 1)
#define FLASH_AREA_2_OFFSET        (FLASH_AREA_1_OFFSET + FLASH_AREA_1_SIZE)
#define FLASH_AREA_2_SIZE          (FLASH_S_PARTITION_SIZE)
/* Non-secure image secondary slot */
#define FLASH_AREA_3_ID            (FLASH_AREA_2_ID + 1)
#define FLASH_AREA_3_OFFSET        (FLASH_AREA_2_OFFSET + FLASH_AREA_2_SIZE)
#define FLASH_AREA_3_SIZE          (FLASH_NS_PARTITION_SIZE)
/* Not used (scratch area), the 'Swap' firmware upgrade operation is not
 * supported on RW61x.
 */
#define FLASH_AREA_SCRATCH_ID      (FLASH_AREA_3_ID + 1)
#define FLASH_AREA_SCRATCH_OFFSET  (FLASH_AREA_3_OFFSET + FLASH_AREA_3_SIZE)
#define FLASH_AREA_SCRATCH_SIZE    (0)
/* Maximum number of image sectors supported by the bootloader. */
#define MCUBOOT_MAX_IMG_SECTORS    (FLASH_MAX_PARTITION_SIZE / \
                                    FLASH_AREA_IMAGE_SECTOR_SIZE)
#else /* MCUBOOT_IMAGE_NUMBER > 2 */
#error "Only MCUBOOT_IMAGE_NUMBER 1 and 2 are supported!"
#endif /* MCUBOOT_IMAGE_NUMBER */

/* Not used, the 'Swap' firmware upgrade operation is not supported on RW61x.
 * The maximum number of status entries supported by the bootloader.
 */
#define MCUBOOT_STATUS_MAX_ENTRIES (0)

#else /* NO BL2 */

#ifdef SB_FILE /* Use signed Secure Binary (SB) image */
#define FLASH_SB_TAIL   (0x1000)    /* 4 KB */
#else
#define FLASH_SB_TAIL   (0x0)       /* 0 KB */
#endif /* SB_FILE */


/* Secure + Non-secure image primary slot */
#define FLASH_AREA_0_ID            (1)
#define FLASH_AREA_0_OFFSET        (FLASH_IMAGE_HEADER_SIZE)
#define FLASH_AREA_0_SIZE          (FLASH_S_PARTITION_SIZE + FLASH_NS_PARTITION_SIZE + FLASH_SB_TAIL)

/* Not used */
#define FLASH_AREA_SCRATCH_ID      (FLASH_AREA_0_ID + 1)
#define FLASH_AREA_SCRATCH_OFFSET  (FLASH_AREA_0_OFFSET + FLASH_AREA_0_SIZE)
#define FLASH_AREA_SCRATCH_SIZE    (0)
#endif /* BL2 */


/* Protected Storage (PS) Service definitions */
#define FLASH_PS_AREA_OFFSET                (FLASH_AREA_SCRATCH_OFFSET + FLASH_AREA_SCRATCH_SIZE)
#define FLASH_PS_AREA_SIZE                  (0x20000) /* 128 KB */

/* Internal Trusted Storage (ITS) Service definitions */
#define FLASH_ITS_AREA_OFFSET               (FLASH_PS_AREA_OFFSET + FLASH_PS_AREA_SIZE)
#define FLASH_ITS_AREA_SIZE                 (0x20000) /* 128 KB */

/* OTP_definitions */
#define FLASH_OTP_NV_COUNTERS_AREA_OFFSET   (FLASH_ITS_AREA_OFFSET + FLASH_ITS_AREA_SIZE)
#define FLASH_OTP_NV_COUNTERS_AREA_SIZE     (FLASH_AREA_IMAGE_SECTOR_SIZE * 4)
#define FLASH_OTP_NV_COUNTERS_SECTOR_SIZE   (FLASH_AREA_IMAGE_SECTOR_SIZE)

/* Offset and size definition in flash area used by assemble.py */
#define SECURE_IMAGE_OFFSET                 (0x0)
#define SECURE_IMAGE_MAX_SIZE               (FLASH_S_PARTITION_SIZE)

#define NON_SECURE_IMAGE_OFFSET             (SECURE_IMAGE_OFFSET + SECURE_IMAGE_MAX_SIZE)
#define NON_SECURE_IMAGE_MAX_SIZE           (FLASH_NS_PARTITION_SIZE)


/* Flash device name used by BL2.
 * Name is defined in flash driver file: Driver_Flash.c
 */
#define FLASH_DEV_NAME                  (Driver_FLASH0)

/* Smallest flash programmable unit in bytes */
#define TFM_HAL_FLASH_PROGRAM_UNIT      (FLASH_AREA_PROGRAM_SIZE)

/* Protected Storage (PS) Service definitions
 * Note: Further documentation of these definitions can be found in the TF-M PS Integration Guide.
 */
#define TFM_HAL_PS_FLASH_DRIVER         (Driver_FLASH0)


/* In this target the CMSIS driver requires only the offset from the base address instead of the full memory
 * address.
 */
/* Base address of dedicated flash area for PS */
#define TFM_HAL_PS_FLASH_AREA_ADDR      (FLASH_PS_AREA_OFFSET)

/* Size of dedicated flash area for PS */
#define TFM_HAL_PS_FLASH_AREA_SIZE      (FLASH_PS_AREA_SIZE)
#define PS_RAM_FS_SIZE                  (TFM_HAL_PS_FLASH_AREA_SIZE)

/* Number of physical erase sectors per logical FS block */
#define TFM_HAL_PS_SECTORS_PER_BLOCK    (1)

/* Smallest flash programmable unit in bytes */
#define TFM_HAL_PS_PROGRAM_UNIT         (FLASH_AREA_PROGRAM_SIZE)
#define PS_FLASH_NAND_BUF_SIZE          (FLASH_AREA_IMAGE_SECTOR_SIZE * TFM_HAL_PS_SECTORS_PER_BLOCK)


/* Internal Trusted Storage (ITS) Service definitions
 * Note: Further documentation of these definitions can be found in the TF-M ITS Integration Guide. The ITS should be
 * in the internal flash, but is allocated in the external flash just for development platforms that don't have
 * internal flash available.
 */
#define TFM_HAL_ITS_FLASH_DRIVER        (Driver_FLASH0_IPED)


/* In this target the CMSIS driver requires only the offset from the base address instead of the full memory
 * address.
 */
/* Base address of dedicated flash area for ITS */
#define TFM_HAL_ITS_FLASH_AREA_ADDR     (FLASH_ITS_AREA_OFFSET)

/* Size of dedicated flash area for ITS */
#define FLASH_AREA_ITS_SECTOR_SIZE      ((FLASH_AREA_IMAGE_SECTOR_SIZE - FLASH_AREA_PROGRAM_SIZE) / 5 * 4)
#define TFM_HAL_ITS_FLASH_AREA_SIZE     ((FLASH_ITS_AREA_SIZE / FLASH_AREA_IMAGE_SECTOR_SIZE) * FLASH_AREA_ITS_SECTOR_SIZE)
#define ITS_RAM_FS_SIZE                 (TFM_HAL_ITS_FLASH_AREA_SIZE)

/* Number of physical erase sectors per logical FS block */
#define TFM_HAL_ITS_SECTORS_PER_BLOCK   (1)

/* Smallest flash programmable unit in bytes */
#define TFM_HAL_ITS_PROGRAM_UNIT        (FLASH_AREA_ITS_SECTOR_SIZE)
#define ITS_FLASH_NAND_BUF_SIZE         (FLASH_AREA_ITS_SECTOR_SIZE * TFM_HAL_ITS_SECTORS_PER_BLOCK)

/* OTP / NV counter definitions */
#define OTP_NV_COUNTERS_FLASH_DEV               (Driver_FLASH0)
#define TFM_OTP_NV_COUNTERS_AREA_SIZE           (FLASH_OTP_NV_COUNTERS_AREA_SIZE / 2)
#define TFM_OTP_NV_COUNTERS_AREA_ADDR           (FLASH_OTP_NV_COUNTERS_AREA_OFFSET)
#define TFM_OTP_NV_COUNTERS_SECTOR_SIZE         (FLASH_OTP_NV_COUNTERS_SECTOR_SIZE)
#define TFM_OTP_NV_COUNTERS_BACKUP_AREA_ADDR    (TFM_OTP_NV_COUNTERS_AREA_ADDR + TFM_OTP_NV_COUNTERS_AREA_SIZE)
#define OTP_NV_COUNTERS_WRITE_BLOCK_SIZE        (FLASH_AREA_PROGRAM_SIZE)

#define TFM_WIFI_FIRMWARE_ADDR  (0x08400000)
#define TFM_WIFI_FIRMWARE_SIZE  (0x000A0000)

#define TFM_EL2GO_NV_DATA_IMPORT_ADDR  (0x084A0000)
#define TFM_EL2GO_NV_DATA_IMPORT_SIZE  (0x00060000)

/* Use Flash memory to store Code data */
/* 0x08000000 is the FlexSPI memory-mapped AHB address */
#define S_ROM_ALIAS_BASE    (0x18000000)
#define NS_ROM_ALIAS_BASE   (0x08000000)

/* Use SRAM memory to store RW data */
#define S_RAM_ALIAS_BASE    (0x30000000)    /* Shared RAM via the CM33 data bus (secure access). */
#define NS_RAM_ALIAS_BASE   (0x20000000)    /* Shared RAM via the CM33 data bus (non-secure access). */

#define TOTAL_ROM_SIZE      (FLASH_TOTAL_SIZE)
#define TOTAL_RAM_SIZE      (RAM_TOTAL_SIZE)

#endif /* __FLASH_LAYOUT_H__ */
