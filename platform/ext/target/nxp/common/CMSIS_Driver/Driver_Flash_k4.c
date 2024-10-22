/*
 * Copyright (c) 2013-2018 ARM Limited. All rights reserved.
 * Copyright (c) 2019-2022 NXP. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.

 */

#include "target_cfg.h"
#include "Driver_Flash.h"
#include "platform_base_address.h"
#include "flash_layout.h"
#ifdef K4_FLASH_ROMAPI
#include "fsl_flash_api.h"
#else
#include "fsl_k4_flash.h"
#endif
#include "tfm_spm_log.h"
#include "critical_section.h"


//#undef TARGET_DEBUG_LOG
//#define TARGET_DEBUG_LOG 1 //DM

#ifndef ARG_UNUSED
#define ARG_UNUSED(arg)  ((void)arg)
#endif

/* Driver version */
#define ARM_FLASH_DRV_VERSION    ARM_DRIVER_VERSION_MAJOR_MINOR(1, 0)

/**
 * \brief Arm Flash device structure.
 */
struct arm_flash_dev_t {
    ARM_FLASH_INFO *data;         /*!< FLASH data */
    flash_config_t flash_config;  /*!< FLASH config */
};

/* Flash Status */
static ARM_FLASH_STATUS FlashStatus = {0, 0, 0};

/* Driver Version */
static const ARM_DRIVER_VERSION DriverVersion = {
    ARM_FLASH_API_VERSION,
    ARM_FLASH_DRV_VERSION
};

/**
 * \brief Flash driver capability macro definitions \ref ARM_FLASH_CAPABILITIES
 */
/* Flash Ready event generation capability values */
#define EVENT_READY_NOT_AVAILABLE   (0u)
#define EVENT_READY_AVAILABLE       (1u)

/* Data access size values */
enum {
    DATA_WIDTH_8BIT   = 0u,
    DATA_WIDTH_16BIT,
    DATA_WIDTH_32BIT,
    DATA_WIDTH_ENUM_SIZE
};

static const uint32_t data_width_byte[DATA_WIDTH_ENUM_SIZE] = {
    sizeof(uint8_t),
    sizeof(uint16_t),
    sizeof(uint32_t),
};

/* Chip erase capability values */
#define CHIP_ERASE_NOT_SUPPORTED    (0u)
#define CHIP_ERASE_SUPPORTED        (1u)

/* Driver Capabilities */
static const ARM_FLASH_CAPABILITIES DriverCapabilities = {
    EVENT_READY_NOT_AVAILABLE,
    DATA_WIDTH_32BIT,
    CHIP_ERASE_NOT_SUPPORTED
};

static ARM_FLASH_INFO ARM_FLASH0_DEV_DATA = {
    .sector_info  = NULL,                  /* Uniform sector layout */
    .sector_count = FLASH0_SIZE / FLASH0_SECTOR_SIZE,
    .sector_size  = FLASH0_SECTOR_SIZE,
    .page_size    = FLASH0_PAGE_SIZE,
    .program_unit = FLASH0_PROGRAM_UNIT,
    .erased_value = 0xFF};

static struct arm_flash_dev_t ARM_FLASH0_DEV = {
    .data        = &(ARM_FLASH0_DEV_DATA)};

static struct arm_flash_dev_t *FLASH0_DEV = &ARM_FLASH0_DEV;

/* Prototypes */
static bool is_range_valid(struct arm_flash_dev_t *flash_dev,
                           uint32_t offset);
static bool is_write_aligned(struct arm_flash_dev_t *flash_dev,
                             uint32_t param);

/* Functions */

/**
 * \brief      Check if the Flash memory boundaries are not violated.
 * \param[in]  flash_dev  Flash device structure \ref arm_flash_dev_t
 * \param[in]  offset     Highest Flash memory address which would be accessed.
 * \return     Returns true if Flash memory boundaries are not violated, false
 *             otherwise.
 */
static bool is_range_valid(struct arm_flash_dev_t *flash_dev,
                           uint32_t offset)
{
    uint32_t flash_limit = 0;

    /* Calculating the highest address of the Flash memory address range */
    flash_limit = FLASH0_BASE_S + FLASH0_SIZE - 1;

    return (offset < FLASH0_BASE_S || offset > flash_limit) ? (false) : (true) ;
}

/**
 * \brief        Check if the parameter is aligned to program_unit.
 * \param[in]    flash_dev  Flash device structure \ref arm_flash_dev_t
 * \param[in]    param      Any number that can be checked against the
 *                          program_unit, e.g. Flash memory address or
 *                          data length in bytes.
 * \return       Returns true if param is aligned to program_unit, false
 *               otherwise.
 */
static bool is_write_aligned(struct arm_flash_dev_t *flash_dev,
                             uint32_t param)
{
    return ((param % flash_dev->data->program_unit) != 0) ? (false) : (true);
}

/**
  * \brief        Check if the parameter is aligned to page_unit.
  * \param[in]    flash_dev  Flash device structure \ref arm_flash_dev_t
  * \param[in]    param      Any number that can be checked against the
  *                          sector_size, e.g. Flash memory address or
  *                          data length in bytes.
  * \return       Returns true if param is aligned to sector_size, false
  *               otherwise.
  */
static bool is_erase_aligned(struct arm_flash_dev_t *flash_dev,
                             uint32_t param)
{
  return ((param % (flash_dev->data->sector_size)) != 0) ? (false) : (true);
}

static ARM_DRIVER_VERSION ARM_Flash_GetVersion(void)
{
    return DriverVersion;
}

static ARM_FLASH_CAPABILITIES ARM_Flash_GetCapabilities(void)
{
    return DriverCapabilities;
}

static bool flash_init_is_done = false;
static int32_t ARM_Flash_Initialize(ARM_Flash_SignalEvent_t cb_event)
{
    ARG_UNUSED(cb_event);
    status_t                    status;

    if (flash_init_is_done == false)
    {
        /* Clean up Flash, Cache driver Structure*/
        memset(&FLASH0_DEV->flash_config, 0, sizeof(flash_config_t));

        /* Setup flash driver structure for device and initialize variables. */
        status = FLASH_Init(&FLASH0_DEV->flash_config);
        if (status == kStatus_FLASH_Success)
        {
        #if TARGET_DEBUG_LOG
            SPMLOG_DBGMSG("\r\n Successfully initialized flash\r\n ");
        #endif
        }
        else
        {
        #if TARGET_DEBUG_LOG
            SPMLOG_DBGMSG("\r\n NOR Flash initialization failed!\r\n");
        #endif
            return ARM_DRIVER_ERROR;
        }

        /* Need for Cache disable not seen currently. Keeping the code as 
         * a reference in case this is needed in future.
         */
#if 0
#if defined(K4_FLASH_ROMAPI)
        #if defined(SMSCM_CACHE_CLEAR_MASK) && SMSCM_CACHE_CLEAR_MASK
        /* disable flash cache/Prefetch */
            FLASH_CACHE_Disable();
        #endif /* SMSCM_CACHE_CLEAR_MASK */
#endif

#if defined(FLASH_DRIVER_IS_FLASH_RESIDENT) && (FLASH_DRIVER_IS_FLASH_RESIDENT == 1)        
        flash_cache_disable();
#endif
#endif

        #if TARGET_DEBUG_LOG
            SPMLOG_DBGMSG("\r\n***NOR Flash Initialization Success!***\r\n");
        #endif

        flash_init_is_done = true;
    }

    return ARM_DRIVER_OK;
}

static int32_t ARM_Flash_Uninitialize(void)
{
    flash_init_is_done = false;
    /* Nothing to be done */
    return ARM_DRIVER_OK;
}

static int32_t ARM_Flash_PowerControl(ARM_POWER_STATE state)
{
    switch (state) {
    case ARM_POWER_FULL:
        /* Nothing to be done */
        return ARM_DRIVER_OK;

    case ARM_POWER_OFF:
    case ARM_POWER_LOW:
    default:
        return ARM_DRIVER_ERROR_UNSUPPORTED;
    }
}

static int32_t ARM_Flash_ReadData(uint32_t addr, void *data, uint32_t cnt)
{
    static uint32_t status;
    bool is_valid = true;

    /* Conversion between data items and bytes */
    cnt *= data_width_byte[DriverCapabilities.data_width];

    /* Check Flash memory boundaries */
    is_valid = is_range_valid(FLASH0_DEV, addr + cnt);
    if(is_valid != true) {
    #if TARGET_DEBUG_LOG
        SPMLOG_DBGMSG("\r\n***NOR Flash Read error parameters!***\r\n");
    #endif
        return ARM_DRIVER_ERROR_PARAMETER;
    }

    /* Read Data */
    (void)memcpy(data, (uint8_t *)addr, cnt);

    cnt /= data_width_byte[DriverCapabilities.data_width];
    
    return ARM_DRIVER_OK;;
}

static int32_t ARM_Flash_ProgramData(uint32_t addr, const void *data, uint32_t cnt)
{
    static uint32_t status;
    uint32_t failedAddress, failedData;
    /* Conversion between data items and bytes */
    cnt *= data_width_byte[DriverCapabilities.data_width];

    /* Check Flash memory boundaries and alignment with minimum write size
    * (program_unit), data size also needs to be a multiple of program_unit. */
    if(!(is_range_valid(FLASH0_DEV, addr + cnt) &&
         is_write_aligned(FLASH0_DEV, addr)     &&
         is_write_aligned(FLASH0_DEV, cnt)      )) {
    #if TARGET_DEBUG_LOG
        SPMLOG_DBGMSG("\r\n***NOR Flash Program error parameters!***\r\n");
    #endif
        return ARM_DRIVER_ERROR_PARAMETER;
    }

    /* When programming cache, need to ensure no other execution happens from flash */
    struct critical_section_t cs = CRITICAL_SECTION_STATIC_INIT;
    CRITICAL_SECTION_ENTER(cs);
#if defined(K4_FLASH_ROMAPI)
    status = FLASH_Program(&FLASH0_DEV->flash_config, FMU0, addr, (uint32_t *)data, cnt);
#else
    status = FLASH_Program(&FLASH0_DEV->flash_config, FMU0, addr, (uint8_t *)data, cnt);
#endif    
    CRITICAL_SECTION_LEAVE(cs);
    if (status != kStatus_FLASH_Success) {
        return ARM_DRIVER_ERROR;
    }    

#if 1 /* Check result */
    {
        int i;

        for (i = 0; i < cnt; i++)
        {
            void *dst = (void *)addr;
	    if (memcmp(dst, data, cnt))
            {
            #if TARGET_DEBUG_LOG
                SPMLOG_DBGMSG("\r\n***NOR Flash program Check Failed!***\r\n");
            #endif
                break;
            }
        }
    }
#endif

    cnt /= data_width_byte[DriverCapabilities.data_width];
    return cnt;
}

static int32_t ARM_Flash_EraseSector(uint32_t addr)
{
    status_t status;

    if(!(is_range_valid(FLASH0_DEV, addr) &&
         is_erase_aligned(FLASH0_DEV, addr) )) {
    #if TARGET_DEBUG_LOG
        SPMLOG_DBGMSG("\r\n***NOR Flash Erase error parameters!***\r\n");
    #endif
        return ARM_DRIVER_ERROR_PARAMETER;
    }

    /* When programming cache, need to ensure no other execution happens from flash */
    struct critical_section_t cs = CRITICAL_SECTION_STATIC_INIT;
    CRITICAL_SECTION_ENTER(cs);
    status = FLASH_Erase(&FLASH0_DEV->flash_config, FMU0, addr, FLASH0_DEV->data->sector_size, kFLASH_ApiEraseKey);
    CRITICAL_SECTION_LEAVE(cs);
    if (status != kStatus_FLASH_Success) {
        return ARM_DRIVER_ERROR;
    }

     /* Verify sector if it's been erased. */
    CRITICAL_SECTION_ENTER(cs);
    status = FLASH_VerifyEraseSector(&FLASH0_DEV->flash_config, FMU0, addr, FLASH0_DEV->data->sector_size);
    CRITICAL_SECTION_LEAVE(cs);
    if (kStatus_FLASH_Success != status) {
        return ARM_DRIVER_ERROR;
    }
    
    /* Print message for user. */
    return ARM_DRIVER_OK;
}

static ARM_FLASH_STATUS ARM_Flash_GetStatus(void)
{
    return FlashStatus;
}

static ARM_FLASH_INFO * ARM_Flash_GetInfo(void)
{
    return FLASH0_DEV->data;
}

ARM_DRIVER_FLASH Driver_FLASH0 = {
    .GetVersion = ARM_Flash_GetVersion,
    .GetCapabilities = ARM_Flash_GetCapabilities,
    .Initialize = ARM_Flash_Initialize,
    .Uninitialize = ARM_Flash_Uninitialize,
    .PowerControl = ARM_Flash_PowerControl,
    .ReadData = ARM_Flash_ReadData,
    .ProgramData = ARM_Flash_ProgramData,
    .EraseSector = ARM_Flash_EraseSector,
    .GetStatus = ARM_Flash_GetStatus,
    .GetInfo = ARM_Flash_GetInfo
};
