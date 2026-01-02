/*
 * Copyright (c) 2019-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/***********  WARNING: This is an auto-generated file. Do not edit!  ***********/

#ifndef __PSA_MANIFEST_TFM_FPU_SERVICE_TEST_H__
#define __PSA_MANIFEST_TFM_FPU_SERVICE_TEST_H__

#ifdef __cplusplus
extern "C" {
#endif

#if CONFIG_TFM_SPM_BACKEND_IPC == 1
#define TFM_SP_FPU_SERVICE_TEST_MODEL_IPC                       1
#define TFM_SP_FPU_SERVICE_TEST_MODEL_SFN                       0

#define TFM_FPU_CHECK_FP_CALLEE_REGISTER_SIGNAL                 (0x10)
#define TFM_FPU_TEST_NS_PREEMPT_S_SIGNAL                        (0x20)
#else
#define TFM_SP_FPU_SERVICE_TEST_MODEL_IPC                       0
#define TFM_SP_FPU_SERVICE_TEST_MODEL_SFN                       1

psa_status_t tfm_fpu_check_fp_callee_register_sfn(const psa_msg_t* msg);
psa_status_t tfm_fpu_test_ns_preempt_s_sfn(const psa_msg_t* msg);
#endif




#ifdef __cplusplus
}
#endif

#endif /* __PSA_MANIFEST_TFM_FPU_SERVICE_TEST_H__ */
