/*
 * Copyright (c) 2019-2022, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/***********  WARNING: This is an auto-generated file. Do not edit!  ***********/

#ifndef __PSA_MANIFEST_TFM_FLIH_TEST_SERVICE_H__
#define __PSA_MANIFEST_TFM_FLIH_TEST_SERVICE_H__

#ifdef __cplusplus
extern "C" {
#endif

#if CONFIG_TFM_SPM_BACKEND_IPC == 1
#define TFM_SP_FLIH_TEST_MODEL_IPC                              1
#define TFM_SP_FLIH_TEST_MODEL_SFN                              0

#define TFM_FLIH_TEST_CASE_SIGNAL                               (0x10)

#else
#define TFM_SP_FLIH_TEST_MODEL_IPC                              0
#define TFM_SP_FLIH_TEST_MODEL_SFN                              1

psa_status_t tfm_flih_test_case_sfn(const psa_msg_t *msg);
#endif



#define TFM_TIMER0_IRQ_SIGNAL                                   (0x80000000)
psa_flih_result_t tfm_timer0_irq_flih(void);

#ifdef __cplusplus
}
#endif

#endif /* __PSA_MANIFEST_TFM_FLIH_TEST_SERVICE_H__ */
