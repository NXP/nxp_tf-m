/*
 * Copyright (c) 2017-2019 Arm Limited. All rights reserved.
 * Copyright 2019-2020, 2022-2024 NXP
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

/**
 * \file platform_base_address.h
 * \brief This file defines all the peripheral base addresses for platform.
 */

#ifndef __PLATFORM_BASE_ADDRESS_H__
#define __PLATFORM_BASE_ADDRESS_H__

#include "platform_regs.h"           /* Platform registers */
#include "RW612.h"

/* Define USART base */
#define USART_BASE          (USART3)

/* External Flash memory */
#define FLASH0_BASE_S           (S_ROM_ALIAS_BASE)  
#define FLASH0_BASE_NS          (NS_ROM_ALIAS_BASE) 

#define FLASH0_SIZE             (FLASH_TOTAL_SIZE)              /* 64 MB */
#define FLASH0_SECTOR_SIZE      (FLASH_AREA_IMAGE_SECTOR_SIZE)  /* Erase command is executed on sector (4K-byte), or block (32K-byte), or block (64K-byte), or whole chip */
#define FLASH0_PAGE_SIZE        (FLASH_AREA_PROGRAM_SIZE)       /* Page (256 bytes) basis, or word basis. */
#define FLASH0_PROGRAM_UNIT     (FLASH0_PAGE_SIZE)              /* Minimum write size */


/* Encrypted part of Flash */
#define FLASH0_IPED_OFFSET           (FLASH_ITS_AREA_OFFSET)
#define FLASH0_IPED_SIZE             (FLASH_ITS_AREA_SIZE)

#define FLASH0_IPED_SECTOR_SIZE_PHY  (FLASH_AREA_IMAGE_SECTOR_SIZE)
#define FLASH0_IPED_SECTOR_SIZE_LOG  ((FLASH_AREA_IMAGE_SECTOR_SIZE - FLASH_AREA_PROGRAM_SIZE) * 4 / 5)
#define FLASH0_IPED_SECTOR_COUNT     (FLASH_ITS_AREA_SIZE / FLASH0_IPED_SECTOR_SIZE_LOG)

#define FLASH0_IPED_PAGE_SIZE        (FLASH_AREA_PROGRAM_SIZE)   
#define FLASH0_IPED_PROGRAM_UNIT     (FLASH0_IPED_SECTOR_SIZE_LOG)  

#define FLASH0_IPED_REGION_NR        ((iped_region_t) 14)  

#endif  /* __PLATFORM_BASE_ADDRESS_H__ */
