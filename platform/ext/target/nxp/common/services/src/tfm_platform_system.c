/*
 * Copyright (c) 2018-2020, Arm Limited. All rights reserved.
 * Copyright 2025-2026 NXP
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "tfm_platform_system.h"
#include "platform_description.h"
#include "device_definition.h"

#if defined(TFM_RW61X_PM_SUPPORT)
#include "power.h"
#endif

void tfm_platform_hal_system_reset(void)
{
    /* Reset the system */
    NVIC_SystemReset();
}

enum tfm_platform_err_t tfm_platform_hal_ioctl(tfm_platform_ioctl_req_t request,
                                               psa_invec  *in_vec,
                                               psa_outvec *out_vec)
{
#if !defined(TFM_RW61X_PM_SUPPORT)
    (void)in_vec;
    (void)out_vec;
#endif

    switch (request) {
#if defined(TFM_RW61X_PM_SUPPORT)
    case TFM_PLATFORM_IOCTL_RW61X_SET_PM:
        if (!in_vec || (in_vec->len < sizeof(struct rw61x_pm_ioctl_in))) {
            return TFM_PLATFORM_ERR_INVALID_PARAM;
        }
        const struct rw61x_pm_ioctl_in *in = (const struct rw61x_pm_ioctl_in *)in_vec->base;
        return rw61x_enter_pm_secure(in->pm_mode, in->idle_us, &in->slp_cfg);

    case TFM_PLATFORM_IOCTL_RW61X_PMU:
        if (!in_vec || (in_vec->len < sizeof(struct nxp_pmu_ioctl_in))) {
            return TFM_PLATFORM_ERR_INVALID_PARAM;
        }
        return rw61x_pmu_ioctl_secure(
            (const struct nxp_pmu_ioctl_in *)in_vec->base,
            (out_vec && out_vec->len >= sizeof(struct nxp_pmu_ioctl_out))
                ? (struct nxp_pmu_ioctl_out *)out_vec->base : NULL);
#endif /* TFM_RW61X_PM_SUPPORT */

    default:
        return TFM_PLATFORM_ERR_NOT_SUPPORTED;
    }
}
