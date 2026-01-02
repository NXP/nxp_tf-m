/*
 * Copyright (c) 2021-2024, Arm Limited. All rights reserved.
 * Copyright (c) 2021-2023 Cypress Semiconductor Corporation (an Infineon
 * company) or an affiliate of Cypress Semiconductor Corporation. All rights
 * reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/***********  WARNING: This is an auto-generated file. Do not edit!  ***********/

#include <stdint.h>
#include <stddef.h>
#include "config_tfm.h"
#include "region.h"
#include "region_defs.h"
#include "spm.h"
#include "load/interrupt_defs.h"
#include "load/partition_defs.h"
#include "load/service_defs.h"
#include "load/asset_defs.h"
#include "tfm_peripherals_def.h"
#include "psa_manifest/pid.h"
#include "psa_manifest/sid.h"
#include "psa_manifest/tfm_fpu_service_test.h"

#define TFM_SP_FPU_SERVICE_TEST_NDEPS                           (0)
#define TFM_SP_FPU_SERVICE_TEST_NSERVS                          (2)
#if TFM_ISOLATION_LEVEL == 3
#define TFM_SP_FPU_SERVICE_TEST_NASSETS                         (0 + 1)
#else
#define TFM_SP_FPU_SERVICE_TEST_NASSETS                         (0)
#endif
#define TFM_SP_FPU_SERVICE_TEST_NIRQS                           (0)

/* Memory region declaration */
#if TFM_ISOLATION_LEVEL == 3
REGION_DECLARE(Image$$, PT_TFM_SP_FPU_SERVICE_TEST_PRIVATE, _DATA_START$$Base);
REGION_DECLARE(Image$$, PT_TFM_SP_FPU_SERVICE_TEST_PRIVATE, _DATA_END$$Base);
#endif

#if CONFIG_TFM_SPM_BACKEND_IPC == 1
extern uint8_t tfm_sp_fpu_service_test_stack[];

/* Entrypoint function declaration */
extern void fpu_service_test_main(void);
#endif

/* Interrupt init functions */

/* partition load info type definition */
struct partition_tfm_sp_fpu_service_test_load_info_t {
    /* common length load data */
    struct partition_load_info_t    load_info;
    /* per-partition variable length load data */
    uintptr_t                       stack_addr;
    uintptr_t                       heap_addr;
    struct service_load_info_t      services[TFM_SP_FPU_SERVICE_TEST_NSERVS];
#if TFM_ISOLATION_LEVEL == 3
    struct asset_desc_t             assets[TFM_SP_FPU_SERVICE_TEST_NASSETS];
#else
#endif
} __attribute__((aligned(4)));

/* Partition load, deps, service load data. Put to a dedicated section. */
#if defined(__ICCARM__)
#pragma location = ".part_load_priority_02"
__root
#endif /* __ICCARM__ */
const struct partition_tfm_sp_fpu_service_test_load_info_t tfm_sp_fpu_service_test_load
    __attribute__((used, section(".part_load_priority_02"))) = {
    .load_info = {
        .psa_ff_ver                 = 0x0101 | PARTITION_INFO_MAGIC,
        .pid                        = TFM_SP_FPU_SERVICE_TEST,
        .flags                      = 0
#if CONFIG_TFM_SPM_BACKEND_IPC == 1
                                    | PARTITION_MODEL_IPC
#endif
                                    | PARTITION_PRI_NORMAL,
#if CONFIG_TFM_SPM_BACKEND_IPC == 1
        .entry                      = ENTRY_TO_POSITION(fpu_service_test_main),
        .stack_size                 = 0x0400,
#else
        .entry                      = ENTRY_TO_POSITION(0),
        .stack_size                 = 0,
#endif
        .heap_size                  = 0,
        .ndeps                      = TFM_SP_FPU_SERVICE_TEST_NDEPS,
        .nservices                  = TFM_SP_FPU_SERVICE_TEST_NSERVS,
        .nassets                    = TFM_SP_FPU_SERVICE_TEST_NASSETS,
        .nirqs                      = TFM_SP_FPU_SERVICE_TEST_NIRQS,
    },
    .stack_addr                     = (uintptr_t)tfm_sp_fpu_service_test_stack,
    .heap_addr                      = 0,
    .services = {
        {
            .name_strid             = STRING_PTR_TO_STRID("TFM_FPU_CHECK_FP_CALLEE_REGISTER"),
#if CONFIG_TFM_SPM_BACKEND_IPC == 1
            .sfn                    = 0,
            .signal                 = TFM_FPU_CHECK_FP_CALLEE_REGISTER_SIGNAL,
#else
            .sfn                    = ENTRY_TO_POSITION(tfm_fpu_check_fp_callee_register_sfn),,
#endif
            .sid                    = 0x0000F091,
            .flags                  = 0
                                    | SERVICE_FLAG_NS_ACCESSIBLE
                                    | SERVICE_VERSION_POLICY_STRICT,
            .version                = 1,
        },
        {
            .name_strid             = STRING_PTR_TO_STRID("TFM_FPU_TEST_NS_PREEMPT_S"),
#if CONFIG_TFM_SPM_BACKEND_IPC == 1
            .sfn                    = 0,
            .signal                 = TFM_FPU_TEST_NS_PREEMPT_S_SIGNAL,
#else
            .sfn                    = ENTRY_TO_POSITION(tfm_fpu_test_ns_preempt_s_sfn),,
#endif
            .sid                    = 0x0000F092,
            .flags                  = 0
                                    | SERVICE_FLAG_NS_ACCESSIBLE
                                    | SERVICE_VERSION_POLICY_STRICT,
            .version                = 1,
        },
    },
#if TFM_ISOLATION_LEVEL == 3
    .assets                         = {
        {
            .mem.start              = (uintptr_t)&REGION_NAME(Image$$, PT_TFM_SP_FPU_SERVICE_TEST_PRIVATE, _DATA_START$$Base),
            .mem.limit              = (uintptr_t)&REGION_NAME(Image$$, PT_TFM_SP_FPU_SERVICE_TEST_PRIVATE, _DATA_END$$Base),
            .attr                   = ASSET_ATTR_READ_WRITE,
        },
    },
#else
#endif
};

/* Placeholder for partition and service runtime space. Do not reference it. */
#if defined(__ICCARM__)
#pragma location=".bss.part_runtime_priority_02"
__root
#endif /* __ICCARM__ */
static struct partition_t tfm_sp_fpu_service_test_partition_runtime_item
    __attribute__((used, section(".bss.part_runtime_priority_02")));
#if defined(__ICCARM__)
#pragma location = ".bss.serv_runtime_priority_02"
__root
#endif /* __ICCARM__ */
static struct service_t tfm_sp_fpu_service_test_service_runtime_item[TFM_SP_FPU_SERVICE_TEST_NSERVS]
    __attribute__((used, section(".bss.serv_runtime_priority_02")));
