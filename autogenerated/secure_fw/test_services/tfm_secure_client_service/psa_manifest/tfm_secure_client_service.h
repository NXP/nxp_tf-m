/*
 * Copyright (c) 2019-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/***********  WARNING: This is an auto-generated file. Do not edit!  ***********/

#ifndef __PSA_MANIFEST_TFM_SECURE_CLIENT_SERVICE_H__
#define __PSA_MANIFEST_TFM_SECURE_CLIENT_SERVICE_H__

#ifdef __cplusplus
extern "C" {
#endif

#if CONFIG_TFM_SPM_BACKEND_IPC == 1
#define TFM_SP_SECURE_TEST_PARTITION_MODEL_IPC                  1
#define TFM_SP_SECURE_TEST_PARTITION_MODEL_SFN                  0

#define TFM_SECURE_CLIENT_SRV_DUMMY_SIGNAL                      (0x10)
#else
#define TFM_SP_SECURE_TEST_PARTITION_MODEL_IPC                  0
#define TFM_SP_SECURE_TEST_PARTITION_MODEL_SFN                  1

psa_status_t tfm_secure_client_srv_dummy_sfn(const psa_msg_t* msg);
#endif

#ifdef __cplusplus
}
#endif

#endif /* __PSA_MANIFEST_TFM_SECURE_CLIENT_SERVICE_H__ */
