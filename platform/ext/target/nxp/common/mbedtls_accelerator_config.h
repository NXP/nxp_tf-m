/*
 * SPDX-FileCopyrightText: Copyright The TrustedFirmware-M Contributors
 * Copyright 2026 NXP.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef MBEDTLS_ACCELERATOR_CONFIG_H
#define MBEDTLS_ACCELERATOR_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef CRYPTO_HW_ACCELERATOR

#if defined CONFIG_PSA_CRYPTO_DRIVER_SGI_PKC
#define PSA_CRYPTO_DRIVER_PKC
#define PSA_CRYPTO_DRIVER_SGI
#define MBEDTLS_ENTROPY_HARDWARE_ALT
#endif /*CONFIG_PSA_CRYPTO_DRIVER_SGI_PKC*/
#endif /*CRYPTO_HW_ACCELERATOR*/

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MBEDTLS_ACCELERATOR_CONFIG_H */