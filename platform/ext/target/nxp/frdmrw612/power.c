/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * RW612 PM1/PM2/PM3 power management — secure side implementation.
 *
 * NS Zephyr calls tfm_platform_ioctl(TFM_PLATFORM_IOCTL_RW61X_SET_PM)
 * which routes here.  The secure side performs the actual WFI / clock
 * switch / deep-sleep entry.
 *
 * PM3: POWER_EnterPowerMode(3) saves a resume point to NVRAM (see
 * POWER_EnterPm3Asm in fsl_power.c).  On wakeup the ROM jumps back
 * and the function returns true.  The secure side must then restore
 * everything PM3 wiped:
 *
 *   - Clocks, pin mux, debug console, OSTIMER clock source
 *   - AIRCR
 *   - Secure MPU (TFM partition isolation regions)
 *   - TrustZone security (SAU/IDAU, MPC, PPC, NVIC_ITNS routing)
 *   - NS NVIC (VTOR, interrupt enables, priorities, SCB)
 *   - NS MPU regions
 *   - NS banked core registers (MSP_NS, PSP_NS, CONTROL_NS, etc.)
 *
 * FPCCR (FPU TrustZone bits) is also saved/restored as a security
 * measure — not required for PM3 cycling but prevents FP context
 * leaking between security domains.
 */

#include "power.h"
#include "tfm_plat_defs.h"
#include "fsl_clock.h"
#include "clock_config.h"
#include "board.h"
#include "pin_mux.h"
#include "target_cfg_common.h"
#include "utilities.h"

/* Power mode defines */
#define POWER_MODE1 1
#define POWER_MODE2 2
#define POWER_MODE3 3

/* Number of System Handler Priority registers (SHPR[0..11]) per ARMv8-M */
#define SCB_SHPR_COUNT  12U

/*------------------- NS NVIC/SCB context --------------------------------*/
/*
 * Saved before PM3, restored after warm boot.
 * The SDK's POWER_RestoreNvicState() only restores the secure NVIC
 * and SCB.  The NS side is zeroed by PM3 and must be restored from
 * secure mode since NS code never gets a chance to save its own.
 */
#define NS_NVIC_IRQREGS  5U                       /* matches SDK: 5 × 32 = 160 IRQs */
#define NS_NVIC_IRQNUM   (NS_NVIC_IRQREGS * 32U)

static struct {
    /* NVIC */
    uint32_t ISER[NS_NVIC_IRQREGS];
    uint8_t  IPR[NS_NVIC_IRQNUM];
    /* SCB */
    uint32_t VTOR;
    uint32_t AIRCR;
    uint32_t SCR;
    uint32_t CCR;
    uint32_t SHCSR;
    uint32_t CPACR;
    uint8_t  SHPR[12];
} s_ns_nvic;

static void save_ns_nvic(void)
{
    uint32_t i;

    for (i = 0U; i < NS_NVIC_IRQREGS; i++)
    {
        s_ns_nvic.ISER[i] = NVIC_NS->ISER[i];
    }
    for (i = 0U; i < NS_NVIC_IRQNUM; i++)
    {
        s_ns_nvic.IPR[i] = NVIC_NS->IPR[i];
    }

    s_ns_nvic.VTOR  = SCB_NS->VTOR;
    s_ns_nvic.AIRCR = SCB_NS->AIRCR;
    s_ns_nvic.SCR   = SCB_NS->SCR;
    s_ns_nvic.CCR   = SCB_NS->CCR;
    s_ns_nvic.SHCSR = SCB_NS->SHCSR;
    s_ns_nvic.CPACR = SCB_NS->CPACR;

    for (i = 0U; i < SCB_SHPR_COUNT; i++)
    {
        s_ns_nvic.SHPR[i] = SCB_NS->SHPR[i];
    }
}

static void restore_ns_nvic(void)
{
    uint32_t i;

    SCB_NS->VTOR  = s_ns_nvic.VTOR;
    SCB_NS->AIRCR = (s_ns_nvic.AIRCR & 0x0000FFFFU) | (0x05FAU << 16U);
    SCB_NS->SCR   = s_ns_nvic.SCR;
    SCB_NS->CCR   = s_ns_nvic.CCR;
    SCB_NS->SHCSR = s_ns_nvic.SHCSR;
    SCB_NS->CPACR = s_ns_nvic.CPACR;

    for (i = 0U; i < SCB_SHPR_COUNT; i++)
    {
        SCB_NS->SHPR[i] = s_ns_nvic.SHPR[i];
    }
    for (i = 0U; i < NS_NVIC_IRQNUM; i++)
    {
        NVIC_NS->IPR[i] = s_ns_nvic.IPR[i];
    }
    for (i = 0U; i < NS_NVIC_IRQREGS; i++)
    {
        NVIC_NS->ISER[i] = s_ns_nvic.ISER[i];
    }
}

/*------------------- NS MPU context -------------------------------------*/
/*
 * Mirrors save_mpu_state/restore_mpu_state in the non-TFM power.c,
 * but accesses NS MPU via MPU_NS from secure mode.
 */
#define NS_MPU_REGIONS  8U

static struct {
    uint32_t CTRL;
    uint32_t MAIR0;
    uint32_t MAIR1;
    uint32_t RBAR[NS_MPU_REGIONS];
    uint32_t RLAR[NS_MPU_REGIONS];
} s_ns_mpu;

static void save_ns_mpu(void)
{
    uint32_t i;

    for (i = 0U; i < NS_MPU_REGIONS; i++)
    {
        MPU_NS->RNR = i;
        s_ns_mpu.RBAR[i] = MPU_NS->RBAR;
        s_ns_mpu.RLAR[i] = MPU_NS->RLAR;
    }
    s_ns_mpu.CTRL  = MPU_NS->CTRL;
    s_ns_mpu.MAIR0 = MPU_NS->MAIR0;
    s_ns_mpu.MAIR1 = MPU_NS->MAIR1;
}

static void restore_ns_mpu(void)
{
    uint32_t i;

    /* Disable MPU before reprogramming regions */
    MPU_NS->CTRL = 0;

    MPU_NS->MAIR0 = s_ns_mpu.MAIR0;
    MPU_NS->MAIR1 = s_ns_mpu.MAIR1;

    for (i = 0U; i < NS_MPU_REGIONS; i++)
    {
        MPU_NS->RNR  = i;
        MPU_NS->RBAR = s_ns_mpu.RBAR[i];
        MPU_NS->RLAR = s_ns_mpu.RLAR[i];
    }

    /* Re-enable MPU last */
    MPU_NS->CTRL = s_ns_mpu.CTRL;
}

/*------------------- Secure MPU context ---------------------------------*/
/*
 * PM3 resets the secure MPU and the SDK does not restore it.
 * Save and restore it here to preserve TFM partition isolation.
 */
#define S_MPU_REGIONS  8U

static struct {
    uint32_t CTRL;
    uint32_t MAIR0;
    uint32_t MAIR1;
    uint32_t RBAR[S_MPU_REGIONS];
    uint32_t RLAR[S_MPU_REGIONS];
} s_s_mpu;

static void save_s_mpu(void)
{
    uint32_t i;

    for (i = 0U; i < S_MPU_REGIONS; i++)
    {
        MPU->RNR = i;
        s_s_mpu.RBAR[i] = MPU->RBAR;
        s_s_mpu.RLAR[i] = MPU->RLAR;
    }
    s_s_mpu.CTRL  = MPU->CTRL;
    s_s_mpu.MAIR0 = MPU->MAIR0;
    s_s_mpu.MAIR1 = MPU->MAIR1;
}

static void restore_s_mpu(void)
{
    uint32_t i;

    MPU->CTRL = 0;

    MPU->MAIR0 = s_s_mpu.MAIR0;
    MPU->MAIR1 = s_s_mpu.MAIR1;

    for (i = 0U; i < S_MPU_REGIONS; i++)
    {
        MPU->RNR  = i;
        MPU->RBAR = s_s_mpu.RBAR[i];
        MPU->RLAR = s_s_mpu.RLAR[i];
    }

    MPU->CTRL = s_s_mpu.CTRL;
}

/*------------------- PM entry handler -----------------------------------*/
enum tfm_platform_err_t rw61x_enter_pm_secure(uint8_t pm_mode,
                                               uint32_t idle_us,
                                               const power_sleep_config_t *slp_cfg)
{
    (void)idle_us;

    __disable_irq();
    __set_BASEPRI(0);

    switch (pm_mode)
    {
    case POWER_MODE1:
        POWER_SetSleepMode(POWER_MODE1);
        __WFI();
        break;
    case POWER_MODE2:
    {
        /* save old value of main clock mux and switch to lposc */
        uint32_t main_sel_a = CLKCTL0->MAINCLKSELA;
        uint32_t main_sel_b = CLKCTL0->MAINCLKSELB;

        CLKCTL0->MAINCLKSELA = 2;
        CLKCTL0->MAINCLKSELB = 0;
        POWER_EnterPowerMode(POWER_MODE2, slp_cfg);
        /* restore previous main clock */
        CLKCTL0->MAINCLKSELA = main_sel_a;
        CLKCTL0->MAINCLKSELB = main_sel_b;

        break;
    }
    case POWER_MODE3:
    {
        /*
         * Save NS banked core registers — only accessible from
         * secure mode via mrs/msr *_ns, zeroed by PM3.
         */
        uint32_t ns_msp, ns_psp, ns_msplim, ns_psplim;
        uint32_t ns_control, ns_primask, ns_basepri, ns_faultmask;

        __ASM volatile("mrs %0, msp_ns"       : "=r" (ns_msp));
        __ASM volatile("mrs %0, psp_ns"       : "=r" (ns_psp));
        __ASM volatile("mrs %0, msplim_ns"    : "=r" (ns_msplim));
        __ASM volatile("mrs %0, psplim_ns"    : "=r" (ns_psplim));
        __ASM volatile("mrs %0, control_ns"   : "=r" (ns_control));
        __ASM volatile("mrs %0, primask_ns"   : "=r" (ns_primask));
        __ASM volatile("mrs %0, basepri_ns"   : "=r" (ns_basepri));
        __ASM volatile("mrs %0, faultmask_ns" : "=r" (ns_faultmask));

        /* Save peripheral state that PM3 wipes */
        save_s_mpu();
        save_ns_nvic();
        save_ns_mpu();

        uint32_t saved_aircr = SCB->AIRCR;
        uint32_t saved_fpccr = FPU->FPCCR;

        if (POWER_EnterPowerMode(POWER_MODE3, slp_cfg))
        {
            /*
             * Warm boot return — ROM found the NVRAM resume address
             * and jumped back.  POWER_PostPowerMode already ran
             * initXip, SystemInit, and POWER_RestoreNvicState (secure
             * NVIC only).  Now restore everything else PM3 wiped.
             */

            /* 1. FPCCR — security measure, restore before any FPU use */
            FPU->FPCCR = saved_fpccr & ~FPU_FPCCR_LSPACT_Msk;

            /* 2. Pin mux, clocks */
            BOARD_InitPins();
            BOARD_BootClockRUN();

            /* 3. AIRCR */
            SCB->AIRCR = (saved_aircr & 0x0000FFFFU) | (0x05FAU << 16U);

            /* 4. Secure MPU — TFM partition isolation */
            restore_s_mpu();

            /* 5. TrustZone security boundaries */
            sau_and_idau_cfg();
            if (mpc_init_cfg() != 0)
            {
                tfm_core_panic();
            }
            if (ppc_init_cfg() != 0)
            {
                tfm_core_panic();
            }

            /* 6. NVIC interrupt target state (route IRQs to NS) */
            nvic_interrupt_target_state_cfg();
            nvic_interrupt_enable();

            /* 7. NS NVIC, MPU */
            restore_ns_nvic();
            restore_ns_mpu();

            /* 8. NS banked core registers — restore last */
            __ASM volatile("msr msp_ns, %0"       : : "r" (ns_msp));
            __ASM volatile("msr psp_ns, %0"       : : "r" (ns_psp));
            __ASM volatile("msr msplim_ns, %0"    : : "r" (ns_msplim));
            __ASM volatile("msr psplim_ns, %0"    : : "r" (ns_psplim));
            __ASM volatile("msr control_ns, %0"   : : "r" (ns_control));
            __ASM volatile("msr primask_ns, %0"   : : "r" (ns_primask));
            __ASM volatile("msr basepri_ns, %0"   : : "r" (ns_basepri));
            __ASM volatile("msr faultmask_ns, %0" : : "r" (ns_faultmask));
        }

        break;
    }
    default:
        __enable_irq();
        return TFM_PLATFORM_ERR_INVALID_PARAM;
    }

    __enable_irq();

    return TFM_PLATFORM_ERR_SUCCESS;
}

/*------------------- PMU IOCTL handler ---------------------------------*/
/*
 * NS drivers route PMU register access here because PMU is
 * attributed PRIV_S via PPC.
 */
enum tfm_platform_err_t rw61x_pmu_ioctl_secure(
    const struct nxp_pmu_ioctl_in *in,
    struct nxp_pmu_ioctl_out *out)
{
    switch (in->cmd)
    {
    case NXP_PMU_CMD_ENABLE_WAKEUP:
        POWER_EnableWakeup((IRQn_Type)in->arg);
        break;
    case NXP_PMU_CMD_DISABLE_WAKEUP:
        POWER_DisableWakeup((IRQn_Type)in->arg);
        break;
    case NXP_PMU_CMD_GET_WAKEUP_STATUS:
        if (out)
        {
            out->result = POWER_GetWakeupStatus((IRQn_Type)in->arg);
        }
        break;
    case NXP_PMU_CMD_CLEAR_WAKEUP_STATUS:
        POWER_ClearWakeupStatus((IRQn_Type)in->arg);
        break;
    case NXP_PMU_CMD_GET_POWER_MODE_STATUS:
        if (out)
        {
            out->result = PMU->PWR_MODE_STATUS;
        }
        break;
    case NXP_PMU_CMD_INIT_POWER_CONFIG:
        POWER_InitPowerConfig((power_init_config_t *)&in->power_cfg);
        break;
    case NXP_PMU_CMD_CLEAR_RESET_CAUSE:
        POWER_ClearResetCause(in->arg);
        break;
    case NXP_PMU_CMD_GET_WAKEUP_PINS:
        if (out)
        {
            out->result = PMU->WAKEUP_STATUS &
                (PMU_WAKEUP_STATUS_PIN0_MASK | PMU_WAKEUP_STATUS_PIN1_MASK);
        }
        break;
    case NXP_PMU_CMD_CONFIG_WAKEUP_PIN:
        /* arg packs pin in low 16 bits, level in high 16 bits */
        POWER_ConfigWakeupPin((power_wakeup_pin_t)(in->arg & 0xFFFFU),
                              (uint8_t)(in->arg >> 16));
        break;
    case NXP_PMU_CMD_ENABLE_XTAL32K:
        CLOCK_EnableXtal32K((bool)in->arg);
        if (in->arg)
        {
            CLOCK_AttachClk(kXTAL32K_to_CLK32K);
        }
        break;
    default:
        return TFM_PLATFORM_ERR_NOT_SUPPORTED;
    }

    return TFM_PLATFORM_ERR_SUCCESS;
}
