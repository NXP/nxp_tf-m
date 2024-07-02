/*
 * Copyright (c) 2013-2018 ARM Limited. All rights reserved.
 * Copyright (c) 2019-2024 NXP
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
#include "fsl_romapi_iap.h"
#include "fsl_cache.h"
#include "tfm_spm_log.h"
#include "app.h"

#ifndef ARG_UNUSED
#define ARG_UNUSED(arg) ((void)arg)
#endif

/* Driver version */
#define ARM_FLASH_DRV_VERSION ARM_DRIVER_VERSION_MAJOR_MINOR(1, 0)

/* ARM FLASH device structure */
struct arm_flash_dev_t
{
    const uint32_t memory_base; /*!< FLASH memory base address */
    ARM_FLASH_INFO *data;       /*!< FLASH data */
};

/* Flash Status */
static ARM_FLASH_STATUS FlashStatus = {0, 0, 0};

/* Driver Version */
static const ARM_DRIVER_VERSION DriverVersion = {ARM_FLASH_API_VERSION, ARM_FLASH_DRV_VERSION};

/**
 * \brief Flash driver capability macro definitions \ref ARM_FLASH_CAPABILITIES
 */
/* Flash Ready event generation capability values */
#define EVENT_READY_NOT_AVAILABLE (0u)
#define EVENT_READY_AVAILABLE     (1u)
/* Data access size values */
#define DATA_WIDTH_8BIT  (0u)
#define DATA_WIDTH_16BIT (1u)
#define DATA_WIDTH_32BIT (2u)
/* Chip erase capability values */
#define CHIP_ERASE_NOT_SUPPORTED (0u)
#define CHIP_ERASE_SUPPORTED     (1u)

/* Driver Capabilities */
static const ARM_FLASH_CAPABILITIES DriverCapabilities = {
    EVENT_READY_NOT_AVAILABLE,
    DATA_WIDTH_8BIT, /*Data width set to 8 bit for RW610, as it being used in evaluating read size*/
    CHIP_ERASE_NOT_SUPPORTED};

#define FCB_ADDRESS (FLASH0_BASE_NS + 0x400U)

static ARM_FLASH_INFO ARM_FLASH0_DEV_DATA = {.sector_info  = NULL, /* Uniform sector layout */
                                             .sector_count = FLASH0_SIZE / FLASH0_SECTOR_SIZE,
                                             .sector_size  = FLASH0_SECTOR_SIZE,
                                             .page_size    = FLASH0_PAGE_SIZE,
                                             .program_unit = FLASH0_PROGRAM_UNIT,
                                             .erased_value = 0xFF};

static struct arm_flash_dev_t ARM_FLASH0_DEV = {
    // Romapi always expects the NS alias of a flash address.
    .memory_base = FLASH0_BASE_NS,
    .data        = &(ARM_FLASH0_DEV_DATA)};

struct arm_flash_dev_t *FLASH0_DEV = &ARM_FLASH0_DEV;

// Note, 4096 is not enough for sbloader API, don't blindly copy and paste!
__attribute__((aligned)) static uint8_t iap_api_arena[4096] = {0};
static bool iap_initialized        = false;
// The romapi context is shared for both of the IAP API based flash drivers.
__attribute__((aligned)) api_core_context_t g_context  = {0};

/* Functions */

static void disable_cache(flexspi_cache_status_t *cacheStatus)
{
    /* Disable cache */
    CACHE64_DisableCache(EXAMPLE_CACHE);
    cacheStatus->CacheEnableFlag = true;
}

static void enable_cache(flexspi_cache_status_t cacheStatus)
{
    if (cacheStatus.CacheEnableFlag)
    {
        /* Enable cache. */
        CACHE64_EnableCache(EXAMPLE_CACHE);
    }
}

/**
 * \brief      Check if the Flash memory boundaries are not violated.
 * \param[in]  flash_dev  Flash device structure \ref arm_flash_dev_t
 * \param[in]  offset     Highest Flash memory address which would be accessed.
 * \return     Returns true if Flash memory boundaries are not violated, false
 *             otherwise.
 */
static bool is_range_valid(struct arm_flash_dev_t *flash_dev, uint32_t offset)
{
    uint32_t flash_limit = 0;

    /* Calculating the highest address of the Flash memory address range */
    flash_limit = FLASH0_SIZE - 1;

    return (offset > flash_limit) ? (false) : (true);
}

static ARM_DRIVER_VERSION ARM_Flash_GetVersion(void)
{
    return DriverVersion;
}

static ARM_FLASH_CAPABILITIES ARM_Flash_GetCapabilities(void)
{
    return DriverCapabilities;
}

static int32_t ARM_Flash_Initialize(ARM_Flash_SignalEvent_t cb_event)
{
    if (!iap_initialized)
    {
        kp_api_init_param_t params = {0};
        params.allocStart          = (uint32_t)&iap_api_arena;
        params.allocSize           = sizeof(iap_api_arena);

        status_t status = iap_api_init(&g_context, &params);
        if (kStatus_Success != status)
        {
            return ARM_DRIVER_ERROR;
        }

        const uint32_t fcb_address = FCB_ADDRESS;

        status = iap_mem_config(&g_context, (uint32_t *)fcb_address, kMemoryID_FlexspiNor);
        if (kStatus_Success != status)
        {
            return ARM_DRIVER_ERROR;
        }
    }
    return ARM_DRIVER_OK;
}

static int32_t ARM_Flash_Uninitialize(void)
{
    if (iap_initialized)
    {
        status_t status = iap_api_deinit(&g_context);
        if (kStatus_Success != status)
        {
            return ARM_DRIVER_ERROR;
        }
    }
    /* Nothing to be done */
    return ARM_DRIVER_OK;
}

static int32_t ARM_Flash_PowerControl(ARM_POWER_STATE state)
{
    switch (state)
    {
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
    bool is_valid = true;

    /* Check Flash memory boundaries */
    is_valid = is_range_valid(FLASH0_DEV, addr + cnt);
    if (is_valid != true)
    {
#if TARGET_DEBUG_LOG
        SPMLOG_DBGMSG("\r\n***NOR Flash Read error parameters!***\r\n");
#endif
        return ARM_DRIVER_ERROR_PARAMETER;
    }

    /* Read Data */
    if (cnt)
    {
#if TARGET_DEBUG_LOG
        // TBD LOG_MSG("[OK] 0x%X => 0x%X (%d)\r\n", addr, data, cnt );
#endif
        uint8_t *src = (uint8_t *)(addr + FLASH0_DEV->memory_base);
        memcpy(data, src, cnt);
    }

    return ARM_DRIVER_OK;
}

static int32_t ARM_Flash_ProgramData(uint32_t addr, const void *data, uint32_t cnt)
{
    __disable_irq();

    flexspi_cache_status_t cacheStatus;
    disable_cache(&cacheStatus);

    status_t status = iap_mem_write(&g_context, ARM_FLASH0_DEV.memory_base + addr, cnt, data, kMemoryID_FlexspiNor);

    enable_cache(cacheStatus);
    __enable_irq();
    if (kStatus_Success != status)
    {
        return ARM_DRIVER_ERROR;
    }
    return ARM_DRIVER_OK;
}

static int32_t ARM_Flash_EraseSector(uint32_t addr)
{
    __disable_irq();
    flexspi_cache_status_t cacheStatus;
    disable_cache(&cacheStatus);

    status_t status =
        iap_mem_erase(&g_context, ARM_FLASH0_DEV.memory_base + addr, FLASH0_DEV->data->sector_size, kMemoryID_FlexspiNor);

    enable_cache(cacheStatus);
    __enable_irq();
    if (kStatus_Success != status)
    {
        return ARM_DRIVER_ERROR;
    }
    return ARM_DRIVER_OK;
}

static int32_t ARM_Flash_EraseChip(void)
{
    return ARM_DRIVER_ERROR_UNSUPPORTED;
}

static ARM_FLASH_STATUS ARM_Flash_GetStatus(void)
{
    return FlashStatus;
}

static ARM_FLASH_INFO *ARM_Flash_GetInfo(void)
{
    return FLASH0_DEV->data;
}

ARM_DRIVER_FLASH Driver_FLASH0 = {ARM_Flash_GetVersion,   ARM_Flash_GetCapabilities, ARM_Flash_Initialize,
                                  ARM_Flash_Uninitialize, ARM_Flash_PowerControl,    ARM_Flash_ReadData,
                                  ARM_Flash_ProgramData,  ARM_Flash_EraseSector,     ARM_Flash_EraseChip,
                                  ARM_Flash_GetStatus,    ARM_Flash_GetInfo};
