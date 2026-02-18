/*
 * Copyright (c) 2019-2022, Arm Limited. All rights reserved.
 * Copyright 2023, 2025-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef CRYPTO_ACCELERATOR_CONF_H
#define CRYPTO_ACCELERATOR_CONF_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/****************************************************************/
/* Require built-in implementations based on PSA requirements */
/****************************************************************/


#ifdef CRYPTO_HW_ACCELERATOR
#include "platform_psa_hw_accel.h"
#endif


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CRYPTO_ACCELERATOR_CONF_H */