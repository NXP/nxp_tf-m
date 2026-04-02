/*
 * Copyright 2024, 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitDEBUG_UARTPins();
    BOARD_InitDebugConsole();

    /* Explicitly setting UART clock*/
    CLOCK_SetClkDiv(kCLOCK_DivPllFrgClk, 13U);         /* Set .FRGPLLCLKDIV divider to value 13 */
    CLOCK_SetFRGClock(&(const clock_frg_clk_config_t){3, kCLOCK_FrgPllDiv, 255, 0});
    CLOCK_AttachClk(kFRG_to_FLEXCOMM3);
}

void SystemInitHook(void)
{
    extern void *__VECTOR_TABLE[];
    SCB->VTOR = (uint32_t) & (__VECTOR_TABLE[0]);

    SCB->CPACR |= ((3UL << 0 * 2) | (3UL << 1 * 2)); /* set CP0, CP1 Full Access in Non-secure mode (enable PowerQuad) */
    SCB->NSACR |= ((3UL << 0) | (3UL << 10)); /* enable CP0, CP1, CP10, CP11 Non-secure Access */

    /* Board specific HW init*/
    BOARD_InitHardware();   
}

/*${function:end}*/
