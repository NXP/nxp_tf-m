/*
 * Copyright (c) 2022, Arm Limited. All rights reserved.
 * Copyright 2022, 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __CONFIG_TFM_TARGET_H__
#define __CONFIG_TFM_TARGET_H__

/* Using of stored NV seed to provide entropy is disabled, when CRYPTO_HW_ACCELERATOR is defined.  */
#ifdef CRYPTO_HW_ACCELERATOR
#define CRYPTO_NV_SEED       0
#define CRYPTO_EXT_RNG       1
#endif

/* Enable PSA Crypto Cipher module */
#define CRYPTO_CIPHER_MODULE_ENABLED           1

/* Default size of the internal scratch buffer used for PSA FF IOVec allocations */
#undef CRYPTO_IOVEC_BUFFER_SIZE
#define CRYPTO_IOVEC_BUFFER_SIZE               0x800

/* The maximum asset size to be stored in the Protected Storage area. */
#define PS_MAX_ASSET_SIZE    512

/* The maximum number of assets to be stored in the Protected Storage area. */
#define PS_NUM_ASSETS        5

/* The maximum number of assets to be stored in the Internal Trusted Storage */
#define ITS_NUM_ASSETS       5


#ifdef PLATFORM_NO_FLASH
/* Enable emulated RAM FS for platforms that don't have flash for Internal Trusted Storage partition */
#define ITS_RAM_FS           1

/* Enable emulated RAM FS for platforms that don't have flash for Protected Storage partition */
#define PS_RAM_FS            1

/* Enable OTP/NV_COUNTERS emulation in RAM */
#define OTP_NV_COUNTERS_RAM_EMULATION 1

#endif /* PLATFORM_NO_FLASH */

#endif /* __CONFIG_TFM_TARGET_H__ */
