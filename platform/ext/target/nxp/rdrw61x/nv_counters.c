/*
 * Copyright (c) 2018-2022, Arm Limited. All rights reserved.
 * Copyright 2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/* NOTE: For the security of the protected storage system, the bootloader
 * rollback protection, and the protection of cryptographic material  it is
 * CRITICAL to use a internal (in-die) persistent memory for the implementation
 * of the OTP_NV_COUNTERS flash area (see flash_otp_nv_layout.c).
 */

#include "tfm_plat_nv_counters.h"
#include "fsl_romapi_otp.h"

#define OCOTP_NV_COUNTERS_RAM_EMULATION


#include <string.h>

#ifdef OCOTP_NV_COUNTERS_RAM_EMULATION

// The number of fuses used for a single PS counter.
#define OTP_NV_COUNTER_PS_FUSE_COUNT   (32u)

// The number of fuses used for a single ITS counter.
#define OTP_NV_COUNTER_ITS_FUSE_COUNT   (16u)

// If we use RAM emulation for the OTP fuses, we also can define to have more fuses available than actual OCOTP. So here
// we make the number of fuses depending on the definition of the counters above. Howver in reality this is vice-versa.
// HW offers a given nr of fuses which can be distributed among counters.
#define OTP_NV_COUNTER_FUSES_START_INDEX (0u)
#define OTP_NV_COUNTER_FUSES_COUNT       (2u * OTP_NV_COUNTER_ITS_FUSE_COUNT + 3u * OTP_NV_COUNTER_PS_FUSE_COUNT)

#else // #ifdef OCOTP_NV_COUNTERS_RAM_EMULATION

// This is a possible real fuse usage in OCOTP (not emulated).
#define OTP_NV_COUNTER_FUSES_START_INDEX (404u)
#define OTP_NV_COUNTER_FUSES_COUNT       (16u)

// The number of fuses used for a single PS counter. 3 * 4 = 12 in total are used for PS.
#define OTP_NV_COUNTER_PS_FUSE_COUNT   (4u)

// The number of fuses used for a single ITS counter. 2 * 2 = 4 in total are used for ITS.
#define OTP_NV_COUNTER_ITS_FUSE_COUNT   (2u)


#endif // #ifdef OCOTP_NV_COUNTERS_RAM_EMULATION


#define OTP_NV_COUNTER_PS_0_FUSE_START (OTP_NV_COUNTER_FUSES_START_INDEX)
#define OTP_NV_COUNTER_PS_0_FUSE_COUNT OTP_NV_COUNTER_PS_FUSE_COUNT
#define OTP_NV_COUNTER_PS_1_FUSE_START (OTP_NV_COUNTER_PS_0_FUSE_START + OTP_NV_COUNTER_PS_0_FUSE_COUNT)
#define OTP_NV_COUNTER_PS_1_FUSE_COUNT OTP_NV_COUNTER_PS_FUSE_COUNT
#define OTP_NV_COUNTER_PS_2_FUSE_START (OTP_NV_COUNTER_PS_1_FUSE_START + OTP_NV_COUNTER_PS_1_FUSE_COUNT)
#define OTP_NV_COUNTER_PS_2_FUSE_COUNT OTP_NV_COUNTER_PS_FUSE_COUNT

#define OTP_NV_COUNTER_ITS_0_FUSE_START (OTP_NV_COUNTER_PS_2_FUSE_START + OTP_NV_COUNTER_PS_2_FUSE_COUNT)
#define OTP_NV_COUNTER_ITS_0_FUSE_COUNT OTP_NV_COUNTER_ITS_FUSE_COUNT
#define OTP_NV_COUNTER_ITS_1_FUSE_START (OTP_NV_COUNTER_ITS_0_FUSE_START + OTP_NV_COUNTER_ITS_0_FUSE_COUNT)
#define OTP_NV_COUNTER_ITS_1_FUSE_COUNT OTP_NV_COUNTER_ITS_FUSE_COUNT

#define OTP_COUNTER_MAX_SIZE 128u
#define NV_COUNTER_SIZE      4

enum tfm_plat_err_t tfm_plat_init_nv_counter(void)
{
    return TFM_PLAT_ERR_SUCCESS;
}

#ifdef OCOTP_NV_COUNTERS_RAM_EMULATION

uint32_t emulated_fuses[OTP_NV_COUNTER_FUSES_COUNT] = {0};

status_t read_fuse(uint32_t fuse_idx, uint32_t *fuse_value)
{
    uint32_t offset = fuse_idx - OTP_NV_COUNTER_FUSES_START_INDEX;
    *fuse_value     = emulated_fuses[offset];
    return kStatus_Success;
}

status_t write_fuse(uint32_t fuse_idx, uint32_t fuse_value)
{
    uint32_t offset = fuse_idx - OTP_NV_COUNTER_FUSES_START_INDEX;
    emulated_fuses[offset] |= fuse_value;
    return kStatus_Success;
}

#else

#error THIS WILL ENABLE OTP WRITES!!!!

status_t read_fuse(uint32_t fuse_idx, uint32_t *fuse_value)
{
    return otp_fuse_read(fuse_idx, fuse_value);
}

status_t write_fuse(uint32_t fuse_idx, uint32_t fuse_value)
{
    return otp_fuse_program(fuse_idx, fuse_value);
}

#endif // #ifdef OCOTP_NV_COUNTERS_RAM_EMULATION

static enum tfm_plat_err_t count_set_fuse_bits(uint32_t start_fuse, uint32_t fuse_count, uint32_t *val)
{
    uint32_t end_fuse = start_fuse + fuse_count;

#ifndef OCOTP_NV_COUNTERS_RAM_EMULATION
    status_t status = otp_init(DEFAULT_SYSTEM_CLOCK);
    if (kStatus_Success != status)
    {
        return TFM_PLAT_ERR_SYSTEM_ERR;
    }
#endif

    *val = 0;
    for (uint32_t fuse_idx = start_fuse; fuse_idx < end_fuse; fuse_idx++)
    {
        uint32_t fuse_value = 0;
        status_t status     = read_fuse(fuse_idx, &fuse_value);
        if (kStatus_Success != status)
        {
            return TFM_PLAT_ERR_SYSTEM_ERR;
        }

        for (uint32_t bit_idx = 0; bit_idx < 16; bit_idx++)
        {
            if ((fuse_value & (1 << bit_idx)) == 0)
            {
                return TFM_PLAT_ERR_SUCCESS;
            }
            *val += 1;
        }
    }
    return TFM_PLAT_ERR_SUCCESS;
}

static enum tfm_plat_err_t get_fuses_for_nv_counter(uint32_t id, uint32_t* start_fuse, uint32_t *fuse_count)
{
    assert(start_fuse != NULL);
    assert(fuse_count != NULL);

    switch (id)
    {
        case PLAT_NV_COUNTER_PS_0:
            *start_fuse = OTP_NV_COUNTER_PS_0_FUSE_START;
            *fuse_count = OTP_NV_COUNTER_PS_0_FUSE_COUNT;
            break;
        case PLAT_NV_COUNTER_PS_1:
            *start_fuse = OTP_NV_COUNTER_PS_1_FUSE_START;
            *fuse_count = OTP_NV_COUNTER_PS_1_FUSE_COUNT;
            break;
        case PLAT_NV_COUNTER_PS_2:
            *start_fuse = OTP_NV_COUNTER_PS_2_FUSE_START;
            *fuse_count = OTP_NV_COUNTER_PS_2_FUSE_COUNT;
            break;
#if defined(TFM_PARTITION_INTERNAL_TRUSTED_STORAGE) && !defined(PLATFORM_DEFAULT_NV_COUNTERS)
        case PLAT_NV_COUNTER_ITS_0:
            *start_fuse = OTP_NV_COUNTER_ITS_0_FUSE_START;
            *fuse_count = OTP_NV_COUNTER_ITS_0_FUSE_COUNT;
            break;
        case PLAT_NV_COUNTER_ITS_1:
            *start_fuse = OTP_NV_COUNTER_ITS_1_FUSE_START;
            *fuse_count = OTP_NV_COUNTER_ITS_1_FUSE_COUNT;
            break;
#endif
        default:
            return TFM_PLAT_ERR_INVALID_INPUT;
    }
    return TFM_PLAT_ERR_SUCCESS;
}

static enum tfm_plat_err_t read_nv_counter_otp(uint32_t id, uint32_t size, uint8_t *val)
{
    assert(size >= sizeof(uint32_t));
    assert(val);

    uint32_t start_fuse = 0;
    uint32_t fuse_count = 0;
    enum tfm_plat_err_t err = get_fuses_for_nv_counter(id, &start_fuse, &fuse_count);
    if (err != TFM_PLAT_ERR_SUCCESS)
    {
        return err;
    }

    err = count_set_fuse_bits(start_fuse, fuse_count, (uint32_t *)val);
    if (err != TFM_PLAT_ERR_SUCCESS)
    {
        return err;
    }

    return TFM_PLAT_ERR_SUCCESS;
}

enum tfm_plat_err_t tfm_plat_read_nv_counter(enum tfm_nv_counter_t counter_id, uint32_t size, uint8_t *val)
{
    if (size != NV_COUNTER_SIZE)
    {
        return TFM_PLAT_ERR_SYSTEM_ERR;
    }

    switch ((uint32_t) counter_id)
    {
#ifdef TFM_PARTITION_PROTECTED_STORAGE
        case (PLAT_NV_COUNTER_PS_0):
            return read_nv_counter_otp(PLAT_NV_COUNTER_PS_0, size, val);
        case (PLAT_NV_COUNTER_PS_1):
            return read_nv_counter_otp(PLAT_NV_COUNTER_PS_1, size, val);
        case (PLAT_NV_COUNTER_PS_2):
            return read_nv_counter_otp(PLAT_NV_COUNTER_PS_2, size, val);
#endif /* TFM_PARTITION_PROTECTED_STORAGE */

#ifdef BL2
#error unsupported
#endif /* BL2 */

#ifdef BL1
#error unsupported
#endif /* BL1 */

#if (PLATFORM_NS_NV_COUNTERS > 0)
#error unsupported
#endif
#if (PLATFORM_NS_NV_COUNTERS > 1)
#error unsupported
#endif
#if (PLATFORM_NS_NV_COUNTERS > 2)
#error unsupported
#endif
#if defined(TFM_PARTITION_INTERNAL_TRUSTED_STORAGE) && !defined(PLATFORM_DEFAULT_NV_COUNTERS)
        case (PLAT_NV_COUNTER_ITS_0):
            return read_nv_counter_otp(PLAT_NV_COUNTER_ITS_0, size, val);
        case (PLAT_NV_COUNTER_ITS_1):
            return read_nv_counter_otp(PLAT_NV_COUNTER_ITS_1, size, val);
#endif
        default:
            return TFM_PLAT_ERR_UNSUPPORTED;
    }
}

static enum tfm_plat_err_t set_counter_fuse_bits(uint32_t start_fuse, uint32_t fuse_count, uint32_t val)
{
    // Per OCOTP fuse 16 bits are usable.
    if (val > (fuse_count * 16))
    {
        // Not enough bits in the fuses to store the value, this is invalid input.
        return TFM_PLAT_ERR_INVALID_INPUT;
    }

#ifndef OCOTP_NV_COUNTERS_RAM_EMULATION
    status_t status = otp_init(DEFAULT_SYSTEM_CLOCK);
    if (kStatus_Success != status)
    {
        return TFM_PLAT_ERR_SYSTEM_ERR;
    }
#endif

    uint32_t end_fuse        = start_fuse + fuse_count;
    uint32_t overall_bit_idx = 0;
    for (uint32_t fuse_idx = start_fuse; fuse_idx < end_fuse; fuse_idx++)
    {
        uint32_t fuse_value_to_write = 0;
        for (uint32_t bit_idx = 0; (bit_idx < 16) && (val > overall_bit_idx); bit_idx++)
        {
            fuse_value_to_write |= (1 << bit_idx);
            overall_bit_idx++;
        }

        uint32_t read_fuse_value = 0;
        status_t status          = read_fuse(fuse_idx, &read_fuse_value);
        if (kStatus_Success != status)
        {
            return TFM_PLAT_ERR_SYSTEM_ERR;
        }

        if (read_fuse_value != fuse_value_to_write)
        {
            status = write_fuse(fuse_idx, fuse_value_to_write);
            if (kStatus_Success != status)
            {
                 return TFM_PLAT_ERR_SYSTEM_ERR;
            }
        }
    }
    return TFM_PLAT_ERR_SUCCESS;
}

static enum tfm_plat_err_t set_nv_counter_otp(uint32_t id, uint32_t value)
{
    uint32_t original_value = 0;
    enum tfm_plat_err_t err = read_nv_counter_otp(id, sizeof(original_value), (uint8_t *)&original_value);
    if (err != TFM_PLAT_ERR_SUCCESS)
    {
        return err;
    }

    if (original_value > value)
    {
        return TFM_PLAT_ERR_INVALID_INPUT;
    }
    if (original_value == value)
    {
        return TFM_PLAT_ERR_SUCCESS;
    }

    uint32_t start_fuse = 0;
    uint32_t fuse_count = 0;
    err = get_fuses_for_nv_counter(id, &start_fuse, &fuse_count);
    if (err != TFM_PLAT_ERR_SUCCESS)
    {
        return err;
    }

    status_t status = set_counter_fuse_bits(start_fuse, fuse_count, value);
    if (kStatus_Success != status)
    {
        return TFM_PLAT_ERR_SYSTEM_ERR;
    }

    return TFM_PLAT_ERR_SUCCESS;
}

enum tfm_plat_err_t tfm_plat_set_nv_counter(enum tfm_nv_counter_t counter_id, uint32_t value)
{
    uint32_t new_value;
    enum tfm_plat_err_t err;

    switch ((uint32_t) counter_id)
    {
#ifdef TFM_PARTITION_PROTECTED_STORAGE
        case (PLAT_NV_COUNTER_PS_0):
            err = set_nv_counter_otp(PLAT_NV_COUNTER_PS_0, value);
            break;
        case (PLAT_NV_COUNTER_PS_1):
            err = set_nv_counter_otp(PLAT_NV_COUNTER_PS_1, value);
            break;
        case (PLAT_NV_COUNTER_PS_2):
            err = set_nv_counter_otp(PLAT_NV_COUNTER_PS_2, value);
            break;
#endif /* TFM_PARTITION_PROTECTED_STORAGE */

#ifdef BL2
#error unsupported
#endif /* BL2 */

#ifdef BL1
#error unsupported
#endif /* BL1 */

#if (PLATFORM_NS_NV_COUNTERS > 0)
#error unsupported
#endif
#if (PLATFORM_NS_NV_COUNTERS > 1)
#error unsupported
#endif
#if (PLATFORM_NS_NV_COUNTERS > 2)
#error unsupported
#endif
#if defined(TFM_PARTITION_INTERNAL_TRUSTED_STORAGE) && !defined(PLATFORM_DEFAULT_NV_COUNTERS)
        case (PLAT_NV_COUNTER_ITS_0):
            return set_nv_counter_otp(PLAT_NV_COUNTER_ITS_0, value);
        case (PLAT_NV_COUNTER_ITS_1):
            return set_nv_counter_otp(PLAT_NV_COUNTER_ITS_1, value);
#endif
        default:
            return TFM_PLAT_ERR_UNSUPPORTED;
    }
    if (err != TFM_PLAT_ERR_SUCCESS)
    {
        return err;
    }

    /* Check that the NV counter write hasn't failed (in case the driver doesn't
     * have a check).
     */
    err = tfm_plat_read_nv_counter(counter_id, sizeof(new_value), (uint8_t *)&new_value);
    if (err != TFM_PLAT_ERR_SUCCESS)
    {
        return err;
    }

    if (new_value != value)
    {
        return TFM_PLAT_ERR_SYSTEM_ERR;
    }

    return TFM_PLAT_ERR_SUCCESS;
}

enum tfm_plat_err_t tfm_plat_increment_nv_counter(enum tfm_nv_counter_t counter_id)
{
    uint32_t security_cnt;
    enum tfm_plat_err_t err;

    err = tfm_plat_read_nv_counter(counter_id, sizeof(security_cnt), (uint8_t *)&security_cnt);
    if (err != TFM_PLAT_ERR_SUCCESS)
    {
        return err;
    }

    if (security_cnt == UINT32_MAX)
    {
        return TFM_PLAT_ERR_MAX_VALUE;
    }

    return tfm_plat_set_nv_counter(counter_id, security_cnt + 1u);
}
