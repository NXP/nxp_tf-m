/*
 * Copyright (c) 2013-2018 ARM Limited. All rights reserved.
 * Copyright 2019-2024 NXP
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

#include "Driver_Common.h"
#include "RW612.h"
#include "fsl_common.h"
#include "target_cfg.h"
#include "Driver_Flash.h"
#include "platform_base_address.h"
#include "flash_layout.h"
#include "fsl_romapi_iap.h"
#include "fsl_cache.h"
#include "fsl_iped.h"
#include "tfm_sp_log.h"
#include "tfm_platform_api.h"
#include "tfm_plat_nv_counters.h"

#include "app.h"

#include "mcuxClEls.h"
#include "mcuxClEls_Aead.h"

#ifdef UNITTEST_ENV
#include "unittest.h"
#endif

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

#define TRACE_SECTOR_WRITE(fmt, ...)
#define TRACE_SECTOR_ERASE(fmt, ...)
#define TRACE_PAGE_WRITE(fmt, ...)
#define TRACE_IPED_CONFIG(fmt, ...)
#define TRACE_READ(fmt, ...)
#define TRACE_CACHE_CLEAR()
#define TRACE_AHB_BUFFER_CLEAR()
#define TRACE_DATA(name, addr, size)
#define TRACE_ROLLBACK_PROTECTION(fmt, ...)

#define PLOG_ERROR(fmt, ...)

#define TRACE_ENABLED (0u)
#if TRACE_ENABLED
#undef TRACE_SECTOR_WRITE
#define TRACE_SECTOR_WRITE(fmt, ...)        LOG_DBGFMT(fmt "\n" __VA_OPT__(,) __VA_ARGS__)
#undef TRACE_SECTOR_ERASE
#define TRACE_SECTOR_ERASE(fmt, ...)        LOG_DBGFMT(fmt "\n" __VA_OPT__(,) __VA_ARGS__)
#undef TRACE_PAGE_WRITE
#define TRACE_PAGE_WRITE(fmt, ...)          LOG_DBGFMT(fmt "\n" __VA_OPT__(,) __VA_ARGS__)
#undef TRACE_IPED_CONFIG
#define TRACE_IPED_CONFIG(fmt, ...)         LOG_DBGFMT(fmt "\n" __VA_OPT__(,) __VA_ARGS__)
#undef TRACE_READ
#define TRACE_READ(fmt, ...)                LOG_DBGFMT(fmt "\n" __VA_OPT__(,) __VA_ARGS__)
#undef TRACE_DATA
#define TRACE_DATA(name, addr, size) printf_buffer(name, addr, size)
#undef TRACE_ROLLBACK_PROTECTION
#define TRACE_ROLLBACK_PROTECTION(fmt, ...) LOG_DBGFMT(fmt "\n" __VA_OPT__(,) __VA_ARGS__)
#endif

// Uncomment those for enabling printout of error messages
#undef PLOG_ERROR
#define PLOG_ERROR(fmt, ...) LOG_ERRFMT(fmt "\n" __VA_OPT__(,) __VA_ARGS__)

#define ASSERT_OR_EXIT(condition, fmt, ...)              \
    {                                                    \
        if (!(condition))                                \
        {                                                \
            PLOG_ERROR(fmt __VA_OPT__(,) __VA_ARGS__);   \
            status = ARM_DRIVER_ERROR;                   \
            goto exit;                                   \
        }                                                \
    }

#define ASSERT_OR_EXIT_EN_IRQ(condition, fmt, ...)       \
    {                                                    \
        if (!(condition))                                \
        {                                                \
            __enable_irq();                              \
            PLOG_ERROR(fmt __VA_OPT__(,) __VA_ARGS__);   \
            status = ARM_DRIVER_ERROR;                   \
            goto exit;                                   \
        }                                                \
    }

#define ASSERT_STATUS_OK_OR_EXIT(fmt, ...)                                         \
    {                                                                              \
        if (status != ARM_DRIVER_OK)                                               \
        {                                                                          \
            PLOG_ERROR(fmt "0x%x (%d)" __VA_OPT__(,) __VA_ARGS__, status, status); \
            goto exit;                                                             \
        }                                                                          \
    }

// User configurations can be passed via yml/cmake to configure certain features
// By default all features/configurations are enabled for RW61x
#if defined (RW61X_IPED_METADATA_WRITE_ENABLE)
#define IPED_METADATA_WRITE_USER_CONFIG RW61X_IPED_METADATA_WRITE_ENABLE
#else
#define IPED_METADATA_WRITE_USER_CONFIG 1
#endif

#if defined (RW61X_IPED_ENCRYPT_ENABLE)
#define IPED_ENCRYPT_USER_CONFIG RW61X_IPED_ENCRYPT_ENABLE
#else
#define IPED_ENCRYPT_USER_CONFIG 1
#endif

#if defined (RW61X_IPED_IV_ENCRYPT_ENABLE)
#define IPED_IV_ENCRYPT_USER_CONFIG RW61X_IPED_IV_ENCRYPT_ENABLE
#else
#define IPED_IV_ENCRYPT_USER_CONFIG 1
#endif

#if defined (RW61X_IPED_ITS_ROLLBACK_PROTECTION_ENABLE)
#define IPED_ROLLBACK_PROTECTION_USER_CONFIG RW61X_IPED_ITS_ROLLBACK_PROTECTION_ENABLE
#else
#define IPED_ROLLBACK_PROTECTION_USER_CONFIG 0
#endif

// To disable certain features (for easier debugging) the following can be adapted. Note, some features
// have dependencies, disabling one, also disables all that depend on it.
#define FLASH0_IPED_METADATA_WRITE_ENABLED      (IPED_METADATA_WRITE_USER_CONFIG)
#define FLASH0_IPED_ENCRYPT_ENABLED             (IPED_ENCRYPT_USER_CONFIG & FLASH0_IPED_METADATA_WRITE_ENABLED)
#define FLASH0_IPED_IV_ENCRYPT_ENABLED          (IPED_IV_ENCRYPT_USER_CONFIG & FLASH0_IPED_ENCRYPT_ENABLED)
#define FLASH0_IPED_ROLLBACK_PROTECTION_ENABLED (IPED_ROLLBACK_PROTECTION_USER_CONFIG & FLASH0_IPED_IV_ENCRYPT_ENABLED)

#define SECTOR_METADATA_ID (0x4980534D)
#define IPED_IV_SIZE       (8u)

//
// Note, that the sector_versions field is used as AAD for the sector authenticity check. The hardware accelerator
// mandates that the size of the AAD input is a multiple of its block size. In order to avoid having to copy data, we
// make the field itself already suitable. As one element of the array is 4 bytes, and the blocksize is 16 bytes, it is
// necessary and sufficient that the number of elements is divisible by 4.
#define SECTOR_VERSIONS_ELEMENT_COUNT (((FLASH0_IPED_SECTOR_COUNT + 3U) / 4U) * 4U)

// There are sector versions stored in the metadata part of a sector. In particular the main sector needs to hold sector
// version numbers for all other sectors to form a chain of protection. There is only limited capacity in the sector
// metadata. This value gives the upper bound for the number of sectors that can be used based on the available space.
#define MAX_SECTOR_COUNT                                                                                     \
    ((FLASH0_PAGE_SIZE - sizeof(uint32_t) - MCUXCLCSS_AEAD_IV_BLOCK_SIZE - MCUXCLCSS_CIPHER_BLOCK_SIZE_AES - \
      MCUXCLCSS_CIPHER_BLOCK_SIZE_AES) /                                                                     \
     sizeof(uint32_t))

// Size of the IV used to encrypt the sector's IVs
#define METADATA_IV_SIZE (96U / 8U)

#if defined(static_assert)
static_assert(SECTOR_VERSIONS_ELEMENT_COUNT <= MAX_SECTOR_COUNT,
              "Too many sectors - not enough space to store metadata in page!");
static_assert(METADATA_IV_SIZE == (96U / 8U), "This implementation is designed to work with IV size of 96 bits!");
#endif

#if FLASH0_IPED_ROLLBACK_PROTECTION_ENABLED
// The rollback counters designated for ITS rollback protection as identified by the platform service.
#define ROLLBACK_COUNTER_0 PLAT_NV_COUNTER_ITS_0
#define ROLLBACK_COUNTER_1 PLAT_NV_COUNTER_ITS_1
#endif

#if TRACE_ENABLED

static const char nibble_to_char[16] = {
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f',
};
static bool buffer_log_enable = true;

void printf_buffer(const char *name, const unsigned char *buffer, size_t size)
{
    if (!buffer_log_enable)
        return;
    if ((NULL == name) || (NULL == buffer))
        return;

    const unsigned char *src = buffer;
    char chunk[33];
    size_t remaining = size;
    while (remaining > 0)
    {
        size_t block_size = (remaining > sizeof(chunk) / 2) ? (sizeof(chunk) / 2) : remaining;
        char *dst         = &chunk[0];
        for (size_t i = 0; i < block_size; i++)
        {
            *dst++ = nibble_to_char[((*src) & 0xf0) >> 4];
            *dst++ = nibble_to_char[(*src++) & 0x0f];
        }
        *dst = 0x00;
        LOG_DBGFMT("%s (0x%p): %s\r\n", name, src, chunk);
        remaining -= block_size;
    }
}

typedef struct IPED_context_t
{
    __IO uint32_t IV0;   /**< IPED context IV0 */
    __IO uint32_t IV1;   /**< IPED context IV1 */
    __IO uint32_t START; /**< Start address of region  */
    __IO uint32_t END;   /**< End address of region  */
    __IO uint32_t AAD0;  /**< IPED context AAD0 */
    __IO uint32_t AAD1;  /**< IPED context AAD1 */
} IPED_context_t;

static const uint32_t context_offset_diff =
    ((uint32_t)&EXAMPLE_FLEXSPI->IPEDCTX1IV0) - ((uint32_t)&EXAMPLE_FLEXSPI->IPEDCTX0IV0);
static IPED_context_t *get_iped_context_regs_r(iped_region_t region_nr)
{
    uint32_t context_offset      = context_offset_diff * region_nr;
    IPED_context_t *iped_context = (IPED_context_t *)(((uint32_t)&EXAMPLE_FLEXSPI->IPEDCTX0IV0) + context_offset);
    return iped_context;
}

#endif

/** @brief This is the metadata stored for each of the sectors. */
typedef struct _persistent_metadata_t
{
    uint32_t id;                                             //! A field to hint that the metadata structure is valid.
    uint8_t metadata_iv[METADATA_IV_SIZE];                   //! The IV to use for AEAD on the metadata.
    uint32_t sector_versions[SECTOR_VERSIONS_ELEMENT_COUNT]; //! A version number for each sector.
    uint8_t sector_iv[IPED_IV_SIZE];                         //! The encrypted IV used for IPED.
    uint8_t tag[MCUXCLCSS_CIPHER_BLOCK_SIZE_AES];            //! A tag protecting the sector metadata.
} persistent_metadata_t;

/** @brief In order not to have to repeatedly decrypt IVs of sectors, a small cache is used to store some of them - this
 * is the structure of the data used to store a single entry in that cache. */
typedef struct _cached_metadata_t
{
    uint32_t sector_nr;
    bool sector_is_encrypted;
    uint8_t plain_iv[IPED_IV_SIZE];
} cached_metadata_t;

typedef struct _ahb_configuration_t
{
    uint32_t sector_nr;
    bool use_iped;
} ahb_configuration_t;

static const uint32_t encrypted_area_start_offset = FLASH0_IPED_OFFSET;

static const uint32_t sector_size_log    = FLASH0_IPED_SECTOR_SIZE_LOG;
static const uint32_t sector_size_phy    = FLASH0_IPED_SECTOR_SIZE_PHY;
static const uint32_t sector_count       = FLASH0_IPED_SECTOR_COUNT;
static const uint32_t page_size          = FLASH0_IPED_PAGE_SIZE;
static const uint32_t pages_per_sector   = FLASH0_IPED_SECTOR_SIZE_LOG / FLASH0_IPED_PAGE_SIZE;
static const uint32_t flash_base_address = FLASH0_BASE_NS;

// To encrypt the IVs used for IPED, this code relies on an encryption key being
// present in an S50 keyslot. This is the keyslot number that this key shall be in.
// Note that the NXP bootloader does leave a suitable key in keyslot 3 which can be reused.
static const uint32_t iv_encryption_key_slot = 3U;

static ARM_FLASH_INFO ARM_FLASH0_IPED_DEV_DATA = {.sector_info  = NULL, /* Uniform sector layout */
                                                  .sector_count = FLASH0_IPED_SECTOR_COUNT,
                                                  .sector_size  = FLASH0_IPED_SECTOR_SIZE_LOG,
                                                  .page_size    = FLASH0_IPED_PAGE_SIZE,
                                                  .program_unit = FLASH0_IPED_PROGRAM_UNIT,
                                                  .erased_value = 0xFF};

extern ARM_DRIVER_FLASH Driver_FLASH0;
extern api_core_context_t g_context;

/* Cache for storing metadata of sectors (in particular not to have to repeatedly decrypt IVs). Sector 0 and sector 1
 * are used to hold filesystem information and are accessed very regularly, so the cache slots 0 and 1 are reserved for
 * sector 0 and 1 respectively. Slot 2 is shared among data sectors. */
static cached_metadata_t metadata_cache[3]                     = {0};
static uint32_t sector_versions[SECTOR_VERSIONS_ELEMENT_COUNT] = {0};

/* Used to keep track of the currently used configuration for AHB accesses on flash. It is required to determine when
 * AHB buffer clearing is required. */
static ahb_configuration_t ahb_configuration = {.sector_nr = UINT32_MAX};

/* Functions */

#if FLASH0_IPED_ROLLBACK_PROTECTION_ENABLED
static int32_t increment_rollback_counter(uint32_t counter_id);
static int32_t get_rollback_counter(uint32_t counter_id, uint32_t *val);
static int32_t align_rollback_counters();
#endif // FLASH0_IPED_ROLLBACK_PROTECTION_ENABLED

static bool is_valid_addr_range(uint32_t addr_log, uint32_t cnt);
static bool is_sector_start_addr(uint32_t addr_log);
static uint32_t get_sector_nr(uint32_t addr_log);
static uint32_t get_offset_in_sector(uint32_t addr_log);
static uint32_t get_sector_start_addr_phy(uint32_t sector_nr);
static uint32_t get_sector_end_addr_phy(uint32_t sector_nr);
static uint32_t get_sector_metadata_addr_phy(uint32_t sector_nr);
static bool is_iv_encryption_key_loaded(void);
static int32_t perform_crypto_operation(const uint8_t *iv,
                                        size_t iv_size,
                                        const uint8_t *aad,
                                        size_t aad_size,
                                        const uint8_t *src,
                                        uint8_t *dst,
                                        size_t data_size,
                                        uint8_t *tag,
                                        size_t tag_size,
                                        bool is_encrypt);
static int32_t encrypt_sector_metadata(uint32_t sector_nr, const cached_metadata_t *src, persistent_metadata_t *dst);
static int32_t decrypt_sector_metadata(uint32_t sector_nr, const persistent_metadata_t *src, cached_metadata_t *dst);
static bool is_valid_sector_metadata(const persistent_metadata_t *metadata);

/** Look in the metadata cache for the entry for the given sector nr. */
static const cached_metadata_t *find_cached_metadata(uint32_t sector_nr);

/** Copy the given data to the metadata cache. Returns a pointer to the updated slot in the cache. */
static const cached_metadata_t *metadata_cache_update_from_struct(const cached_metadata_t *cached_metadata);

/** Look up the flash memory contents for valid metadata and put the information found into the metadata cash. If the
 * sector hold no valid metadata, it is marked as non-encrypted in the metadata cache, but still the cache information
 * is updated. Returns a pointer to the updated slot in the cache. As a prerequisite, the AHB configuration must be
 * set-up for plain reads on the given sector. */
static const cached_metadata_t *metadata_cache_update_from_flash(uint32_t sector_nr);

/** Return whether the current configuration of the AHB is fitting to read the given sector number and encryption
 * settings. */
static bool is_ahb_configured_for_sector_read(uint32_t sector_nr, bool use_iped);

/** Clear the AHB read buffer. */
static void clear_ahb_buffer();

/** Configure the AHB to be able to read from the given sector and the given encryption setting.
 * This function takes information from the metadata cache, if available. If not, it will read metadata from the given
 * secor and update the metadata cache. */
static int32_t setup_ahb_for_sector_read(uint32_t sector_nr, bool use_iped);

/** Enable an IPED region to cover the given sector. The information for the IV is taken from the passed metadata, the
 * AAD is taken from the maintained list of sector_versions. */
static void enable_iped(uint32_t sector_nr, const cached_metadata_t *cfg);

/** Draw a new random number as IV, update the metadata cache with the new IV and finally also enable an IPED region
 * for the sector. The function also updates the metadata cache. */
static int32_t enable_iped_for_writing_new_sector(uint32_t sector_nr, const cached_metadata_t **cached_metadata);

/** Get the minimum and maximum allowed main sector version based on the rolback counter values. */
static int32_t get_valid_sector_version_range(uint32_t *min_version_nr, uint32_t *max_version_nr);

/** Inspect the two main sectors and return the sector number of the one with the higher sector version nunber. */
static int32_t get_most_recent_main_sector_nr(uint32_t *sector_nr);

/** Initialize the sector_versions list - a copy in RAM of all the version numbers of all sectors used for rollback
 * protection. This function does consider the rollback counters and returns an error in case a violation of the
 * constraints imposed by rollback counters is encountered. */
static int32_t initialize_sector_versions();

#if FLASH0_IPED_ROLLBACK_PROTECTION_ENABLED
/** Increment the version number of a sector in the sector_versions list. */
static int32_t increment_sector_version(uint32_t sector_nr);

/** Set the version number of a given sector in the sector_versions list. */
static int32_t set_sector_version(uint32_t sector_nr, uint32_t sector_version);
#endif /* FLASH0_IPED_ROLLBACK_PROTECTION_ENABLED */
/** Callback to perform operations before programming a sector update. This is used to increment sector versions. For
 * main sectors this also increments a rollback counter. */
static int32_t pre_update_sector(uint32_t sector_nr);

/** Callback to perform operations before programming a sector update. For main sectors this aligns the rollback
 * counters. */
static int32_t post_update_sector(uint32_t sector_nr);

/** Clear the cache64. */
static void clear_cache();

//
// Notes on hardware behavior on caching/buffering:
//
// AHB buffering:
//   The FLEXSPI controller buffers across IPED region boundaries without considering the correct encryptions settings.
//   E.g., with an IPED region spanning the address range from 0x08120000 to 0x08120c00, reading at an unencrypted
//   address 0x0811ffe0 buffers ciphertext of the region. Now a consecutive read from an address 0x08120000
//   may hit the AHB buffer from the read before and return ciphertext that was copied into the buffer instead of doing
//   a proper decryption. The same holds at the end of a region as well. Reading from 0x08120be0 does perform decryption
//   operations and buffer the results in the AHB buffer. A consecutive read from (outsize of the IPED region)
//   0x08120f00, can hit the buffer of the previous read and return unexpected data.
//
//   Even two reads that are both encrypted but are in different sectors can be affected. The last address of a sector
//   that is encrypted is at 0x081200cff, the first address of the next sector that is encrypted (but with different
//   IPED configuration!) is at 0x08120100, the difference between those two is less than 0x400, which means that a
//   fully filled prefetch buffer would contain data of the second sector.
//
//   Effectively this means that all reads close to region boundaries are dangerous and we need to clear AHB buffer
//   whenever we switch between different IPED settings.
//
//   To do that, we keep track which read we did last and call a function setup_ahb_for_sector_read() before AHB
//   accesses on Flash.
//
// Caching:
//   Data from the prefetch buffer does only get copied to the cache64 in (aligned) chunks of 32 bytes. Therefore it is
//   not affected by the wrong prefetch behavior mentioned above. This was confirmed with the IP team.
//
//   Caches become inconsistent if for one address either the encryption setting changes (understandably a cache can not
//   keep track of this), or an address gets programmed (programming is done with IP writes which bypass cache) or
//   erased. In both cases a manual invalidation of the cache is required.
//

#if FLASH0_IPED_ROLLBACK_PROTECTION_ENABLED
static int32_t increment_rollback_counter(uint32_t counter_id)
{
    TRACE_ROLLBACK_PROTECTION("increment_rollback_counter counter_id: %d", counter_id);
    enum tfm_platform_err_t err = tfm_platform_nv_counter_increment(counter_id);
    if (err != TFM_PLATFORM_ERR_SUCCESS)
    {
        return err;
    }
    return ARM_DRIVER_OK;
}

static int32_t get_rollback_counter(uint32_t counter_id, uint32_t *val)
{
    TRACE_ROLLBACK_PROTECTION("get counter_id: %d", counter_id);
    enum tfm_platform_err_t err = tfm_platform_nv_counter_read(counter_id, sizeof(*val), (uint8_t *)val);
    if (err != TFM_PLATFORM_ERR_SUCCESS)
    {
        return err;
    }
    TRACE_ROLLBACK_PROTECTION("    counter value: %d", *val);
    return ARM_DRIVER_OK;
}

static int32_t align_rollback_counters()
{
    TRACE_ROLLBACK_PROTECTION("align_rollback_counters");
    int32_t status                = ARM_DRIVER_OK;
    uint32_t rollback_counters[2] = {0};

    status = get_rollback_counter(ROLLBACK_COUNTER_0, &rollback_counters[0]);
    ASSERT_STATUS_OK_OR_EXIT("get_rollback_counter failed: ");

    status = get_rollback_counter(ROLLBACK_COUNTER_1, &rollback_counters[1]);
    ASSERT_STATUS_OK_OR_EXIT("get_rollback_counter failed: ");

    ASSERT_OR_EXIT(rollback_counters[0] >= rollback_counters[1], "rollback counter 0 (%d) < rollback counter 1 (%d)",
                   rollback_counters[0], rollback_counters[1]);

    while (rollback_counters[1] < rollback_counters[0])
    {
        status = increment_rollback_counter(ROLLBACK_COUNTER_1);
        ASSERT_STATUS_OK_OR_EXIT("increment_rollback_counter failed: ");

        status = get_rollback_counter(ROLLBACK_COUNTER_1, &rollback_counters[1]);
        ASSERT_STATUS_OK_OR_EXIT("get_rollback_counter failed: ");
    }

exit:
    return status;
}
#endif // FLASH0_IPED_ROLLBACK_PROTECTION_ENABLED

static bool is_valid_addr_range(uint32_t addr_log, uint32_t cnt)
{
    if (addr_log < encrypted_area_start_offset)
    {
        return false;
    }
    uint32_t offset = addr_log - encrypted_area_start_offset;
    if ((offset + cnt) > (sector_size_log * sector_count))
    {
        return false;
    }
    return true;
}

static bool is_sector_start_addr(uint32_t addr_log)
{
    uint32_t offset = addr_log - encrypted_area_start_offset;
    return offset % sector_size_log == 0;
}

static uint32_t get_sector_nr(uint32_t addr_log)
{
    uint32_t offset         = addr_log - encrypted_area_start_offset;
    uint32_t offset_sectors = offset / sector_size_log;
    return offset_sectors;
}

static uint32_t get_offset_in_sector(uint32_t addr_log)
{
    uint32_t offset           = addr_log - encrypted_area_start_offset;
    uint32_t offset_in_sector = offset % sector_size_log;
    return offset_in_sector;
}

static uint32_t get_sector_start_addr_phy(uint32_t sector_nr)
{
    uint32_t sector_start_addr_phy = flash_base_address + encrypted_area_start_offset + sector_nr * sector_size_phy;
    return sector_start_addr_phy;
}

static uint32_t get_sector_end_addr_phy(uint32_t sector_nr)
{
    uint32_t sector_start_addr_phy = get_sector_start_addr_phy(sector_nr);
    return sector_start_addr_phy + sector_size_log;
}

static uint32_t get_sector_metadata_addr_phy(uint32_t sector_nr)
{
    uint32_t sector_start_addr_phy = get_sector_start_addr_phy(sector_nr);
    return sector_start_addr_phy + sector_size_phy - page_size;
}

static bool is_iv_encryption_key_loaded(void)
{
    return ((__IO uint32_t *)&(ELS->ELS_KS0))[iv_encryption_key_slot] & ELS_ELS_KS0_KS0_KACT_MASK;
}

static int32_t perform_crypto_operation(const uint8_t *iv,
                                        size_t iv_size,
                                        const uint8_t *aad,
                                        size_t aad_size,
                                        const uint8_t *src,
                                        uint8_t *dst,
                                        size_t data_size,
                                        uint8_t *tag,
                                        size_t tag_size,
                                        bool is_encrypt)
{
    int32_t status = ARM_DRIVER_OK;
    ASSERT_OR_EXIT(is_iv_encryption_key_loaded(), "is_iv_encryption_key_loaded returned false");

    mcuxClCss_AeadOption_t options               = {.word = 0U};
    uint8_t css_ctx[MCUXCLCSS_AEAD_CONTEXT_SIZE] = {0U};

    options.bits.dcrpt  = is_encrypt ? (uint8_t)MCUXCLCSS_AEAD_ENCRYPT : (uint8_t)MCUXCLCSS_AEAD_DECRYPT;
    options.bits.extkey = 0;

    /* Initialise the AEAD operation with the random IV */
    // The crypto hardware accelerator requires this to be a multiple of it's blocksize (16). Ceil the size to the next
    // multiple of 16. Also it must be pre-padded. For the 96 bit IV, the NIST spec. mandates the last byte of the block
    // being 1. This is not a generic implementation, an IV size of 96 bits is mandated!
    assert(iv_size == (96 / 8));
    uint8_t iv_buffer[MCUXCLCSS_AEAD_IV_BLOCK_SIZE];
    memset(iv_buffer, 0, sizeof(iv_buffer));
    memcpy(iv_buffer, iv, iv_size);
    iv_buffer[sizeof(iv_buffer) - 1] = 0x01;
    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(
        result, token,
        mcuxClCss_Aead_Init_Async(options, iv_encryption_key_slot, NULL, 0, iv_buffer, sizeof(iv_buffer), css_ctx));
    ASSERT_OR_EXIT(
        MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClCss_Aead_Init_Async) == token && MCUXCLCSS_STATUS_OK_WAIT == result,
        "mcuxClCss_Aead_Init_Async failed: 0x%x", result);
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(result, token, mcuxClCss_WaitForOperation(MCUXCLCSS_ERROR_FLAGS_CLEAR));
    ASSERT_OR_EXIT(MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClCss_WaitForOperation) == token && MCUXCLCSS_STATUS_OK == result,
                   "mcuxClCss_WaitForOperation failed: 0x%x", result);
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    /* Setup the AAD */
    // The crypto hardware accelerator requires this to be a multiple of it's blocksize (16). This is not a generic
    // implementation, we do mandate that the caller takes care of that.
    assert((aad_size % MCUXCLCSS_AEAD_AAD_BLOCK_SIZE) == 0);
    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(
        result, token,
        mcuxClCss_Aead_UpdateAad_Async(options, iv_encryption_key_slot, NULL, 0, aad, aad_size, css_ctx));
    ASSERT_OR_EXIT(
        MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClCss_Aead_UpdateAad_Async) == token && MCUXCLCSS_STATUS_OK_WAIT == result,
        "mcuxClCss_Aead_UpdateAad_Async failed: 0x%x", result);
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(result, token, mcuxClCss_WaitForOperation(MCUXCLCSS_ERROR_FLAGS_CLEAR));
    ASSERT_OR_EXIT(MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClCss_WaitForOperation) == token && MCUXCLCSS_STATUS_OK == result,
                   "mcuxClCss_WaitForOperation failed: 0x%x", result);
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    /* Perform the encryption/decryption  */
    // The crypto hardware accelerator requires this to be a multiple of it's blocksize (16). This is not a generic
    // implementation, we do mandate that the data is less than one block in size!
    assert(data_size <= MCUXCLCSS_CIPHER_BLOCK_SIZE_AES);
    uint8_t block_buffer[MCUXCLCSS_CIPHER_BLOCK_SIZE_AES];
    memset(block_buffer, 0, sizeof(block_buffer));
    memcpy(block_buffer, src, data_size);
    options.bits.msgendw = (data_size & 0xF);
    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(
        result, token,
        mcuxClCss_Aead_UpdateData_Async(options, iv_encryption_key_slot, NULL, 0, block_buffer, sizeof(block_buffer),
                                        block_buffer, css_ctx));
    ASSERT_OR_EXIT(
        MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClCss_Aead_UpdateData_Async) == token && MCUXCLCSS_STATUS_OK_WAIT == result,
        "mcuxClCss_Aead_UpdateData_Async failed: 0x%x", result);
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(result, token, mcuxClCss_WaitForOperation(MCUXCLCSS_ERROR_FLAGS_CLEAR));
    ASSERT_OR_EXIT(MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClCss_WaitForOperation) == token && MCUXCLCSS_STATUS_OK == result,
                   "mcuxClCss_WaitForOperation failed: 0x%x", result);
    MCUX_CSSL_FP_FUNCTION_CALL_END();
    memcpy(dst, block_buffer, data_size);

    /* Generate (and verify) the tag */
    uint8_t tag_buffer[MCUXCLCSS_AEAD_TAG_SIZE] = {0};
    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(result, token,
                                     mcuxClCss_Aead_Finalize_Async(options, iv_encryption_key_slot, NULL, 0, aad_size,
                                                                   data_size, tag_buffer, css_ctx));
    ASSERT_OR_EXIT(
        MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClCss_Aead_Finalize_Async) == token && MCUXCLCSS_STATUS_OK_WAIT == result,
        "mcuxClCss_Aead_Finalize_Async failed: 0x%x", result);
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(result, token, mcuxClCss_WaitForOperation(MCUXCLCSS_ERROR_FLAGS_CLEAR));
    ASSERT_OR_EXIT(MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClCss_WaitForOperation) == token && MCUXCLCSS_STATUS_OK == result,
                   "mcuxClCss_WaitForOperation failed: 0x%x", result);
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    /* In case of an encrypt, return the calculated tag. */
    if (is_encrypt)
    {
        memcpy(tag, tag_buffer, tag_size);
    }
    /* In case of an encrypt, return the calculated tag. */
    else
    {
        ASSERT_OR_EXIT(memcmp(tag, tag_buffer, tag_size) == 0, "Tag verification failed");
    }
exit:
    return status;
}

static int32_t encrypt_sector_metadata(uint32_t sector_nr, const cached_metadata_t *src, persistent_metadata_t *dst)
{
#if FLASH0_IPED_IV_ENCRYPT_ENABLED
    int32_t status = ARM_DRIVER_OK;
    dst->id        = SECTOR_METADATA_ID;
    memcpy(dst->sector_versions, sector_versions, sizeof(sector_versions));

    /* Get random IV for sector metadata encryption. */
    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(result, token,
                                     mcuxClCss_Rng_DrbgRequest_Async(dst->metadata_iv, sizeof(dst->metadata_iv)));
    ASSERT_OR_EXIT(
        MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClCss_Rng_DrbgRequest_Async) == token && MCUXCLCSS_STATUS_OK_WAIT == result,
        "mcuxClCss_Rng_DrbgRequest_Async failed: 0x%x", result);
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(result, token, mcuxClCss_WaitForOperation(MCUXCLCSS_ERROR_FLAGS_CLEAR));
    ASSERT_OR_EXIT(MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClCss_WaitForOperation) == token && MCUXCLCSS_STATUS_OK == result,
                   "mcuxClCss_WaitForOperation failed: 0x%x", result);
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    status = perform_crypto_operation(dst->metadata_iv, sizeof(dst->metadata_iv), (uint8_t *)sector_versions,
                                      sizeof(sector_versions), src->plain_iv, dst->sector_iv, sizeof(src->plain_iv),
                                      dst->tag, sizeof(dst->tag), true);

    ASSERT_OR_EXIT(status == ARM_DRIVER_OK, "perform_crypto_operation failed: 0x%x", status);

exit:
    return status;
#else
    dst->id = SECTOR_METADATA_ID;
    memcpy(dst->sector_versions, sector_versions, sizeof(sector_versions));
    memset(dst->metadata_iv, 0, sizeof(dst->metadata_iv));
    memcpy(dst->sector_iv, src->plain_iv, sizeof(src->plain_iv));
    memset(dst->tag, 42, sizeof(dst->tag));
    return ARM_DRIVER_OK;
#endif
}

static int32_t decrypt_sector_metadata(uint32_t sector_nr, const persistent_metadata_t *src, cached_metadata_t *dst)
{
#if FLASH0_IPED_IV_ENCRYPT_ENABLED
    int32_t status = ARM_DRIVER_OK;

    ASSERT_OR_EXIT(src->sector_versions[sector_nr] == sector_versions[sector_nr],
                   "Unexpected sector version: %d", src->sector_versions[sector_nr]);

    status = perform_crypto_operation(src->metadata_iv, sizeof(src->metadata_iv), (uint8_t *)&src->sector_versions[0],
                                      sizeof(src->sector_versions), src->sector_iv, dst->plain_iv,
                                      sizeof(src->sector_iv), (uint8_t *)src->tag, sizeof(src->tag), false);

    ASSERT_OR_EXIT(status == ARM_DRIVER_OK, "perform_crypto_operation failed: 0x%x", status);

exit:
    return status;
#else
    memcpy(dst->plain_iv, src->sector_iv, sizeof(dst->plain_iv));
    return ARM_DRIVER_OK;
#endif
}

static bool is_valid_sector_metadata(const persistent_metadata_t *metadata)
{
    TRACE_IPED_CONFIG("found metadata id: 0x%x", metadata->id);
    return metadata->id == SECTOR_METADATA_ID;
}

// The caching strategy for metadata is to keep the metadata for sector 0 and 1 alive at all times in slots 0 and 1
// respectively. Slot 2 is swapped on ever access to a data sector.

static const cached_metadata_t *find_cached_metadata(uint32_t sector_nr)
{
    if (sector_nr < 2)
    {
        if (metadata_cache[sector_nr].sector_nr == sector_nr)
        {
            return &metadata_cache[sector_nr];
        }
    }
    else
    {
        for (size_t i = 2; i < sizeof(metadata_cache) / sizeof(metadata_cache[0]); i++)
        {
            if (metadata_cache[i].sector_nr == sector_nr)
            {
                return &metadata_cache[i];
            }
        }
    }
    return NULL;
}

static const cached_metadata_t *metadata_cache_update_from_struct(const cached_metadata_t *cached_metadata)
{
    uint32_t slot_nr = (cached_metadata->sector_nr < 2) ? cached_metadata->sector_nr : 2;
    memcpy(&metadata_cache[slot_nr], cached_metadata, sizeof(*cached_metadata));
    return &metadata_cache[slot_nr];
}

static const cached_metadata_t *metadata_cache_update_from_flash(uint32_t sector_nr)
{
    persistent_metadata_t *persistent_metadata = (persistent_metadata_t *)get_sector_metadata_addr_phy(sector_nr);
    cached_metadata_t cached_metadata          = {.sector_nr           = sector_nr,
                                                  .sector_is_encrypted = is_valid_sector_metadata(persistent_metadata)};
    if (cached_metadata.sector_is_encrypted)
    {
        int32_t status = decrypt_sector_metadata(sector_nr, persistent_metadata, &cached_metadata);
        if (ARM_DRIVER_OK != status)
        {
            PLOG_ERROR("decrypt_sector_metadata failed: 0x%x", (uint32_t)status);
            return NULL;
        }
    }
    return metadata_cache_update_from_struct(&cached_metadata);
}

void metadata_cache_clear()
{
    for (size_t i = 0; i < sizeof(metadata_cache) / sizeof(metadata_cache[0]); i++)
    {
        metadata_cache[i].sector_nr = UINT32_MAX;
    }
}
static bool is_ahb_configured_for_sector_read(uint32_t sector_nr, bool use_iped)
{
    return ahb_configuration.sector_nr == sector_nr && ahb_configuration.use_iped == use_iped;
}

static void clear_ahb_buffer()
{
    __ISB();
    __DMB();
    *((volatile uint32_t*)0x18000000);
    *((volatile uint32_t*)0x18002000);
    __ISB();
    __DMB();
}

static int32_t setup_ahb_for_sector_read(uint32_t sector_nr, bool use_iped)
{
    int32_t status = ARM_DRIVER_OK;
    if (is_ahb_configured_for_sector_read(sector_nr, use_iped))
    {
        goto exit;
    }
    else
    {
        if (use_iped)
        {
            const cached_metadata_t *cached_metadata = find_cached_metadata(sector_nr);
            if (cached_metadata == NULL)
            {
                setup_ahb_for_sector_read(sector_nr, false);
                cached_metadata = metadata_cache_update_from_flash(sector_nr);
                if (cached_metadata == NULL) {
                    status = ARM_DRIVER_ERROR;                   \
                    goto exit;                                   \
                }
            }
            if (cached_metadata->sector_is_encrypted)
            {
                enable_iped(sector_nr, cached_metadata);
            }
            else
            {
                setup_ahb_for_sector_read(sector_nr, false);
            }
        }
        else
        {
            clear_ahb_buffer();
        }
    }
exit:
    ahb_configuration.sector_nr = sector_nr;
    ahb_configuration.use_iped  = use_iped;
    return status;
}

static void enable_iped(uint32_t sector_nr, const cached_metadata_t *cfg)
{
#if FLASH0_IPED_ENCRYPT_ENABLED
    iped_region_t region_nr = FLASH0_IPED_REGION_NR;

    uint32_t sector_start_addr_phy = get_sector_start_addr_phy(sector_nr);
    uint32_t sector_end_addr_phy   = get_sector_end_addr_phy(sector_nr);
    IPED_SetRegionAddressRange(EXAMPLE_FLEXSPI, region_nr, sector_start_addr_phy, sector_end_addr_phy);

    uint8_t aad[8] = {0};
    memcpy(aad, &sector_versions[sector_nr], sizeof(sector_versions[sector_nr]));

    IPED_SetRegionIV(EXAMPLE_FLEXSPI, region_nr, cfg->plain_iv);
    IPED_SetRegionAAD(EXAMPLE_FLEXSPI, region_nr, aad);

    IPED_SetRegionEnable(EXAMPLE_FLEXSPI, region_nr, true);

    TRACE_IPED_CONFIG("set parameters for region nr: 0x%x", region_nr);
    TRACE_IPED_CONFIG("    iv 0:  0x%x", get_iped_context_regs_r(region_nr)->IV0);
    TRACE_IPED_CONFIG("    iv 1:  0x%x", get_iped_context_regs_r(region_nr)->IV1);
    TRACE_IPED_CONFIG("    aad 0: 0x%x", get_iped_context_regs_r(region_nr)->AAD0);
    TRACE_IPED_CONFIG("    aad 1: 0x%x", get_iped_context_regs_r(region_nr)->AAD1);
    TRACE_IPED_CONFIG("    start: 0x%x", get_iped_context_regs_r(region_nr)->START);
    TRACE_IPED_CONFIG("    end:   0x%x", get_iped_context_regs_r(region_nr)->END);

    clear_cache();
#endif
}

static int32_t enable_iped_for_writing_new_sector(uint32_t sector_nr, const cached_metadata_t **cached_metadata)
{
    int32_t status = ARM_DRIVER_OK;

    cached_metadata_t metadata = {.sector_nr = sector_nr, .sector_is_encrypted = true};

    /* Get random IV for sector metadata encryption. */
    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(result, token,
                                     mcuxClCss_Rng_DrbgRequest_Async(metadata.plain_iv, sizeof(metadata.plain_iv)));
    ASSERT_OR_EXIT(
        MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClCss_Rng_DrbgRequest_Async) == token && MCUXCLCSS_STATUS_OK_WAIT == result,
        "mcuxClCss_Rng_DrbgRequest_Async failed: 0x%x", result);
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(result, token, mcuxClCss_WaitForOperation(MCUXCLCSS_ERROR_FLAGS_CLEAR));
    ASSERT_OR_EXIT(MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClCss_WaitForOperation) == token && MCUXCLCSS_STATUS_OK == result,
                   "mcuxClCss_WaitForOperation failed: 0x%x", result);
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    enable_iped(sector_nr, &metadata);
    *cached_metadata = metadata_cache_update_from_struct(&metadata);
exit:
    return status;
}

static int32_t get_valid_sector_version_range(uint32_t *min_version_nr, uint32_t *max_version_nr)
{
#if FLASH0_IPED_ROLLBACK_PROTECTION_ENABLED
    int32_t status                = ARM_DRIVER_OK;
    uint32_t rollback_counters[2] = {0};

    status = get_rollback_counter(ROLLBACK_COUNTER_0, &rollback_counters[0]);
    ASSERT_STATUS_OK_OR_EXIT("get_rollback_counter failed: ");

    status = get_rollback_counter(ROLLBACK_COUNTER_1, &rollback_counters[1]);
    ASSERT_STATUS_OK_OR_EXIT("get_rollback_counter failed: ");

    if (rollback_counters[0] == rollback_counters[1])
    {
        // Default case, the counters are aligned, the sector version must match the
        // rollback counter number.
        *min_version_nr = rollback_counters[0];
        *max_version_nr = rollback_counters[0];
    }
    else if (rollback_counters[0] > rollback_counters[1])
    {
        // An power-loss happened, the counters are out of sync. This can happen, in that case all in between the
        // rollback counters is valid versions. The counters will be aligned again on the next write.
        *min_version_nr = rollback_counters[1];
        *max_version_nr = rollback_counters[0];
    }
    else
    {
        PLOG_ERROR("rollback counter 0 (%d) < rollback counter 1 (%d)", rollback_counters[0], rollback_counters[1]);
        *min_version_nr = UINT32_MAX;
        *max_version_nr = UINT32_MAX;
        status          = ARM_DRIVER_ERROR;
        goto exit;
    }
    TRACE_ROLLBACK_PROTECTION("read rollback counters: ");
    TRACE_ROLLBACK_PROTECTION("    min_version_nr: %d", *min_version_nr);
    TRACE_ROLLBACK_PROTECTION("    max_version_nr: %d", *max_version_nr);
exit:
    return status;
#else
    *min_version_nr = 0;
    *max_version_nr = UINT32_MAX;
    TRACE_ROLLBACK_PROTECTION("rollback protection disabled.");
    return ARM_DRIVER_OK;
#endif
}

static int32_t get_most_recent_main_sector_nr(uint32_t *sector_nr)
{
    bool sector_valid[2]        = {0};
    uint32_t local_sector_versions[2] = {0};
    for (uint32_t i = 0; i < 2; i++)
    {
        uint32_t local_sector_nr                         = i;
        persistent_metadata_t *persistent_metadata = (persistent_metadata_t *)get_sector_metadata_addr_phy(local_sector_nr);

        sector_valid[i]    = is_valid_sector_metadata(persistent_metadata);
        local_sector_versions[i] = persistent_metadata->sector_versions[local_sector_nr];
    }

    if (sector_valid[0] && sector_valid[1])
    {
        if (local_sector_versions[1] > local_sector_versions[0])
        {
            *sector_nr = 1;
            TRACE_ROLLBACK_PROTECTION(
                "both sectors valid, sector_version[1] (%d) > sector_version[0] (%d), resulting sector_nr: %d",
                local_sector_versions[1], local_sector_versions[0], *sector_nr);
        }
        *sector_nr = 0;
        TRACE_ROLLBACK_PROTECTION("both sectors valid, resulting sector_nr: %d", *sector_nr);
    }
    else if (sector_valid[0])
    {
        *sector_nr = 0;
        TRACE_ROLLBACK_PROTECTION("only sector 0 valid, resulting sector_nr: %d", *sector_nr);
    }
    else if (sector_valid[1])
    {
        *sector_nr = 1;
        TRACE_ROLLBACK_PROTECTION("only sector 1 valid, resulting sector_nr: %d", *sector_nr);
    }
    else
    {
        return ARM_DRIVER_ERROR;
    }
    return ARM_DRIVER_OK;
}

static int32_t initialize_sector_versions()
{
    int32_t status                   = ARM_DRIVER_OK;
    uint32_t min_main_sector_version = 0;
    uint32_t max_main_sector_version = 0;
    uint32_t main_sector_nr          = 0;

    // In here, we only ever access plaintext (never IPED encrypted memory locations). This allows to rely on
    // AHB buffer contents being valid from this point and no further buffer clearing is done.
    clear_ahb_buffer();

    status = get_valid_sector_version_range(&min_main_sector_version, &max_main_sector_version);
    ASSERT_STATUS_OK_OR_EXIT("get_min_main_sector_version failed: ");

    status = get_most_recent_main_sector_nr(&main_sector_nr);
    if (status != ARM_DRIVER_OK)
    {
        // There is no valid main sector, this happens on empty/uninitialized file systems. To start somewhere, make all
        // the sector versions the same as the current rollback counter. There should no sector with valid metadata
        // exist.
        TRACE_ROLLBACK_PROTECTION(
            "no valid sector found, falling back to min rollback counter value 0 for all sector versions.");
        memset(sector_versions, min_main_sector_version, sizeof(sector_versions));
        status = ARM_DRIVER_OK;
        goto exit;
    }

    // Copy the list of sector versions to RAM for easier manipulation and lookup. Also, the authentication of the
    // sector is done already with the copy in RAM.
    persistent_metadata_t *persistent_metadata = (persistent_metadata_t *)get_sector_metadata_addr_phy(main_sector_nr);
    memcpy(sector_versions, persistent_metadata->sector_versions, sizeof(sector_versions));

#if FLASH0_IPED_ROLLBACK_PROTECTION_ENABLED
    // If the most recent sector's version is not at least as big as the min allowed version from the rollback
    // protection, then there is an issue.
    ASSERT_OR_EXIT(persistent_metadata->sector_versions[main_sector_nr] >= min_main_sector_version,
                   "sector version is too small for rollback counter");
    // The sector also cannot be "newer" than the rollback counter.
    ASSERT_OR_EXIT(persistent_metadata->sector_versions[main_sector_nr] <= max_main_sector_version,
                   "sector version is too big for rollback counter");
#endif

#if FLASH0_IPED_IV_ENCRYPT_ENABLED

    // decrypt and - more-importantly - authenticate the sector_versions
    cached_metadata_t dst = {0};
    status                = perform_crypto_operation(
        persistent_metadata->metadata_iv, sizeof(persistent_metadata->metadata_iv), (uint8_t *)&sector_versions[0],
        sizeof(sector_versions), persistent_metadata->sector_iv, dst.plain_iv, sizeof(persistent_metadata->sector_iv),
        (uint8_t *)persistent_metadata->tag, sizeof(persistent_metadata->tag), false);
    ASSERT_OR_EXIT(status == ARM_DRIVER_OK, "perform_crypto_operation failed: 0x%x", status);
#endif

exit:
    return status;
}

#if FLASH0_IPED_ROLLBACK_PROTECTION_ENABLED
static int32_t increment_sector_version(uint32_t sector_nr)
{
    int32_t status = ARM_DRIVER_OK;
    ASSERT_OR_EXIT(sector_versions[sector_nr] < UINT32_MAX, "Overflow for sector version (%d).", sector_versions[sector_nr]);
    sector_versions[sector_nr]++;
    return ARM_DRIVER_OK;
exit:
    return status;
}

static int32_t set_sector_version(uint32_t sector_nr, uint32_t sector_version)
{
    int32_t status = ARM_DRIVER_OK;
    ASSERT_OR_EXIT(sector_versions[sector_nr] < UINT32_MAX, "Overflow for sector version. (%d)", sector_versions[sector_nr]);
    sector_versions[sector_nr] = sector_version;
    return ARM_DRIVER_OK;
exit:
    return status;
}
#endif // FLASH0_IPED_ROLLBACK_PROTECTION_ENABLED

static int32_t pre_update_sector(uint32_t sector_nr)
{
#if FLASH0_IPED_ROLLBACK_PROTECTION_ENABLED
    int32_t  status = ARM_DRIVER_OK;
    if (sector_nr == 0 || sector_nr == 1)
    {
        status = increment_rollback_counter(ROLLBACK_COUNTER_0);
        ASSERT_STATUS_OK_OR_EXIT("increment_rollback_counter failed: ");
        uint32_t sector_version = 0;
        status                  = get_rollback_counter(ROLLBACK_COUNTER_0, &sector_version);
        ASSERT_STATUS_OK_OR_EXIT("get_rollback_counter failed: ");
        set_sector_version(sector_nr, sector_version);
        ASSERT_STATUS_OK_OR_EXIT("set_sector_version failed: ");
    }
    else
    {
        status = increment_sector_version(sector_nr);
        ASSERT_STATUS_OK_OR_EXIT("increment_sector_version failed: ");
    }
exit:
    return status;
#else
    return ARM_DRIVER_OK;
#endif
}

static int32_t post_update_sector(uint32_t sector_nr)
{
#if FLASH0_IPED_ROLLBACK_PROTECTION_ENABLED
    int32_t  status = ARM_DRIVER_OK;
    if (sector_nr == 0 || sector_nr == 1)
    {
        status = align_rollback_counters();
        ASSERT_STATUS_OK_OR_EXIT("align_rollback_counters failed: ");
    }
exit:
    return status;
#else
    return ARM_DRIVER_OK;
#endif
}

static void clear_cache()
{
    setup_ahb_for_sector_read(0, false);
    CACHE64_InvalidateCache(CACHE64_CTRL0);
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
    int32_t arm_status = Driver_FLASH0.Initialize(cb_event);
    if (arm_status != ARM_DRIVER_OK)
    {
        return arm_status;
    }
    metadata_cache_clear();
    setup_ahb_for_sector_read(0, false);
    IPED_EncryptEnable(EXAMPLE_FLEXSPI);
    int32_t status = initialize_sector_versions();
    return status;
}

static int32_t ARM_Flash_Uninitialize(void)
{
    int32_t arm_status = Driver_FLASH0.Uninitialize();
    if (arm_status != ARM_DRIVER_OK)
    {
        return arm_status;
    }
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
    if (! is_valid_addr_range(addr, cnt)) {
        PLOG_ERROR("invalid addr range: addr: 0x%x, cnt: 0x%x", addr, cnt);
        return ARM_DRIVER_ERROR_PARAMETER;
    }

    uint32_t remaining = cnt;
    uint8_t *dst_pos = data;
    while (remaining > 0)
    {
        uint32_t sector_nr             = get_sector_nr(addr);
        uint32_t sector_start_addr_phy = get_sector_start_addr_phy(sector_nr);
        uint32_t offset_in_sector      = get_offset_in_sector(addr);
        TRACE_READ("read: 0x%x", sector_start_addr_phy + offset_in_sector);

        uint32_t free_in_sector          = sector_size_log - offset_in_sector;
        uint32_t count_in_this_iteration = free_in_sector > remaining ? remaining : free_in_sector;

        int32_t status = setup_ahb_for_sector_read(sector_nr, true);

        if (status == ARM_DRIVER_OK)
        {
            memcpy(dst_pos, (uint8_t *)(sector_start_addr_phy + offset_in_sector), count_in_this_iteration);
        }
        else
        {
            memset(dst_pos, 0xCC, count_in_this_iteration);
        }

        remaining -= count_in_this_iteration;
        addr += count_in_this_iteration;
        dst_pos += count_in_this_iteration;
    }

    TRACE_DATA("read data", data, cnt);
    return ARM_DRIVER_OK;
}

static int32_t ARM_Flash_ProgramData(uint32_t addr, const void *data, uint32_t cnt)
{
    if (! is_valid_addr_range(addr, cnt)) {
        PLOG_ERROR("invalid addr range: addr: 0x%x, cnt: 0x%x", addr, cnt);
        return ARM_DRIVER_ERROR_PARAMETER;
    }

    // We may only write complete sectors!
    if ((cnt % sector_size_log != 0) || (!is_sector_start_addr(addr)))
    {
        return ARM_DRIVER_ERROR_PARAMETER;
    }

    status_t status = ARM_DRIVER_OK;

    uint32_t remaining_sector_count = cnt / sector_size_log;
    uint32_t sector_nr              = get_sector_nr(addr);
    const uint8_t *src_pos          = data;
    while (remaining_sector_count > 0)
    {
        const cached_metadata_t *cached_metadata = NULL;

        uint32_t sector_start_addr_phy = get_sector_start_addr_phy(sector_nr);
        TRACE_SECTOR_WRITE("program sector: 0x%x", sector_start_addr_phy);

        status = pre_update_sector(sector_nr);
        ASSERT_STATUS_OK_OR_EXIT("pre_update_sector failed: ");

        status = enable_iped_for_writing_new_sector(sector_nr, &cached_metadata);
        ASSERT_STATUS_OK_OR_EXIT("iap_mem_write failed: ");

        uint32_t dst_address     = sector_start_addr_phy;
        uint32_t remaining_pages = pages_per_sector;
        while (remaining_pages > 0)
        {
            TRACE_PAGE_WRITE("program page: 0x%x", dst_address);
            TRACE_DATA("    programmed data", src_pos, page_size);

            __disable_irq();
            clear_cache();
            status_t status = iap_mem_write(&g_context, dst_address, page_size, src_pos, kMemoryID_FlexspiNor);
            ASSERT_OR_EXIT_EN_IRQ(kStatus_Success == status, "iap_mem_write failed: 0x%x", status);
            clear_cache();
            __enable_irq();

            dst_address += page_size;
            src_pos += page_size;
            remaining_pages -= 1;
        }

#if FLASH0_IPED_METADATA_WRITE_ENABLED
        // We need to write the metadata last. If the metadata is ok, this means that the whole sector is OK.

        persistent_metadata_t persistent_metadata = {0};
        status = encrypt_sector_metadata(sector_nr, cached_metadata, &persistent_metadata);
        ASSERT_STATUS_OK_OR_EXIT("encrypt_sector_metadata failed");

        uint32_t metadata_address = get_sector_metadata_addr_phy(sector_nr);
        TRACE_PAGE_WRITE("program metadata: 0x%x", metadata_address);
        TRACE_DATA("    programmed metadata", (uint8_t *)&persistent_metadata, sizeof(persistent_metadata));

        __disable_irq();
        clear_cache();
        status = iap_mem_write(&g_context, metadata_address, sizeof(persistent_metadata), (uint8_t *)&persistent_metadata,
                               kMemoryID_FlexspiNor);
        ASSERT_OR_EXIT_EN_IRQ(kStatus_Success == status, "iap_mem_write failed: 0x%x", status);
        clear_cache();
        status = iap_mem_flush(&g_context);
        ASSERT_OR_EXIT_EN_IRQ(kStatus_Success == status, "iap_mem_flush failed: 0x%x", status);
        clear_cache();
        __enable_irq();

#endif
        status = post_update_sector(sector_nr);
        ASSERT_STATUS_OK_OR_EXIT("post_update_sector failed: ");

        remaining_sector_count -= 1;
        sector_nr += 1;
    }

exit:
    clear_cache();
    __enable_irq();
    return status;
}

static int32_t ARM_Flash_EraseSector(uint32_t addr)
{
    if (! is_valid_addr_range(addr, 0)) {
        PLOG_ERROR("invalid erase addr: 0x%x", addr);
        return ARM_DRIVER_ERROR_PARAMETER;
    }

    // We may only erase complete sectors!
    if (!is_sector_start_addr(addr))
    {
        PLOG_ERROR("unaligned erase addr: 0x%x", addr);
        return ARM_DRIVER_ERROR_PARAMETER;
    }

    uint32_t sector_nr             = get_sector_nr(addr);
    uint32_t sector_start_addr_phy = get_sector_start_addr_phy(sector_nr);

    TRACE_SECTOR_ERASE("erase: 0x%x", sector_start_addr_phy);
    __disable_irq();
    clear_cache();
    status_t status = iap_mem_erase(&g_context, sector_start_addr_phy, sector_size_log, kMemoryID_FlexspiNor);
    clear_cache();
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
    return &ARM_FLASH0_IPED_DEV_DATA;
}

ARM_DRIVER_FLASH Driver_FLASH0_IPED = {ARM_Flash_GetVersion,   ARM_Flash_GetCapabilities, ARM_Flash_Initialize,
                                       ARM_Flash_Uninitialize, ARM_Flash_PowerControl,    ARM_Flash_ReadData,
                                       ARM_Flash_ProgramData,  ARM_Flash_EraseSector,     ARM_Flash_EraseChip,
                                       ARM_Flash_GetStatus,    ARM_Flash_GetInfo};
