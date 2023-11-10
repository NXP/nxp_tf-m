/*
 * Copyright (c) 2018-2022, Arm Limited. All rights reserved.
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TFM_LDS_DEFS_H__
#define __TFM_LDS_DEFS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "tfm_veneers.h"
/* include the public api of conn_fw loader service apis*/
#include "fsl_loader.h"

/**
 * \brief Structure used to pack non-pointer types in a call
 *
 */
struct tfm_loader_pack_iovec {
    LOAD_Target_Type target_id; /*!< Target among WiFi, ble, 15d4 or WiFi VDLL */
    uint32_t src_address;       /*!< Flash address of source data */
    uint32_t flag;              /*!< Flag is unused in conn_fw, may be useful for future  */
    uint16_t function_id;       /*!< Used to identify the function in the API dispatcher to the service backend*/
};

#ifdef __cplusplus
}
#endif

#endif /* __TFM_LDS_DEFS_H__ */
