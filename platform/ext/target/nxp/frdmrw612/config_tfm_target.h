/*
 * Copyright (c) 2022, Arm Limited. All rights reserved.
 * Copyright 2024 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __CONFIG_TFM_TARGET_H__
#define __CONFIG_TFM_TARGET_H__

/* Using of stored NV seed to provide entropy is disabled, when CRYPTO_HW_ACCELERATOR is defined.  */
#ifdef CRYPTO_HW_ACCELERATOR
#define CRYPTO_NV_SEED       0
#endif

/* Heap size for the crypto backend */
#undef CRYPTO_ENGINE_BUF_SIZE
#define CRYPTO_ENGINE_BUF_SIZE                 0x4000

/* Default size of the internal scratch buffer used for PSA FF IOVec allocations */
#undef CRYPTO_IOVEC_BUFFER_SIZE
#define CRYPTO_IOVEC_BUFFER_SIZE               33000

/* The maximum asset size to be stored in the Internal Trusted Storage */
#undef ITS_MAX_ASSET_SIZE
#define ITS_MAX_ASSET_SIZE                     0xB80

/* The maximum asset size to be stored in the Protected Storage area. */
#define PS_MAX_ASSET_SIZE    2048

/* The maximum number of assets to be stored in the Protected Storage area. */
#define PS_NUM_ASSETS        10

/* The maximum number of assets to be stored in the Internal Trusted Storage */
#define ITS_NUM_ASSETS       10


#ifdef PLATFORM_NO_FLASH
/* Enable emulated RAM FS for platforms that don't have flash for Internal Trusted Storage partition */
#define ITS_RAM_FS           1

/* Enable emulated RAM FS for platforms that don't have flash for Protected Storage partition */
#define PS_RAM_FS            1

/* Enable OTP/NV_COUNTERS emulation in RAM */
#define OTP_NV_COUNTERS_RAM_EMULATION 1

#endif /* PLATFORM_NO_FLASH */

/* Enable TFM ITS tests in Regression suite */
#define TFM_INTERNAL_TRUSTED_STORAGE_SERVICE 1

#endif /* __CONFIG_TFM_TARGET_H__ */
