/*
 * Copyright (c) 2021-2023, Arm Limited. All rights reserved.
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "config_tfm.h"
#include "tfm_platform_api.h"
#include "service_api.h"
#include "tfm_veneers.h"
#include "psa/service.h"
#include "compiler_ext_defs.h"
#include "tfm_lds_req_mngr.h"
#include "tfm_lds_func_ids.h"
#include "tfm_lds_defs.h"

////////////////////////////////////////////////////////////////////////////
//! @brief tfm api tfm_loader_api_dispatcher
////////////////////////////////////////////////////////////////////////////
psa_status_t tfm_loader_api_dispatcher(psa_invec in_vec[],
                                       size_t in_len,
                                       psa_outvec out_vec[],
                                       size_t out_len)
{
    const struct tfm_loader_pack_iovec *iov = in_vec[0].base;
    LOAD_Target_Type target_id;
    uint32_t src_address;
    uint16_t function_id;
    uint32_t flag;

    if (in_vec[0].len != sizeof(struct tfm_loader_pack_iovec)) {
        return PSA_ERROR_PROGRAMMER_ERROR;
    }

    target_id = iov->target_id;
    src_address = iov->src_address;
    function_id = iov->function_id;
    flag        = iov->flag;
    
    /* Dispatch to each sub-module based on the Group ID */
    switch (function_id) {
    case TFM_LDS_SB3_FW_DOWNLOAD:
        return sb3_fw_download_impl(target_id, flag, src_address);
    case TFM_LDS_SB3_FW_RESET:
        return sb3_fw_reset_impl(target_id, flag, src_address);
    case TFM_LDS_PWR_ON_DEVICE:
        power_on_device_impl(target_id);
        return PSA_SUCCESS;
    case TFM_LDS_PWR_OFF_DEVICE:
        power_off_device_impl(target_id);
        return PSA_SUCCESS;
    default:
        return PSA_ERROR_NOT_SUPPORTED;
    }

    return PSA_ERROR_NOT_SUPPORTED;
}

////////////////////////////////////////////////////////////////////////////
//! @brief tfm api tfm_lds_call_srv
////////////////////////////////////////////////////////////////////////////
static psa_status_t tfm_lds_call_srv(const psa_msg_t *msg)
{
    psa_status_t status;
    size_t in_len = PSA_MAX_IOVEC, out_len = PSA_MAX_IOVEC, i;
    psa_invec in_vec[PSA_MAX_IOVEC] = { {NULL, 0} };
    psa_outvec out_vec[PSA_MAX_IOVEC] = { {NULL, 0} };
    struct tfm_loader_pack_iovec iov = {0};

    /* Check the number of in_vec filled */
    while ((in_len > 0) && (msg->in_size[in_len - 1] == 0)) {
        in_len--;
    }

    /* Check the number of out_vec filled */
    while ((out_len > 0) && (msg->out_size[out_len - 1] == 0)) {
        out_len--;
    }

    /* There will always be a tfm_loader_pack_iovec in the first iovec */
    if (in_len < 1) {
        return PSA_ERROR_GENERIC_ERROR;
    }

    if (psa_read(msg->handle, 0, &iov, sizeof(iov)) != sizeof(iov)) {
        return PSA_ERROR_GENERIC_ERROR;
    }

    /* Initialise the first iovec with the IOV read when parsing */
    in_vec[0].base = &iov;
    in_vec[0].len = sizeof(struct tfm_loader_pack_iovec);

    /* Call the dispatcher to the functions that implement the PSA Loader API */
    status = tfm_loader_api_dispatcher(in_vec, in_len, out_vec, out_len);

#if PSA_FRAMEWORK_HAS_MM_IOVEC == 1
    for (i = 0; i < out_len; i++) {
        if (out_vec[i].base != NULL) {
            psa_unmap_outvec(msg->handle, i, out_vec[i].len);
        }
    }
#else
    /* Write into the IPC framework outputs from the scratch */
    for (i = 0; i < out_len; i++) {
        psa_write(msg->handle, i, out_vec[i].base, out_vec[i].len);
    }
#endif

    return status;
}

////////////////////////////////////////////////////////////////////////////
//! @brief tfm api tfm_loader_service_sfn
////////////////////////////////////////////////////////////////////////////
psa_status_t tfm_loader_service_sfn(const psa_msg_t *msg)
{
    switch (msg->type) {
    case PSA_IPC_CALL:
        return tfm_lds_call_srv(msg);
    default:
        return PSA_ERROR_NOT_SUPPORTED;
    }

    return PSA_SUCCESS;
}

////////////////////////////////////////////////////////////////////////////
//! @brief tfm api tfm_lds_entry
////////////////////////////////////////////////////////////////////////////
psa_status_t tfm_lds_entry(void)
{
    return PSA_SUCCESS;
}
