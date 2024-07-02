/*
 * Copyright (c) 2022, Arm Limited. All rights reserved.
 * Copyright 2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __CONFIG_TFM_TARGET_H__
#define __CONFIG_TFM_TARGET_H__


/* Using of stored NV seed to provide entropy is disabled, when CRYPTO_HW_ACCELERATOR is defined.  */
#ifdef CRYPTO_HW_ACCELERATOR
#undef CRYPTO_NV_SEED
#define CRYPTO_NV_SEED                         0
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
#undef PS_MAX_ASSET_SIZE
#define PS_MAX_ASSET_SIZE    2048

/* The maximum number of assets to be stored in the Protected Storage area. */
#undef PS_NUM_ASSETS
#define PS_NUM_ASSETS        10

/* The maximum number of assets to be stored in the Internal Trusted Storage */
#undef ITS_NUM_ASSETS
#define ITS_NUM_ASSETS       10


#ifdef PLATFORM_NO_FLASH
/* Enable emulated RAM FS for platforms that don't have flash for Internal Trusted Storage partition */
#undef ITS_RAM_FS
#define ITS_RAM_FS           1

/* Enable emulated RAM FS for platforms that don't have flash for Protected Storage partition */
#undef PS_RAM_FS
#define PS_RAM_FS            1
#endif /* PLATFORM_NO_FLASH */

/* els_pkc lib requires this for rsa key generation. */
#if CRYPTO_STACK_SIZE < 0x2000
#undef CRYPTO_STACK_SIZE
#define CRYPTO_STACK_SIZE 0x2000
#endif

#endif /* __CONFIG_TFM_TARGET_H__ */
