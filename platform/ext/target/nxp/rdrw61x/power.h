/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __FRDMRW612_POWER_H__
#define __FRDMRW612_POWER_H__

#include "fsl_power.h"
#include "tfm_platform_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Extended IOCTL request IDs for RW61x power management */
enum tfm_platform_ioctl_rw61x_request_types_t {
    TFM_PLATFORM_IOCTL_RW61X_SET_PM  = (1 + 1),  /* CORE_LAST + 1 = 2 */
    TFM_PLATFORM_IOCTL_RW61X_PMU     = (1 + 2),  /* = 3 */
};

/* Input struct from NS to S for PM IOCTL */
struct rw61x_pm_ioctl_in {
    uint8_t pm_mode;              /* 1 -> PM1, 2 -> PM2, 3 -> PM3 */
    uint32_t idle_us;             /* optional: expected idle time (us), can be 0 */
    power_sleep_config_t slp_cfg;
};

/* Sub-commands for TFM_PLATFORM_IOCTL_RW61X_PMU */
enum nxp_pmu_cmd {
    NXP_PMU_CMD_ENABLE_WAKEUP,
    NXP_PMU_CMD_DISABLE_WAKEUP,
    NXP_PMU_CMD_GET_WAKEUP_STATUS,
    NXP_PMU_CMD_CLEAR_WAKEUP_STATUS,
    NXP_PMU_CMD_GET_POWER_MODE_STATUS,
    NXP_PMU_CMD_INIT_POWER_CONFIG,
    NXP_PMU_CMD_CLEAR_RESET_CAUSE,
    NXP_PMU_CMD_GET_WAKEUP_PINS,
    NXP_PMU_CMD_CONFIG_WAKEUP_PIN,
    NXP_PMU_CMD_ENABLE_XTAL32K,
};

/* Input/output structs for PMU IOCTL (must match NS side in pmu.h) */
struct nxp_pmu_ioctl_in {
    uint32_t cmd;                  /* enum nxp_pmu_cmd */
    uint32_t arg;                  /* IRQn, bitmask, or packed pin|level<<16 */
    power_init_config_t power_cfg; /* used by NXP_PMU_CMD_INIT_POWER_CONFIG */
};

struct nxp_pmu_ioctl_out {
    uint32_t result;               /* status/register value for GET operations */
};

/**
 * \brief Handle a PMU sub-command from the NS side.
 *
 * \param[in]  in   Input struct with command, argument, and optional config
 * \param[out] out  Output struct for return values (may be NULL)
 *
 * \return TFM_PLATFORM_ERR_SUCCESS on success.
 */
enum tfm_platform_err_t rw61x_pmu_ioctl_secure(
    const struct nxp_pmu_ioctl_in *in,
    struct nxp_pmu_ioctl_out *out);

/**
 * \brief Enter a power mode from secure side.
 *
 * Handles PM1 (WFI), PM2 (clock-switch sleep), and PM3 (deep sleep with
 * NVRAM resume).  For PM3 the function saves and restores all NS and S
 * state that is wiped by the power-down.
 *
 * \param[in] pm_mode   Power mode (1, 2, or 3)
 * \param[in] idle_us   Expected idle time in microseconds (informational)
 * \param[in] slp_cfg   Pointer to the sleep configuration
 *
 * \return TFM_PLATFORM_ERR_SUCCESS on success,
 *         TFM_PLATFORM_ERR_INVALID_PARAM for unknown pm_mode.
 */
enum tfm_platform_err_t rw61x_enter_pm_secure(uint8_t pm_mode,
                                               uint32_t idle_us,
                                               const power_sleep_config_t *slp_cfg);

#ifdef __cplusplus
}
#endif

#endif /* __FRDMRW612_POWER_H__ */
