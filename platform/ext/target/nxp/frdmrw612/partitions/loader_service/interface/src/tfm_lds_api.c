/*
 * Copyright (c) 2018-2022, Arm Limited. All rights reserved.
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "tfm_lds_defs.h"
#include "psa/client.h"
#include "psa_manifest/sid.h"
//#include "tfm_lds_sid.h"
#include "tfm_lds_func_ids.h"

#define API_DISPATCH(in_vec, out_vec)                  \
    psa_call(TFM_LOADER_SERVICE_HANDLE, PSA_IPC_CALL,  \
             in_vec, IOVEC_LEN(in_vec),                \
             out_vec, IOVEC_LEN(out_vec))
#define API_DISPATCH_NO_OUTVEC(in_vec)                 \
    psa_call(TFM_LOADER_SERVICE_HANDLE, PSA_IPC_CALL,  \
             in_vec, IOVEC_LEN(in_vec),                \
             (psa_outvec *)NULL, 0)

#define TFM_LOADER_SERVICE_API(ret, fun) ret fun

TFM_LOADER_SERVICE_API(void,power_on_device)(LOAD_Target_Type loadTarget)
{
    struct tfm_loader_pack_iovec iov = {
        .target_id = loadTarget,
        .function_id = TFM_LDS_PWR_ON_DEVICE,
        .flag = 0,
        .src_address = 0,
    };
    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_loader_pack_iovec)},
    };

    API_DISPATCH_NO_OUTVEC(in_vec);
}

TFM_LOADER_SERVICE_API(void,power_off_device)(LOAD_Target_Type loadTarget)
{
    struct tfm_loader_pack_iovec iov = {
        .target_id = loadTarget,
        .function_id = TFM_LDS_PWR_OFF_DEVICE,
        .flag = 0,
        .src_address = 0,
    };
    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_loader_pack_iovec)},
    };

    API_DISPATCH_NO_OUTVEC(in_vec);
}

TFM_LOADER_SERVICE_API(psa_status_t,sb3_fw_download)(LOAD_Target_Type loadTarget, uint32_t flag, uint32_t sourceAddr)
{
    struct tfm_loader_pack_iovec iov = {
        .target_id = loadTarget,
        .function_id = TFM_LDS_SB3_FW_DOWNLOAD,
        .flag = flag,
        .src_address = sourceAddr,
    };
    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_loader_pack_iovec)},
    };

    return API_DISPATCH_NO_OUTVEC(in_vec);
}

TFM_LOADER_SERVICE_API(psa_status_t,sb3_fw_reset)(LOAD_Target_Type loadTarget, uint32_t flag, uint32_t sourceAddr)
{
    struct tfm_loader_pack_iovec iov = {
        .target_id = loadTarget,
        .function_id = TFM_LDS_SB3_FW_RESET,
        .flag = flag,
        .src_address = sourceAddr,
    };
    psa_invec in_vec[] = {
        {.base = &iov, .len = sizeof(struct tfm_loader_pack_iovec)},
    };

    return API_DISPATCH_NO_OUTVEC(in_vec);
}



