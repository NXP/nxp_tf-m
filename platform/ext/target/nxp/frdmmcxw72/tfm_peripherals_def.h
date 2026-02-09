/*
 * Copyright (c) 2018-2019, Arm Limited. All rights reserved.
 * Copyright 2019-2020, 2025-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TFM_PERIPHERALS_DEF_H__
#define __TFM_PERIPHERALS_DEF_H__

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Quantized default IRQ priority, the value is:
 * (Number of configurable priority) / 4: (1UL << __NVIC_PRIO_BITS) / 4
 */
#define DEFAULT_IRQ_PRIORITY    (1UL << (__NVIC_PRIO_BITS - 2))

#define TIMER_BASE             LPTMR0                     /* Timer 0 */
#define TIMER_CLK_FREQ         (CLOCK_GetFreq(kCLOCK_RtcOscClk))
#define TIMER_IRQ_HANDLER      LPTMR0_IRQHandler
#define TFM_TIMER0_IRQ         LPTMR0_IRQn                /* (tfm_core_irq_signal_data_t->irq_line) */

#define TIMER_NS_BASE          LPTMR1                     /* Timer 1 */
#define TIMER_NS_CLK_FREQ      (CLOCK_GetFreq(kCLOCK_RtcOscClk))
#define TIMER_NS_IRQ_HANDLER   LPTMR1_IRQHandler
#define TFM_TIMER1_IRQ         LPTMR1_IRQn                /* use by tfm_core_test_irq() */

#define LPTMR_PRE_SCALER_CLOCK kLPTMR_PrescalerClock_2    /* Select ROSC as the LPTMR clock source */

struct platform_data_t;

extern struct platform_data_t tfm_peripheral_std_uart;
extern struct platform_data_t tfm_peripheral_timer0;

#define TFM_PERIPHERAL_STD_UART     (&tfm_peripheral_std_uart)
#define TFM_PERIPHERAL_TIMER0       (&tfm_peripheral_timer0)
#define TFM_PERIPHERAL_FPGA_IO      (0)

#ifdef __cplusplus
}
#endif

#endif /* __TFM_PERIPHERALS_DEF_H__ */
